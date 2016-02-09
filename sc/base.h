#ifndef HASH_H_
#define HASH_H_

#define MAX_BUF_ALLOC 1000

#define RETVAL_OK 0
#define RETVAL_FOUND 1
#define RETVAL_NO_MEMORY (-1)
#define RETVAL_MAX_SIZE_REACHED (-2)
#define RETVAL_MAX_BUF_ALLOC_REACHED (-3)

#define CHR char
#define U8 unsigned char
#define UNICHR unsigned short
#define NUM unsigned
#define SIZE unsigned
#define FLOAT float

#define STRUCT_STR(NAME) \
	CHR *NAME;\
	SIZE NAME##_len;
#define STRUCT_UNI(NAME) \
	UNICHR *NAME;\
	SIZE NAME##_len;
#define STRUCT_INT(NAME) \
	NUM NAME;
#define STRUCT_U8(NAME) \
	U8 NAME;
#define STRUCT_FLOAT(NAME) \
	FLOAT NAME;

#define PARAM_STR(NAME) \
	const char *NAME, unsigned NAME##_len
#define PARAM_UNI(NAME) \
	const UNICHR *NAME, unsigned NAME##_len
#define PARAM_INT(NAME) \
	NUM NAME
#define PARAM_U8(NAME) \
	U8 NAME
#define PARAM_FLOAT(NAME) \
	FLOAT NAME

#define SIZEOF_STR(X) X##_len
#define SIZEOF_UNI(X) X##_len
#define SIZEOF_INT(_) sizeof(NUM)
#define SIZEOF_U8(_) 1
#define SIZEOF_FLOAT(_) sizeof(FLOAT)

typedef enum {
	R_U8 = 1,
	R_U16,
	R_U32,
	R_U64,

	R_I8,
	R_I16,
	R_I32,
	R_I64,

	// R_F16, //
	R_F32,
	R_F64,
	// R_F128,

	R_STR, //str, byte
	// R_STR16,
	// R_STR32,
	R_UNI, //Py_Unicode
} REG_TYPE;

typedef enum {
	E_SET 	= 1 << 16,
	E_DICT 	= 2 << 16,
	E_DEQUE = 3 << 16,
	E_ARRAY = 4 << 16,
} DATA_TYPE;

#endif
