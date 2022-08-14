#ifndef VapidDetails_h
#define VapidDetails_h

#include <string>

class VapidDetails
{
public:
    VapidDetails()
    {
        _Expiration = -1;
    }

    /// <param name="subject">This should be a URL or a 'mailto:' email address</param>
    /// <param name="publicKey">The VAPID public key as a base64 encoded string</param>
    /// <param name="privateKey">The VAPID private key as a base64 encoded string</param>
    VapidDetails(std::string subject, std::string publicKey, std::string privateKey)
    {
        _Expiration = -1;
        _Subject = subject;
        _PublicKey = publicKey;
        _PrivateKey = privateKey;
    }

    std::string _Subject;
    std::string _PublicKey;
    std::string _PrivateKey;

    long _Expiration;
};

#endif
