#include "JwsSigner.h"

#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/coding.h>




std::string JwsSigner::UrlBase64Encode(const char* sz)
{
	char baseOut[1024];
	word32 baseOutLen = sizeof(baseOut);
	int rc = Base64_Encode_NoNl((byte*)sz, strlen(sz), (byte*)baseOut, &baseOutLen);
	for (int i = 0; i < baseOutLen; i++)
	{
		if (baseOut[i] == '+')
			baseOut[i] = '-';
		else if (baseOut[i] == '/')
			baseOut[i] = '_';
	}

	// trimend =
	for (int i = baseOutLen - 1; i > 0; i--)
	{
		if (baseOut[i] != '=')
			break;

		baseOut[i] = 0;
	}

	return std::string(baseOut);
}


std::string JwsSigner::GenerateSignature(const char *szJSONHeader, const char *szJSONPayload)
{
	// var securedInput = SecureInput(header, payload);
	std::string encodeHeader = UrlBase64Encode(szJSONHeader);
	std::string encodePayload = UrlBase64Encode(szJSONPayload);
	std::string message = encodeHeader + "." + encodePayload;

	// https://github.com/wolfSSL/wolfssl-examples/blob/master/hash/sha256-hash.c
	int ret = -1;
	byte  hash[WC_SHA256_DIGEST_SIZE];
	ret = wc_Sha256Hash((const byte*)message.c_str(), message.length(), hash);

	//var signer = new ECDsaSigner();
	//signer.Init(true, _privateKey);
	//var results = signer.GenerateSignature(hashedMessage);

	return "";
}
