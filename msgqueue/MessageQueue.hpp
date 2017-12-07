#ifndef _MESSAGE_QUEUE_HPP_
#define _MESSAGE_QUEUE_HPP_

#include <mutex>
#include <condition_variable>
#include <queue>
#include "Message.hpp"

class MessageQueue {
public:

    MessageQueue(){};

    virtual ~MessageQueue(){};

    bool empty();

    Message* next();

    int enqueueMessage(Message* pMsg);

private:

    std::mutex mMtx;

    std::condition_variable mCV;

    std::queue<Message*> mMsgQueue;

};

#endif
