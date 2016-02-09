#!/usr/bin/env python

import rc_cidrx # https://github.com/S-YOU/rc or use regex findall "\n(\d+)\.(\d+)\.(\d+)\.(\d+)/(\d+),(\d*),(\d*)"

# download - http://geolite.maxmind.com/download/geoip/database/GeoLite2-Country-CSV.zip
# extract into ./data/

loc_lines = open("data/GeoLite2-Country-Locations-en.csv", "rb").read().rstrip().split("\n")
loc_map = {0: 0}
countries = [""]
i = 1
for x in loc_lines[1:]:
	v = x.split(",", 5)
	k = int(v[0])
	if k in loc_map:
		print k, "exists in loc_map", loc_map[k], i
	loc_map[k] = i
	countries.append(v[4] or v[2])
	i += 1

# print loc_map, max(map(len, loc_lines))
# print countries, len(countries), len(set(countries))

d = open("data/GeoLite2-Country-Blocks-IPv4.csv", "rb").read()

ips = rc_cidrx.findall(d, 190000)

header_data = """// This file is generated
#ifndef DATA_H_
#define DATA_H_

"""

header1 = """// This file is generated
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sc/dict_int_chr.h" //https://github.com/S-YOU/sc
#include "data.h" //generate from MaxMind csv with gen.py

unsigned char *direct; //first 24 bit direct mapping, cover 75% ranges
DictIntChr map; //remaining ranges as hashtable
"""

header2 = """
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
"""

body = """
	direct = calloc(0x%x, 1);

	if (DictIntChrInit(&map, 0x%x, 1)) goto error;

	for (i = 0; i < 0x%x; i++) {
		direct[r1[i][0]] = r1[i][1];
	}
	for (i = 0; i < 0x%x; i++) {
		memset(direct + rx[i][0], rx[i][1], rx[i][2]);
	}
	for (i = 0; i < 0x%x; i++) {
		DictIntChrInsert(map, maps[i][0], maps[i][1]);
	}
"""

footer = """
	//calloc/init/memset time: 14.371ms
	printf("calloc/init/memset time: %.3fms\\n", (clock() - start) / 1000); start = clock();

	// printf("lookup: [%s]\\n",lookup(3743093650));

	char x = 0;
	for (i = 0; i < 0xffffffff; i++) {
		x += *lookup(i);
	}
	direct[0] = x; //to prevent optimization from compiler

	//benchmark time: 8296.153ms
	printf("benchmark time: %.3fms\\n", (clock() - start) / 1000); start = clock();

error:
	DictIntChrDestroy(map);
	free(direct);

	//destroy time: 1.375ms
	printf("destroy time: %.3fms\\n", (clock() - start) / 1000);
	return 0;
}
"""

r1, rx, maps = [], [], []
dup = set()
for o1, o2, o3, o4, bit, cid1, cid2 in ips:
	cid = cid2 or cid1
	o123 = (o1 << 16) + (o2 << 8) + o3

	if bit <= 23:
		rx.append((o123, loc_map[cid], 1 << (24 - bit)))
	elif bit == 24:
		r1.append((o123, loc_map[cid]))
	else:
		if o123 not in dup:
			r1.append((o123, 255))
			dup.add(o123)
		oi = (o123 << 8) + o4
		maps.append((oi, loc_map[cid]))

def chunks(l, n): # http://stackoverflow.com/q/312443
	for i in xrange(0, len(l), n):
		yield l[i:i + n]

with open("data.h", "wb") as f:
	f.write(header_data)
	f.write('const char countries[][3] = {%s};\n' % (','.join('"%s"' % x.lower() for x in countries)))
	f.write('const unsigned r1[][2] = {%s};\n' % ','.join(','.join('{%d,%d}' % z for z in y) + '\n' for y in chunks(r1, 10)))
	f.write('const unsigned rx[][3] = {%s};\n' % ','.join(','.join('{%d,%d,%d}' % z for z in y) + '\n' for y in chunks(rx, 10)))
	f.write('const unsigned maps[][2] = {%s};\n' % ','.join(','.join('{%d,%d}' % z for z in y) + '\n' for y in chunks(maps, 10)))
	f.write("#endif")

with open("lookup.c", "wb") as f:
	f.write(header1)
	f.write(header2)
	f.write(body % (0xe00000, 0x100000, len(r1), len(rx), len(maps)))
	f.write(footer)

print "ranges", len(ips), "and file written"

