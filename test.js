var ip = require(".")

console.log([ip.lookup(0)])
console.log([ip.lookup(0x10000000)])
console.log([ip.lookup(0x08080808)])
console.log([ip.lookup(0x40404040)])
console.log([ip.lookup(+'210.140.1.1'.split('.').reverse().reduce((x,y,i)=>(y<<i*8)+x,0))])
console.log([ip.lookup(0xdeffffff)])
console.log([ip.lookup(0xffffffff)])
console.log([ip.lookup(Infinity)])
console.log([ip.lookup(NaN)])
console.log(ip.pton('210.140.1.1'))
console.log([ip.lookup('210.140.1.1')])

console.time("benchmark")
for (var i=0; i < 0xffffff; i++) {
	ip.lookup(i)
}
console.timeEnd("benchmark")

