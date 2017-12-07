#ifndef _HANDLER_HPP_
#define _HANDLER_HPP_

#include "Message.hpp"
#include "MessageQueue.hpp"
#include "Looper.hpp"

class Handler {
public:

    Handler();

    Handler(Looper *pLooper);

    virtual ~Handler();

    int sendMessage(Message *pMsg);

    int dispatchMessage(Message *pMsg);

    virtual Message* obtainMessage(int what);

    virtual int handleMessage(Message *pMsg);

private:

    int enqueueMessage(MessageQueue *pQueue, Message *pMsg);

    MessageQueue *mpQueue;
};

#endif
