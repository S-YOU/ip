// This file is generated
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sc/dict_int_chr.h" //https://github.com/S-YOU/sc
#include "data.h" //generate from MaxMind csv with gen.py

unsigned char *direct; //first 24 bit direct mapping, cover 75% ranges
DictIntChr map; //remaining ranges as hashtable

static inline const char *lookup(unsigned int ip) {
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

//time python gen.py; time clang -O3 sc/dict_int_chr.c lookup.c; time ./a.out
int main() {
	unsigned i;
	double start = clock();

	direct = calloc(0xe00000, 1);

	if (DictIntChrInit(&map, 0x100000, 1)) goto error;

	for (i = 0; i < 0x98a7; i++) {
		direct[r1[i][0]] = r1[i][1];
	}
	for (i = 0; i < 0x19817; i++) {
		memset(direct + rx[i][0], rx[i][1], rx[i][2]);
	}
	for (i = 0; i < 0xd042; i++) {
		DictIntChrInsert(map, maps[i][0], maps[i][1]);
	}

	//calloc/init/memset time: 14.371ms
	printf("calloc/init/memset time: %.3fms\n", (clock() - start) / 1000); start = clock();

	// printf("lookup: [%s]\n",lookup(3743093650));

	char x = 0;
	for (i = 0; i < 0xffffffff; i++) {
		x += *lookup(i);
	}
	direct[0] = x; //to prevent optimization from compiler

	//benchmark time: 8296.153ms
	printf("benchmark time: %.3fms\n", (clock() - start) / 1000); start = clock();

error:
	DictIntChrDestroy(map);
	free(direct);

	//destroy time: 1.375ms
	printf("destroy time: %.3fms\n", (clock() - start) / 1000);
	return 0;
}
