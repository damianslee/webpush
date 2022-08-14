
#include "user_settings.h"

#if defined(NO_DEV_RANDOM) && defined(__ECOS)
#include <stdio.h>
#include <stdlib.h>
#include "wolfssl/wolfcrypt/random.h"

int custom_GenerateSeed(byte* output, word32 sz)
{
	word32 i;
	srand((unsigned int)time(NULL));

	for (i = 0; i < sz; i++) {
		output[i] = rand() % 256;
		if ((i % 8) == 7) {
			srand((unsigned int)time(NULL));
		}
	}

	return 0;
}
#endif



#if defined(CUSTOM_RAND_GENERATE_BLOCK)

#include <stdio.h>
#include <stdlib.h>

unsigned int custom_rand_generate(void)
{
	return rand();
}


int custom_rand_generate_block(unsigned char* output, unsigned int sz)
{
	unsigned int i = 0;

	while (i < sz)
	{
		/* If not aligned or there is odd/remainder */
		if ((i + sizeof(CUSTOM_RAND_TYPE)) > sz ||
			((unsigned int)&output[i] % sizeof(CUSTOM_RAND_TYPE)) != 0
			) {
			/* Single byte at a time */
			output[i++] = (unsigned char)custom_rand_generate();
		}
		else {
			/* Use native 8, 16, 32 or 64 copy instruction */
			*((CUSTOM_RAND_TYPE*)&output[i]) = custom_rand_generate();
			i += sizeof(CUSTOM_RAND_TYPE);
		}
	}

	return 0;
}
#endif
