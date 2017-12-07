
#include "Message.hpp"
#include "Handler.hpp"

Message::Message()
: what(0), arg1(0), arg2(0), pData(nullptr)
, target(nullptr), callback(nullptr) {

}

Message::Message(int what)
: what(what), arg1(0), arg2(0), pData(nullptr)
, target(nullptr), callback(nullptr) {

}

Message::Message(int what, Handler *h)
: what(what), arg1(0), arg2(0), pData(nullptr)
, target(h), callback(nullptr) {

}

Message::~Message() {
    if (pData != nullptr) {
        delete[] pData;
        pData = nullptr;
    }
}


