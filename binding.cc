#include <math.h>
#include <string.h>

#include <node.h>
#include <node_buffer.h>
#include <nan.h>

#include "sc/dict_int_chr.h" //https://github.com/S-YOU/sc
#include "data.h"

namespace {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::String;
using v8::Number;
using v8::Value;
using v8::Handle;

unsigned char *direct; //first 24 bit direct mapping, cover 75% ranges
DictIntChr map; //remaining ranges as hashtable

static inline const char *_lookup(unsigned int ip) {
	if (ip < 0x01000000 || ip >= 0xe0000000) return countries[0];

	unsigned char range_ip = direct[ip >> 8];
	if (range_ip < 255) return countries[range_ip]; //75% match here

	//search between bit 32 down to 25, and lookup in hashtable each
	DictIntChrItem e;
	unsigned mask = 0xffffffff;
	while (mask > 0xffffff00) {
		e = DictIntChrSearch(map, ip);
		if (e) {
			return countries[e->val];
		}
		mask <<= 1;
		ip &= mask;
	}

	return countries[0];
}

static inline const unsigned char _lookupn(unsigned int ip) {
	if (ip < 0x01000000 || ip >= 0xe0000000) return 0;

	unsigned char range_ip = direct[ip >> 8];
	if (range_ip < 255) return range_ip; //75% match here

	//search between bit 32 down to 25, and lookup in hashtable each
	DictIntChrItem e;
	unsigned mask = 0xffffffff;
	while (mask > 0xffffff00) {
		e = DictIntChrSearch(map, ip);
		if (e) {
			return e->val;
		}
		mask <<= 1;
		ip &= mask;
	}

	return 0;
}


#define ATOI(BUF, BUF_END, NUM) \
	NUM = 0, mul = 1, sTmp = BUF_END - 1;\
	while (sTmp >= BUF) {\
		NUM += (*sTmp - '0') * mul;\
		mul *= 10;\
		sTmp--;\
	}

static inline unsigned _pton(char *a) {
	unsigned mul, num, ret; char *s = a, *sTmp;

	while (*s >= '0' && *s <= '9') s++;
	if (s <= a || *s != '.') return 0;
	ATOI(a, s, num); ret = num << 24; s++; a = s;
	while (*s >= '0' && *s <= '9') s++;
	if (s <= a || *s != '.') return 0;
	ATOI(a, s, num); ret += num << 16; s++; a = s;
	while (*s >= '0' && *s <= '9') s++;
	if (s <= a || *s != '.') return 0;
	ATOI(a, s, num); ret += num << 8; s++; a = s;
	while (*s >= '0' && *s <= '9') s++;
	if (s <= a) return 0;
	ATOI(a, s, num); ret += num;

	return ret;
}

void pton(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	String::Utf8Value str(args[0]->ToObject());
	char *ip = *str;

	const char* err_msg = nullptr;
	if (args.Length() != 1) {
		err_msg = "Wrong number of arguments: expected 1";
		goto err;
	}
	if (!args[0]->IsString()) {
		err_msg = "Input must be String";
		goto err;
	}

	return args.GetReturnValue().Set(Nan::New<Number>(_pton(ip)));

err:
	Nan::ThrowError(Nan::New<String>(err_msg).ToLocalChecked());
}

void lookup(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	unsigned ip = 0;
	const char* err_msg = nullptr;
	if (args.Length() != 1) {
		err_msg = "Wrong number of arguments: expected 1";
		goto err;
	}

	if (args[0]->IsNumber()) {
		ip = args[0]->Uint32Value();

		return args.GetReturnValue().Set(Nan::New<String>(_lookup(ip)).ToLocalChecked());
	} else if (args[0]->IsString()) {
		String::Utf8Value str(args[0]->ToObject());
		char *ip = *str;

		return args.GetReturnValue().Set(Nan::New<String>(_lookup(_pton(ip))).ToLocalChecked());
	} else {
		err_msg = "Input must be Number or String";
		goto err;
	}

err:
	Nan::ThrowError(Nan::New<String>(err_msg).ToLocalChecked());
}

void lookupn(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	unsigned ip = 0;
	const char* err_msg = nullptr;
	if (args.Length() != 1) {
		err_msg = "Wrong number of arguments: expected 1";
		goto err;
	}

	if (args[0]->IsNumber()) {
		ip = args[0]->Uint32Value();

		return args.GetReturnValue().Set(Nan::New<Number>(_lookupn(ip)));
	} else if (args[0]->IsString()) {
		String::Utf8Value str(args[0]->ToObject());
		char *ip = *str;

		return args.GetReturnValue().Set(Nan::New<Number>(_lookupn(_pton(ip))));
	} else {
		err_msg = "Input must be Number or String";
		goto err;
	}

err:
	Nan::ThrowError(Nan::New<String>(err_msg).ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
	unsigned i;

	direct = (unsigned char *) calloc(0xe00000, 1);

	if (DictIntChrInit(&map, 0x100000, 1)) return;

	for (i = 0; i < 0xa2cc; i++) {
		direct[r1[i][0]] = r1[i][1];
	}
	for (i = 0; i < 0x1c4ae; i++) {
		memset(direct + rx[i][0], rx[i][1], rx[i][2]);
	}
	for (i = 0; i < 0x82bf; i++) {
		DictIntChrInsert(map, maps[i][0], maps[i][1]);
	}

	v8::Local<v8::Function> ptonFn = Nan::GetFunction(
		Nan::New<v8::FunctionTemplate>(pton)).ToLocalChecked();

	v8::Local<v8::Function> lookupFn = Nan::GetFunction(
		Nan::New<v8::FunctionTemplate>(lookup)).ToLocalChecked();

	v8::Local<v8::Function> lookupnFn = Nan::GetFunction(
		Nan::New<v8::FunctionTemplate>(lookupn)).ToLocalChecked();

	Nan::Set(target, Nan::New("pton").ToLocalChecked(), ptonFn);
	Nan::Set(target, Nan::New("lookup").ToLocalChecked(), lookupFn);
	Nan::Set(target, Nan::New("lookupn").ToLocalChecked(), lookupnFn);
}

NODE_MODULE(addon, Init)

}
