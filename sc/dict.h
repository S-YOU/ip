#ifndef DICT_H_
#define DICT_H_

#include "object.h"

#define DICT_INTF(TYPE, PTYPE1, PTYPE2) \
	OBJECT_STRUCT(TYPE,\
		STRUCT_##PTYPE1(key)\
		STRUCT_##PTYPE2(val)\
	)\
	int TYPE##Insert(TYPE, PARAM_##PTYPE1(key), PARAM_##PTYPE2(val));\
	int TYPE##Update(TYPE, PARAM_##PTYPE1(key), PARAM_##PTYPE2(val));\
	int TYPE##Delete(TYPE, PARAM_##PTYPE1(key));\
	TYPE##Item TYPE##Search(TYPE, PARAM_##PTYPE1(key));

#define DICT_IMPL(TYPE, PTYPE1, PTYPE2) \
	OBJECT_INIT_DESTROY(TYPE)\
	FN_SEARCH(TYPE, PTYPE1, TYPE##Item, e)\
	FN_DELETE(TYPE, PTYPE1)\
	int TYPE##Insert(TYPE d, PARAM_##PTYPE1(key), PARAM_##PTYPE2(val)) {\
		OBJECT_INSERT_CHECK(SIZEOF_##PTYPE1(key) + SIZEOF_##PTYPE2(val), sizeof(TYPE##Item_s) + 2)\
		OBJECT_INSERT_RETURN(TYPE,PTYPE1,\
			ATTR_COPY_##PTYPE1(key);\
			ATTR_COPY_##PTYPE2(val);\
		)\
	}\
	int TYPE##Update(TYPE d, PARAM_##PTYPE1(key), PARAM_##PTYPE2(val)) {\
		OBJECT_BUFFER_CHECK(SIZEOF_##PTYPE1(key) + SIZEOF_##PTYPE2(val), sizeof(TYPE##Item_s) + 2)\
		OBJECT_UPDATE_RETURN(TYPE,PTYPE1,\
			ATTR_COPY_##PTYPE1(key);\
			ATTR_COPY_##PTYPE2(val);\
		)\
	}

#endif
