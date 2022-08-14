// EMM includes
#include "FusionOS.h"

#include "HTTPClient.h"
#include "HTTPRequest.h"
#include "WebPushClient.h"


WebPushClient::WebPushClient()
    : CHTTPClient(true, "WebPushCLient")
{
    SetKeepAlive(true);
    SetContentType("application/octet-stream");
}


#if false


/// <summary>
///     When sending messages to a GCM endpoint you need to set the GCM API key
///     by either calling setGcmApiKey() or passing in the API key as an option
///     to sendNotification()
/// </summary>
/// <param name="gcmApiKey">The API key to send with the GCM request.</param>
void WebPushClient::SetGcmApiKey(const char *gcmApiKey)
{
    if (gcmApiKey == NULL)
    {
        return;
    }

    if (strlen(gcmApiKey) == 0)
    {
        return;
    }

    _gcmApiKey = gcmApiKey;
}

/// <summary>
///     When marking requests where you want to define VAPID details, call this method
///     before sendNotifications() or pass in the details and options to
///     sendNotification.
/// </summary>
/// <param name="vapidDetails"></param>
void WebPushClient::SetVapidDetails(VapidDetails& vapidDetails)
{
    VapidHelper.ValidateSubject(vapidDetails.Subject);
    VapidHelper.ValidatePublicKey(vapidDetails.PublicKey);
    VapidHelper.ValidatePrivateKey(vapidDetails.PrivateKey);

    _vapidDetails = vapidDetails;
}

/// <summary>
///     When marking requests where you want to define VAPID details, call this method
///     before sendNotifications() or pass in the details and options to
///     sendNotification.
/// </summary>
/// <param name="subject">This must be either a URL or a 'mailto:' address</param>
/// <param name="publicKey">The public VAPID key as a base64 encoded string</param>
/// <param name="privateKey">The private VAPID key as a base64 encoded string</param>
void WebPushClient::SetVapidDetails(string subject, string publicKey, string privateKey)
{
    SetVapidDetails(new VapidDetails(subject, publicKey, privateKey));
}

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
CHTTPClientRequest* WebPushClient::GenerateRequestDetails(PushSubscription &subscription, string payload)
{

    //if (!Uri.IsWellFormedUriString(subscription.Endpoint, UriKind.Absolute))
    //{
    //    throw new ArgumentException(@"You must pass in a subscription with at least a valid endpoint");
    //}

    //var request = new HttpRequestMessage(HttpMethod.Post, subscription.Endpoint);

    //if (!string.IsNullOrEmpty(payload) && (string.IsNullOrEmpty(subscription.Auth) ||
    //                                        string.IsNullOrEmpty(subscription.P256DH)))
    //{
    //    throw new ArgumentException(
    //        @"To send a message with a payload, the subscription must have 'auth' and 'p256dh' keys.");
    //}

/* vapid
Example POSTed subscription

{
    "subscription": {
        "endpoint": "https://fcm.googleapis.com/fcm/send/f5xb46SfQNQ:APA91bEar4rrM9aJYNGcocPNrh4cxs8nGcFxgARBLIwLPRvIjCc3zrpZCEDVQUFaW_h-uDcok7_JZGQtkPXZYmzDLCRPoIjApvhtRtvn6lHvZ_XspPlZirXBnJsPB6UWvoXnAE_T_vWy",
        "expirationTime": null,
        "keys": {
            "p256dh": "BHLaZC9Hbem3Y4VsBeRXq8nTRsPo8CMo2exyh6iGrEdF0aGktTG8dBuFoWzMkyuHzPllkTtlu1P1PnKkoj8uFMU",
            "auth": "b75NrXZ1_mj719GijCUClA"
        }
    }
}

*/

    size_t iProtocol = subscription._Endpoint.find("://");
    if (iProtocol <= 0)
        return NULL;

    size_t iDomain = subscription._Endpoint.find("/", iProtocol+3);
    if (iDomain <= 0)
        return NULL;

    string host = subscription._Endpoint.substr(iProtocol, iDomain);

    size_t iPath = subscription._Endpoint.find('/', iDomain + 1);
    if (iPath <= 0)
        return NULL;

    string path = subscription._Endpoint.substr(iDomain+1, iPath);

    CHTTPClientRequest* clientRequest = new CHTTPClientRequest();
    if (!clientRequest)
        return NULL;

    //clientRequest->m_pUserData = pUserData;
    clientRequest->m_nVerb = HTTP_VERB_POST;
    clientRequest->m_sHost = host;
    clientRequest->m_nPort = 443;
    clientRequest->m_sRequest.assign(path);
    //if (pabBody)
        //  clientRequest->m_abBody.Attach(*pabBody);  // re-assign the ptr to us.  detach from caller

    std::string currentGcmApiKey = _gcmApiKey;
    VapidDetails currentVapidDetails = _vapidDetails;
    int timeToLive = DefaultTtl;
    
    string cryptoKeyHeader;

    if (payload.length() > 0)
    {
        if ((subscription._P256DH.length() > 0) || (subscription._Auth.length() > 0))
        {
            // Unable to send a message with payload to this subscription since it doesn't have the required encryption key
            return NULL;
        }

        var encryptedPayload = EncryptPayload(subscription, payload);

        request.Content = new ByteArrayContent(encryptedPayload.Payload);
        request.Content.Headers.ContentType = new MediaTypeHeaderValue("application/octet-stream");
        request.Content.Headers.ContentLength = encryptedPayload.Payload.Length;
        request.Content.Headers.ContentEncoding.Add("aesgcm");
        request.Headers.Add("Encryption", "salt=" + encryptedPayload.Base64EncodeSalt());
        cryptoKeyHeader = @"dh=" + encryptedPayload.Base64EncodePublicKey();
    }


    bool isGcm = subscription._Endpoint.find("https://android.googleapis.com/gcm/send");
    bool isFcm = subscription._Endpoint.find("https://fcm.googleapis.com/fcm/send/");

    if (isGcm)
    {
        if (currentGcmApiKey.length() > 0)
        {
            clientRequest->m_abBody.Append("Authorization: key=");
            clientRequest->m_abBody.Append(currentGcmApiKey);
            clientRequest->m_abBody.Append("\r\n");
        }
    }
    else if (_vapidDetails != NULL)
    {
        var uri = new Uri(subscription.Endpoint);
        var audience = uri.Scheme + @"://" + uri.Host;

        var vapidHeaders = VapidHelper.GetVapidHeaders(audience, currentVapidDetails.Subject,
            currentVapidDetails.PublicKey, currentVapidDetails.PrivateKey, currentVapidDetails.Expiration);
        request.Headers.Add(@"Authorization", vapidHeaders["Authorization"]);
        if (string.IsNullOrEmpty(cryptoKeyHeader))
        {
            cryptoKeyHeader = vapidHeaders["Crypto-Key"];
        }
        else
        {
            cryptoKeyHeader += @";" + vapidHeaders["Crypto-Key"];
        }
    }
    else if (isFcm && !string.IsNullOrEmpty(currentGcmApiKey))
    {
        request.Headers.TryAddWithoutValidation("Authorization", "key=" + currentGcmApiKey);
    }

    request.Headers.Add("Crypto-Key", cryptoKeyHeader);
    return request;
}

