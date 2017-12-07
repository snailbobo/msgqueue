
#include "Handler.hpp"

Handler::Handler() {

    //Looper *pLooper = Looper::getInstance();
    //mpQueue = &pLooper->mMsgQueue;
    mpQueue = nullptr;
}

Handler::Handler(Looper *pLooper) {

    //if (pLooper == nullptr) {
        //pLooper = Looper::getInstance();
    //}
    mpQueue = nullptr;
    if (pLooper != nullptr) {
        mpQueue = &pLooper->mMsgQueue;
    }
}

Handler::~Handler() {

}

Message* Handler::obtainMessage(int what) {
    Message *pMsg = new Message(what, this);
    return pMsg;
}

int Handler::sendMessage(Message *pMsg) {

    if ((pMsg == nullptr) || (mpQueue == nullptr)) {
        return -1;
    }

    if (pMsg->target == nullptr) {
        pMsg->target = this;
    }

    return enqueueMessage(mpQueue, pMsg);
}

int Handler::dispatchMessage(Message *pMsg) {

    if (pMsg == nullptr) {
        return -1;
    }

    int rc = 0;

    if (pMsg->callback != nullptr) {
        // log
        rc = pMsg->callback(pMsg);
    } else {
        // log
        rc = handleMessage(pMsg);
    }

    return rc;
}

int Handler::handleMessage(Message *pMsg) {

    return 0;
}

int Handler::enqueueMessage(MessageQueue *pQueue, Message *pMsg) {

    pMsg->target = this;
    int rc = mpQueue->enqueueMessage(pMsg);
    // log
    return rc;
}


