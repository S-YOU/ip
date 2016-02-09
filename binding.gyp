{
	"targets": [{
		"target_name": "ip",
		"sources": [
			"binding.cc",
			"data.h",
			"sc/dict_int_chr.h",
			"sc/dict_int_chr.c"
		],
		"include_dirs": [
			"<!(node -e \"require('nan')\")"
		],
		"cflags" : ["-std=c99", "-O3", "-fPIC", "-DNDEBUG", "-Wall"],
		"cflags_cc": ["-std=c99", "-O3", "-fPIC", "-DNDEBUG", "-Wall"],
		"conditions": [
		]
	}]
}
