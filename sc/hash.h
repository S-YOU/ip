#ifndef BASE_H_
#define BASE_H_

#include "base.h"

#define HASH(TYPE, KEY, KEY_LEN, SIZE) \
	unsigned long h = 0;\
	const TYPE *_key = KEY;\
	unsigned _key_len = KEY_LEN;\
	if (_key_len) {\
		while (_key_len--) {\
			h = h * 97 + *_key++;\
		}\
	}\
	h = h % SIZE;

#define HASH_STR(X, SIZE) HASH(CHR, X, X##_len, SIZE)
#define HASH_UNI(X, SIZE) HASH(UNICHR, X, X##_len, SIZE)
#define HASH_INT(X, SIZE) \
	unsigned long h = X % SIZE;

#endif