EncryptionResult WebPushClient::EncryptPayload(PushSubscription subscription, string payload)
{
    try
    {
        return Encryptor.Encrypt(subscription.P256DH, subscription.Auth, payload);
    }
    catch (Exception ex)
    {
        if (ex is FormatException || ex is ArgumentException)
        {
            throw new InvalidEncryptionDetailsException("Unable to encrypt the payload with the encryption key of this subscription.", subscription);
        }

        throw;
    }
}

/// <summary>
///     To send a push notification call this method with a subscription, optional payload and any options
///     Will exception if unsuccessful
/// </summary>
/// <param name="subscription">The PushSubscription you wish to send the notification to.</param>
/// <param name="payload">The payload you wish to send to the user</param>
/// <param name="options">
///     Options for the GCM API key and vapid keys can be passed in if they are unique for each
///     notification.
/// </param>
void WebPushClient::SendNotification(PushSubscription subscription, string payload = null,
    Dictionary<string, object> options = null)
{
    SendNotificationAsync(subscription, payload, options).ConfigureAwait(false).GetAwaiter().GetResult();
}

/// <summary>
///     To send a push notification call this method with a subscription, optional payload and any options
///     Will exception if unsuccessful
/// </summary>
/// <param name="subscription">The PushSubscription you wish to send the notification to.</param>
/// <param name="payload">The payload you wish to send to the user</param>
/// <param name="vapidDetails">The vapid details for the notification.</param>
void WebPushClient::SendNotification(PushSubscription subscription, string payload, VapidDetails vapidDetails)
{
    var options = new Dictionary<string, object> { ["vapidDetails"] = vapidDetails };
    SendNotification(subscription, payload, options);
}

