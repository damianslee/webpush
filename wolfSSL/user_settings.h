#ifndef _EMM_USER_SETTINGS_H_
#define _EMM_USER_SETTINGS_H_

// Damian Slee settings for ECOS WolfSSL build

#if defined(__linux__)
	// assume x64

#elif defined(WIN32)
	#if _DEBUG
		#if !defined(_CRTDBG_MAP_ALLOC)
			#define _CRTDBG_MAP_ALLOC
		#endif
		#include <crtdbg.h>
	#endif
	#include <stdio.h>
	//#define WOLFSSL_AESNI 

#elif defined(__ECOS)
	#define WOLFSSL_SMALL_STACK
	#define NO_DEV_RANDOM
	#define CUSTOM_RAND_GENERATE_SEED custom_GenerateSeed
#endif


#if !defined(__ECOS)
	// Other CPUs can take more work
	#define WOLFSSL_SHA512
	#define TFM_TIMING_RESISTANT
	#define ECC_TIMING_RESISTANT
	#define WC_RSA_BLINDING
#endif


#if defined(_DEBUG) && !defined(__ECOS)
	#define DEBUG_WOLFSSL
	//#define DEBUG_WOLFSSL_VERBOSE
	//#define WOLFSSL_DEBUG_TLS
	//#define WOLFSSL_DEBUG_MATH
	//#define WOLFSSL_DEBUG_PKCS12
	#define WOLFSSL_DEBUG_MEMORY
	//#define WOLFSSL_DEBUG_MEMORY_PRINT
	#define WOLFSSL_TRACK_MEMORY
	#define DO_MEM_LIST
	#define DO_MEM_STATS
#else
	// release only
#endif


// Common settings for Multi-OS/Any build
#define WOLFSSL_TLS13
#define NO_OLD_TLS
#define HAVE_TLS_EXTENSIONS
#define HAVE_SUPPORTED_CURVES
#define HAVE_FALLBACK_SCSV
//#define HAVE_OCSP
#define HAVE_ENCRYPT_THEN_MAC /* https://tools.ietf.org/html/rfc7366 */

// Enables ciphers
#define HAVE_AESGCM
#define HAVE_POLY1305
#define HAVE_CHACHA
#define HAVE_ECC
#define TFM_ECC256
#define ECC_SHAMIR
#define WOLFSSL_SHA384
#define HAVE_FFDHE_2048
#define WC_RSA_PSS
#define USE_FAST_MATH   /* mainly RSA */
#define FP_MAX_BITS (4096*2)  /* USE_FAST_MATH - Set to largest RSA key size times 2 (4096*2) = 8192 */


// random number generation
#define HAVE_HASHDRBG

// PKCS #5 Password based cryptography 
#define HAVE_PBKDF2

// Disabled features
#define NO_DSA
#define NO_PSK
#define NO_MD4
//#define NO_PWDBASED
#define WC_NO_ASYNC_THREADING
#define NO_DES3
#define NO_RC4
#define NO_HC128
#define NO_RABBIT

// Integration with EMM
#define WOLFSSL_USER_IO
#define NO_FILESYSTEM
#define NO_WRITEV
#define WOLFSSL_NO_SOCK
//#define WOLFSSL_CALLBACKS


// Certificates
#define WOLFSSL_KEY_GEN
#define WOLFSSL_CERT_GEN
#define WOLFSSL_CERT_REQ
//#define OPENSSL_EXTRA_X509_SMALL
#define OPENSSL_EXTRA
#define HAVE_HKDF
#define HAVE_ONE_TIME_AUTH
#define HAVE_EXTENDED_MASTER


// not used
//#define HAVE_CERTIFICATE_STATUS_REQUEST
//#define HAVE_CERTIFICATE_STATUS_REQUEST_V2
//#define ALT_ECC_SIZE
//#define HAVE_THREAD_LS



#endif /* _EMM_USER_SETTINGS_H_ */
