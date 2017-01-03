var ip = require(".")

console.log([ip.lookupn(0)])
console.log([ip.lookupn(0x10000000)])
console.log([ip.lookupn(0x08080808)])
console.log([ip.lookupn(0x40404040)])
console.log([ip.lookupn(+'210.140.1.1'.split('.').reverse().reduce((x,y,i)=>(y<<i*8)+x,0))])
console.log([ip.lookupn(0xdeffffff)])
console.log([ip.lookupn(0xffffffff)])
console.log([ip.lookupn(Infinity)])
console.log([ip.lookupn(NaN)])
console.log([ip.lookup('210.140.1.1')])
console.log(ip.pton('210.140.1.1'))

console.time("benchmark")
for (var i=0; i < 0xffffff; i++) {
	ip.lookupn(i)
}
console.timeEnd("benchmark")