/// <summary>
///     To send a push notification call this method with a subscription, optional payload and any options
///     Will exception if unsuccessful
/// </summary>
/// <param name="subscription">The PushSubscription you wish to send the notification to.</param>
/// <param name="payload">The payload you wish to send to the user</param>
/// <param name="gcmApiKey">The GCM API key</param>
void WebPushClient::SendNotification(PushSubscription subscription, string payload, string gcmApiKey)
{
    var options = new Dictionary<string, object> { ["gcmAPIKey"] = gcmApiKey };
    SendNotification(subscription, payload, options);
}


/// <summary>
///     To send a push notification asynchronous call this method with a subscription, optional payload and any options
///     Will exception if unsuccessful
/// </summary>
/// <param name="subscription">The PushSubscription you wish to send the notification to.</param>
/// <param name="payload">The payload you wish to send to the user</param>
/// <param name="options">
///     Options for the GCM API key and vapid keys can be passed in if they are unique for each
///     notification.
/// </param>
/// <param name="cancellationToken">The cancellation token to cancel operation.</param>
async Task WebPushClient::SendNotificationAsync(PushSubscription subscription, string payload = null,
    Dictionary<string, object> options = null, CancellationToken cancellationToken = default)
{
    var request = GenerateRequestDetails(subscription, payload, options);
    var response = await HttpClient.SendAsync(request, cancellationToken).ConfigureAwait(false);

    await HandleResponse(response, subscription).ConfigureAwait(false);
}

/// <summary>
///     To send a push notification asynchronous call this method with a subscription, optional payload and any options
///     Will exception if unsuccessful
/// </summary>
/// <param name="subscription">The PushSubscription you wish to send the notification to.</param>
/// <param name="payload">The payload you wish to send to the user</param>
/// <param name="vapidDetails">The vapid details for the notification.</param>
/// <param name="cancellationToken"></param>
async Task WebPushClient::SendNotificationAsync(PushSubscription subscription, string payload,
    VapidDetails vapidDetails, CancellationToken cancellationToken = default)
{
    var options = new Dictionary<string, object> { ["vapidDetails"] = vapidDetails };
    await SendNotificationAsync(subscription, payload, options, cancellationToken).ConfigureAwait(false);
}

/// <summary>
///     To send a push notification asynchronous call this method with a subscription, optional payload and any options
///     Will exception if unsuccessful
/// </summary>
/// <param name="subscription">The PushSubscription you wish to send the notification to.</param>
/// <param name="payload">The payload you wish to send to the user</param>
/// <param name="gcmApiKey">The GCM API key</param>
/// <param name="cancellationToken"></param>
async Task WebPushClient::SendNotificationAsync(PushSubscription subscription, string payload, string gcmApiKey, CancellationToken cancellationToken = default)
{
    var options = new Dictionary<string, object> { ["gcmAPIKey"] = gcmApiKey };
    await SendNotificationAsync(subscription, payload, options, cancellationToken).ConfigureAwait(false);
}


/// <summary>
///     Handle Web Push responses.
/// </summary>
/// <param name="response"></param>
/// <param name="subscription"></param>
void OnHTTPServerResponse(CHTTPClientRequest& request, CHTTPClientResponse& response)
{
    // Successful
    if (response.IsSuccessStatusCode)
    {
        return;
    }

    // Error
    var responseCodeMessage = @"Received unexpected response code: " + (int)response.StatusCode;
    switch (response.StatusCode)
    {
        case HttpStatusCode.BadRequest:
            responseCodeMessage = "Bad Request";
            break;

        case HttpStatusCode.RequestEntityTooLarge:
            responseCodeMessage = "Payload too large";
            break;

        case (HttpStatusCode)429:
            responseCodeMessage = "Too many request";
            break;

        case HttpStatusCode.NotFound:
        case HttpStatusCode.Gone:
            responseCodeMessage = "Subscription no longer valid";
            break;
    }

    string details = null;
    if (response.Content != null)
    {
        details = await response.Content.ReadAsStringAsync().ConfigureAwait(false); 
    }

    var message = string.IsNullOrEmpty(details)
        ? responseCodeMessage
        : $"{responseCodeMessage}. Details: {details}";

    throw new WebPushException(message, subscription, response);
}

#endif

