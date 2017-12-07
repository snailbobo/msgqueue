
#include "MessageQueue.hpp"

bool MessageQueue::empty() {

    std::unique_lock<std::mutex> lck(mMtx);
    bool bEmpty = mMsgQueue.empty();
    return bEmpty;
}

Message* MessageQueue::next() {

    std::unique_lock<std::mutex> lck(mMtx);
    while (mMsgQueue.empty()) {
        mCV.wait(lck);
    }

    Message *pMsg = mMsgQueue.front();
    mMsgQueue.pop();

    return pMsg;
}

int MessageQueue::enqueueMessage(Message * pMsg) {

    if (pMsg == nullptr) {
        return -1;
    }

    std::unique_lock<std::mutex> lck(mMtx);
    mMsgQueue.push(pMsg);
    mCV.notify_all();

    return 0;
}
