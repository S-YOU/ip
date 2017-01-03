## IP to Country Code Converter

#### Performance
500 Million IPs to Country per seconds (All IPv4 ranges in 7.8 seconds)

```shell
% time python gen.py; time make; time ./a.out
ranges 190000 and file written
python gen.py  0.59s user 0.05s system 97% cpu 0.649 total
clang -Wall -Wextra -O3 -std=c99 -Ofast -fPIC  sc/dict_int_chr.c ./*.c -o ./a.out
make  1.21s user 0.27s system 90% cpu 1.633 total
calloc/init/memset time: 13.365ms
benchmark time: 7844.078ms
destroy time: 1.383ms
./a.out  7.84s user 0.02s system 99% cpu 7.903 total
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

