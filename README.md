## IP to Country Code Converter

#### Performance
500 Million IPs to Country per seconds (All IPv4 ranges in 8.5 seconds)

```shell
% time python gen.py; time make; time ./a.out
ranges 186147 and file written
python gen.py  0.59s user 0.05s system 99% cpu 0.645 total
clang -Wall -Wextra -O3 -std=c99 -Ofast -fPIC  sc/dict_int_chr.c ./*.c -o ./a.out
make  0.93s user 0.07s system 98% cpu 1.014 total
calloc/init/memset time: 14.722ms
benchmark time: 8510.383ms
destroy time: 1.536ms
./a.out  8.51s user 0.02s system 99% cpu 8.548 total
```

#### Requirements
To Generate
- MaxMind GeoLite2 Country CSV - http://dev.maxmind.com/geoip/geoip2/geolite2/
- My Regex to C compiler (can replace with built-in regex) - https://github.com/S-YOU/rc

To Compile
- Some of My Language Implementations (for HashMap) - https://github.com/S-YOU/sc

#### node addon install 
```npm install kuni```

#### Usage 
```
var kuni = require('kuni');
kuni.lookup(0x12345678);
```
