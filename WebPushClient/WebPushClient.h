#ifndef WebPushClient_H
#define WebPushClient_H

#include <string>
#include "PushSubscription.h"
#include "VapidDetails.h"

//#include "HTTPClient.h"
//#include "HTTPRequest.h"

class WebPushClient : public CHTTPClient
{
public:
    WebPushClient();


    /// <summary>
    ///     When sending messages to a GCM endpoint you need to set the GCM API key
    ///     by either calling setGcmApiKey() or passing in the API key as an option
    ///     to sendNotification()
    /// </summary>
    /// <param name="gcmApiKey">The API key to send with the GCM request.</param>
    void SetGcmApiKey(const char* gcmApiKey);

    /// <summary>
    ///     When marking requests where you want to define VAPID details, call this method
    ///     before sendNotifications() or pass in the details and options to
    ///     sendNotification.
    /// </summary>
    /// <param name="vapidDetails"></param>
    void SetVapidDetails(VapidDetails* vapidDetails);

    /// <summary>
    ///     When marking requests where you want to define VAPID details, call this method
    ///     before sendNotifications() or pass in the details and options to
    ///     sendNotification.
    /// </summary>
    /// <param name="subject">This must be either a URL or a 'mailto:' address</param>
    /// <param name="publicKey">The public VAPID key as a base64 encoded std::string</param>
    /// <param name="privateKey">The private VAPID key as a base64 encoded std::string</param>
    void SetVapidDetails(std::string subject, std::string publicKey, std::string privateKey);

    /// <summary>
    ///     To get a request without sending a push notification call this method.
    ///     This method will throw an ArgumentException if there is an issue with the input.
    /// </summary>
    /// <param name="subscription">The PushSubscription you wish to send the notification to.</param>
    /// <param name="payload">The payload you wish to send to the user</param>
    /// <param name="options">
    ///     Options for the GCM API key and vapid keys can be passed in if they are unique for each
    ///     notification.
    /// </param>
    /// <returns>A HttpRequestMessage object that can be sent.</returns>
    CHTTPClientRequest* GenerateRequestDetails(PushSubscription& subscription, std::string payload);

protected:

    virtual void OnHTTPServerResponse(CHTTPClientRequest& request, CHTTPClientResponse& response);

private:
    // default TTL is 4 weeks.
    const int DefaultTtl = 2419200;
    //HttpClientHandler _httpClientHandler;

    std::string _gcmApiKey;
    //HttpClient _httpClient;
    VapidDetails* _vapidDetails;

    // Used so we only cleanup internally created http clients
    bool _isHttpClientInternallyCreated;

};

#endif
