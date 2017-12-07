#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <functional>

class Handler;

class Message {
public:
    Message();
    Message(int what);
    Message(int what, Handler *h);
    virtual ~Message();

public:

    /**
     * User-defined message code so that the recipient can identify
     * what this message is about.
     */
    int what;

    /**
     * arg1 and arg2 are lower-cost alternatives to using
     * if you only need to store a few integer values.
     */
    int arg1;

    /**
     * arg1 and arg2 are lower-cost alternatives to using
     * if you only need to store a few integer values.
     */
    int arg2;

    /**
     * user data
     */
    uint8_t *pData;

    Handler *target;

    std::function<int(Message*)> callback;
};


#endif
