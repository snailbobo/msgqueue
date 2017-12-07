
#include <iostream>
#include "Log.hpp"
#include "Looper.hpp"
#include "Handler.hpp"

Looper::Looper() {

}

Looper::~Looper() {

}

int Looper::prepare() {

    LOGI("### Looper::prepare() ###");
    // TODO prepare

    return 0;
}

void Looper::loop() {

    LOGI("### Looper::loop() ###");

    MessageQueue &queue = mMsgQueue;

    for (;;) {
        Message *msg = queue.next();        // might block
        if (msg == nullptr) {
            // No message
            break;
        }

        if (msg->target != nullptr) {
            Handler *pHandler = msg->target;
            pHandler->dispatchMessage(msg);
        }

        delete msg;
        msg = nullptr;
    }
}

