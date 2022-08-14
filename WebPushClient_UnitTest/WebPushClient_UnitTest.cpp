#include "pch.h"
#include "CppUnitTest.h"


#include "JwsSigner.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace WebPushClientUnitTest
{
	TEST_CLASS(WebPushClientUnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			JwsSigner signer;
			signer.GenerateSignature("{{\"typ\", \"JWT\"}, {\"alg\", \"ES256\"}}", "{{\"aud\", \"audience\" }, {\"exp\", expiration}, {\"sub\", \"subject\"}}");
		}
	};
}
