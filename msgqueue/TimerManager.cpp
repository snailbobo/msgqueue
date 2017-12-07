
#include "TimerManager.hpp"

using namespace std;

std::shared_ptr<TimerManager> TimerManager::mpInstance = nullptr;

std::shared_ptr<TimerManager> TimerManager::getInstance() {
    if (mpInstance == nullptr) {
        mpInstance = std::make_shared<TimerManager>();
    }
    return mpInstance;
}

TimerManager::TimerManager()
    : mTimerSet(comparator)
    , mTimerQid(0)
    , mDoneFlag(false)
{
	unique_lock<std::mutex> lock(mSyncMutex);
	mWorkerThread = std::thread(std::bind(&TimerManager::timerThread, this));
}

TimerManager::~TimerManager()
{
	mDoneFlag = true;
	mWakeUpSignal.notify_all();
	mWorkerThread.join();
}

uint32_t TimerManager::startTimer(std::shared_ptr<TimerTask> task, uint32_t delay, uint32_t period)
{
	unique_lock<std::mutex> lock(mSyncMutex);
	std::shared_ptr<Timer> pTimer = std::make_shared<Timer>(mTimerQid++,
                (chrono::steady_clock::now() + chrono::milliseconds(delay)),
                chrono::milliseconds(period),
                task);

    LOGI("startTimer: timerID(0x%x)", pTimer->mTimerID);

	auto iter = mTimerMap.emplace(pTimer->mTimerID, pTimer);
	mTimerSet.insert(iter.first->second);
	mWakeUpSignal.notify_all();

	return pTimer->mTimerID;
}

uint32_t TimerManager::startTimer(timerFunc cb, uint32_t delay, uint32_t period)
{
	unique_lock<std::mutex> lock(mSyncMutex);
	std::shared_ptr<Timer> pTimer = std::make_shared<Timer>(mTimerQid++,
                (chrono::steady_clock::now() + chrono::milliseconds(delay)),
                chrono::milliseconds(period),
                cb);

    LOGI("startTimer: timerID(0x%x)", pTimer->mTimerID);

	auto iter = mTimerMap.emplace(pTimer->mTimerID, pTimer);
	mTimerSet.insert(iter.first->second);
	mWakeUpSignal.notify_all();

	return pTimer->mTimerID;
}

int TimerManager::stopTimer(uint32_t timerID)
{
    LOGI("stopTimer: timerID(0x%x)", timerID);

	unique_lock<std::mutex> lock(mSyncMutex);
    auto index = mTimerMap.find(timerID);
    if (index == mTimerMap.end())
    {
        LOGW("stopTimer: timer not exit, timerID(0x%x).", timerID);
        return -1;
    }
    else if (index->second->mFlagRunning)
    {
        index->second->mFlagRunning = false;
    }
    else
    {
		mTimerSet.erase(ref(index->second));
		mTimerMap.erase(index);
    }

	mWakeUpSignal.notify_all();
    return true;
}

bool TimerManager::checkTimerExists(uint32_t timerID)
{
	unique_lock<std::mutex> lock(mSyncMutex);
    return mTimerMap.find(timerID) != mTimerMap.end();
}

void TimerManager::timerThread()
{
    LOGI("### Timer Thread Run ###");

	unique_lock<std::mutex> lock(mSyncMutex);

    while (!mDoneFlag)
    {
        if (mTimerSet.empty())
        {
            // Wait (forever) for work
			mWakeUpSignal.wait(lock);
        }
        else
        {
            auto firstTimer = mTimerSet.begin();
			std::shared_ptr<Timer> pTimer = *firstTimer;
			if (pTimer == nullptr) {
                mTimerSet.erase(firstTimer);
                break;
			}

            auto now = chrono::steady_clock::now();
            if (now >= pTimer->mStamp)
            {
				mTimerSet.erase(firstTimer);
				pTimer->mFlagRunning = true;

                lock.unlock();
				std::shared_ptr<TimerTask> pTask = pTimer->mpTask;
				if (pTask != nullptr) {
                    pTask->run();
				}

				timerFunc cb = pTimer->cbFunc;
				if (cb != nullptr) {
                    cb(pTimer->mTimerID);
				}
                lock.lock();

                if (mDoneFlag)
                {
                    break;
                }
                else if (!pTimer->mFlagRunning)
                {
					mTimerMap.erase(pTimer->mTimerID);
                }
                else
                {
					pTimer->mFlagRunning = false;

                    if (pTimer->mPeriod.count() > 0)
                    {
						pTimer->mStamp = pTimer->mStamp + pTimer->mPeriod;
						mTimerSet.insert(pTimer);
                    } else {
						mTimerMap.erase(pTimer->mTimerID);
                    }
                }
            } else {

				mWakeUpSignal.wait_until(lock, pTimer->mStamp);
            }
        }
    }
}



