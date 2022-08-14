#ifndef JwsSigner_H
#define JwsSigner_H

#include <string>

class JwsSigner
{
public:
	std::string GenerateSignature(const char* szHeader, const char* szPayload);

private:
	std::string UrlBase64Encode(const char* sz);
};


#endif