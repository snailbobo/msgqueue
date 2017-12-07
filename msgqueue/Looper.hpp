#ifndef _LOOPER_HPP_
#define _LOOPER_HPP_

#include "MessageQueue.hpp"

class Looper {

public:

    Looper();

    virtual ~Looper();

    int prepare();

    void loop();

private:

    //Looper();

public:

    MessageQueue mMsgQueue;

private:

    static Looper *mpInstance;

};


#endif
