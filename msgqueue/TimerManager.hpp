
#ifndef __MANGER_HPP_
#define __MANGER_HPP_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <set>
#include <unordered_map>
#include "Message.hpp"
#include "Log.hpp"

using namespace std;

typedef std::function<void(uint32_t)> timerFunc;

class TimerTask {

public:
    TimerTask(){}
    virtual ~TimerTask(){}
    virtual void run() = 0;

};

class TimerManager
{
public:

    static std::shared_ptr<TimerManager> getInstance();

    TimerManager();

    ~TimerManager();

    /**
     * Schedules the specified task for repeated fixed-delay execution,
     * beginning after the specified delay.
     * @param task   task to be scheduled.
     * @param delay  delay in milliseconds before task is to be executed.
     * @param period time in milliseconds between successive task executions.
     * @return returns timer id.
     */
    uint32_t startTimer(std::shared_ptr<TimerTask> task, uint32_t delay, uint32_t period = 0);

    /**
     * Schedules the specified task for repeated fixed-delay execution,
     * beginning after the specified delay.
     * @param cb   callback function to be scheduled.
     * @param delay  delay in milliseconds before task is to be executed.
     * @param period time in milliseconds between successive task executions.
     * @return returns timer id.
     */
    uint32_t startTimer(timerFunc cb, uint32_t delay, uint32_t period = 0);

    /**
     * Stop the specified timer task by timer id.
     * @return returns 0 if the timer is stopped successfully.
     */
    int stopTimer(uint32_t timerID);

    /**
     * check whether the specified timer is exist or not.
     * @return returns true if the timer is exist.
     */
    bool checkTimerExists(uint32_t timerID);

private:

    void timerThread();

private:

    class Timer
    {
    public:

		Timer(uint32_t id, chrono::time_point<chrono::steady_clock> stamp, chrono::milliseconds period, std::shared_ptr<TimerTask> task)
		: mTimerID(id)
		, mStamp(stamp)
		, mPeriod(period)
		, mpTask(task)
		, cbFunc(nullptr)
		, mFlagRunning(false) {

		}

		Timer(uint32_t id, chrono::time_point<chrono::steady_clock> stamp, chrono::milliseconds period, timerFunc cb)
		: mTimerID(id)
		, mStamp(stamp)
		, mPeriod(period)
		, mpTask(nullptr)
		, cbFunc(move(cb))
		, mFlagRunning(false) {

		}

		~Timer() {
		}

        uint32_t mTimerID;
        chrono::time_point<chrono::steady_clock> mStamp;
        chrono::milliseconds mPeriod;
        std::shared_ptr<TimerTask> mpTask;
        timerFunc cbFunc;
        bool mFlagRunning;
    };

    struct TimerComparator
    {
        bool operator()(const std::shared_ptr<Timer> timerA, const std::shared_ptr<Timer> timerB) const
        {
			return timerA->mStamp < timerB->mStamp;
        }
    };

    mutex mSyncMutex;
    condition_variable mWakeUpSignal;

    unordered_map<uint32_t, std::shared_ptr<Timer>> mTimerMap;

	TimerComparator comparator;
	multiset<std::shared_ptr<Timer>, TimerComparator> mTimerSet;

	uint32_t mTimerQid;

    std::thread mWorkerThread;
    bool mDoneFlag;

    static std::shared_ptr<TimerManager> mpInstance;
};

#endif // ASSISTCALL_SRC_C_MANGER_H
