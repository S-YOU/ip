#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "base.h"
#include "hash.h"

#define ATTR_COPY_STR(X) \
	e->X = d->_b;\
	e->X##_len = X##_len;\
	memcpy(d->_b, X, ++X##_len);\
	d->_b += X##_len;
#define ATTR_COPY_UNI(X) \
	e->X = (UNICHR*) d->_b;\
	e->X##_len = X##_len;\
	memcpy(d->_b, X, sizeof(UNICHR) * ++X##_len);\
	d->_b += sizeof(UNICHR) * X##_len;
#define ATTR_COPY_INT(X) e->X = X;
#define ATTR_COPY_U8(X) e->X = X;

#define OBJECT_ITEM_STRUCT(TYPE,INNER) \
	struct TYPE##Item_s {\
		struct TYPE##Item_s *next;\
		struct TYPE##Item_s *prev;\
		INNER\
	} TYPE##Item_s;\
	typedef struct TYPE##Item_s *TYPE##Item;

#define OBJECT_TYPE_STRUCT(TYPE,EXTRA) \
	struct TYPE##_s {\
		DATA_TYPE type;\
		unsigned len;\
		unsigned _size;\
		unsigned _buf_len;\
		unsigned _alloc;\
		char *_b;\
		char *_b_end;\
		char **_buf;\
		EXTRA\
	} TYPE##_s;\
	typedef struct TYPE##_s *TYPE;\
	int TYPE##Init(TYPE*, unsigned size, unsigned item_size_hint);\
	void TYPE##Destroy(TYPE);

#define OBJECT_STRUCT(TYPE,CONTENT) \
	OBJECT_ITEM_STRUCT(TYPE,CONTENT)\
	OBJECT_TYPE_STRUCT(TYPE,\
		TYPE##Item *table;\
		TYPE##Item last;\
	)

#define OBJECT_INIT_MEMSET(TYPE) \
	OBJECT_INIT(TYPE,\
		d->last = 0;\
		d->table = malloc(sizeof(void*) * d->_size);\
		memset(d->table, 0, sizeof(void*) * d->_size);\
	)

#define OBJECT_INIT_NOBUF(TYPE,EXTRA) \
	int TYPE##Init(TYPE *dp, unsigned size, unsigned item_size_hint) {\
		TYPE d = *dp = malloc(sizeof(TYPE##_s));\
		if (size < 8) size = 8;\
		if (item_size_hint < 8) item_size_hint = 8;\
		d->_size = size;\
		d->_alloc = d->_size * item_size_hint;\
		d->len = 0;\
		EXTRA\
		return RETVAL_OK;\
	}

#define OBJECT_INIT(TYPE,EXTRA) \
	OBJECT_INIT_NOBUF(TYPE,\
		d->_buf_len = 0;\
		d->_buf = malloc(sizeof(void*) * MAX_BUF_ALLOC);\
		d->_b = d->_buf[0] = malloc(d->_alloc);\
		d->_b_end = d->_b + d->_alloc - sizeof(TYPE##Item_s) - 2;\
		EXTRA\
	)

#define OBJECT_DESTROY_NOBUF(TYPE,EXTRA) \
	void TYPE##Destroy(TYPE d) {\
		EXTRA\
		free(d);\
	}

#define OBJECT_DESTROY(TYPE,EXTRA) \
	OBJECT_DESTROY_NOBUF(TYPE,\
		EXTRA\
		for (unsigned i = 0; i <= d->_buf_len; i++) {\
			free(d->_buf[i]);\
		}\
		free(d->_buf);\
	)

#define OBJECT_INIT_DESTROY(TYPE) \
	OBJECT_INIT_MEMSET(TYPE)\
	OBJECT_DESTROY(TYPE,\
		free(d->table);\
	)

#define OBJECT_CLEAR(d,EXTRA) \
	while(d->_buf_len) free(d->_buf[d->_buf_len--]);\
	d->_b = d->_buf[0];\
	d->_b_end = d->_b + d->_alloc - sizeof(*d) - 2;\
	d->len = 0;\
	EXTRA

#define OBJECT_INSERT_CHECK(ADD_LEN,OBJ_SIZE) \
	OBJECT_SIZE_CHECK\
	OBJECT_BUFFER_CHECK(ADD_LEN,OBJ_SIZE)

#define OBJECT_SIZE_CHECK \
	if (d->len >= d->_size) {\
		return RETVAL_MAX_SIZE_REACHED;\
	}

#define OBJECT_BUFFER_CHECK(ADD_LEN,OBJ_SIZE) \
	if (ADD_LEN + d->_b >= d->_b_end) {\
		if (d->_buf_len >= MAX_BUF_ALLOC)\
			return RETVAL_MAX_BUF_ALLOC_REACHED;\
		d->_b = d->_buf[++d->_buf_len] = malloc(d->_alloc);\
		d->_b_end = d->_b + d->_alloc - (OBJ_SIZE);\
		if (!d->_b) return RETVAL_NO_MEMORY;\
		debug("buffer grew [%d]: %d", d->_buf_len, d->_alloc);\
	}

#define OBJECT_UPDATE_RETURN(TYPE,PTYPE,INNER) \
	TYPE##Item e;\
	HASH_##PTYPE(key, d->_size);\
	for(e = d->table[h]; e != 0; e = e->next) {\
		if(MATCH_##PTYPE(e)) break;\
	}\
	if (!e) {\
		OBJECT_SIZE_CHECK;\
		e = (TYPE##Item) d->_b;\
		d->_b += sizeof(*e);\
		e->prev = d->last;\
		d->last = e;\
		d->len++;\
	}\
	INNER\
	e->next = d->table[h];\
	d->table[h] = e;\
	return RETVAL_OK;

#define OBJECT_INSERT_RETURN(TYPE,PTYPE,INNER) \
	TYPE##Item e = (TYPE##Item) d->_b;\
	d->_b += sizeof(*e);\
	HASH_##PTYPE(key, d->_size);\
	INNER\
	e->next = d->table[h];\
	e->prev = d->last;\
	d->last = e;\
	d->table[h] = e;\
	d->len++;\
	return RETVAL_OK;

#define OBJECT_RESET(d) \
	OBJECT_CLEAR(d,\
		d->last = 0;\
		memset(d->table, 0, sizeof(void*) * d->_size);\
	);

#define MATCH_STR(X) X->key_len == key_len && memcmp(X->key, key, key_len) == 0
#define MATCH_UNI MATCH_STR
#define MATCH_INT(X) X->key == key

#define FN_SEARCH(TYPE, PTYPE, RET_TYPE, RET_VAL) \
	RET_TYPE TYPE##Search(TYPE d, PARAM_##PTYPE(key)) {\
		TYPE##Item e;\
		HASH_##PTYPE(key, d->_size);\
		for(e = d->table[h]; e != 0; e = e->next) {\
			if(MATCH_##PTYPE(e)) {\
				return RET_VAL;\
			}\
		}\
		return 0;\
	}

#define FN_DELETE(TYPE, PTYPE) \
	int TYPE##Delete(TYPE d, PARAM_##PTYPE(key)) {\
	TYPE##Item *prev, e;\
	HASH_##PTYPE(key, d->_size);\
	for (prev = &(d->table[h]); *prev != 0; prev = &((*prev)->next)) {\
		if(MATCH_##PTYPE((*prev))) {\
			e = *prev;\
			*prev = e->next;\
			e = 0;\
			return RETVAL_FOUND;\
		}\
	}\
	return 0;\
}

#endif
