#ifndef PushSubscription_h
#define PushSubscription_h

#include <string>

class PushSubscription
{
public:
    PushSubscription()
    {
    }

    PushSubscription(std::string endpoint, std::string p256dh, std::string auth)
    {
        _Endpoint = endpoint;
        _P256DH = p256dh;
        _Auth = auth;
    }

    std::string _Endpoint;
    std::string _P256DH;
    std::string _Auth;
};

#endif
