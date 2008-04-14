//nop
case 0x00: {
  op_io();
} break;

//sleep
case 0xef: {
  op_io();
  op_io();
  regs.pc--;
} break;

//stop
case 0xff: {
  op_io();
  op_io();
  regs.pc--;
} break;

//xcn
case 0x9f: {
  op_io();
  op_io();
  op_io();
  op_io();
  regs.a = (regs.a >> 4) | (regs.a << 4);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//daa
case 0xdf: {
  op_io();
  op_io();
  if(regs.p.c || (regs.a) > 0x99) {
    regs.a += 0x60;
    regs.p.c = 1;
  }
  if(regs.p.h || (regs.a & 15) > 0x09) {
    regs.a += 0x06;
  }
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//das
case 0xbe: {
  op_io();
  op_io();
  if(!regs.p.c || (regs.a) > 0x99) {
    regs.a -= 0x60;
    regs.p.c = 0;
  }
  if(!regs.p.h || (regs.a & 15) > 0x09) {
    regs.a -= 0x06;
  }
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//clrc
case 0x60: {
  op_io();
  regs.p.c = 0;
} break;

//clrp
case 0x20: {
  op_io();
  regs.p.p = 0;
} break;

//setc
case 0x80: {
  op_io();
  regs.p.c = 1;
} break;

//setp
case 0x40: {
  op_io();
  regs.p.p = 1;
} break;

//clrv
case 0xe0: {
  op_io();
  regs.p.v = 0;
  regs.p.h = 0;
} break;

//notc
case 0xed: {
  op_io();
  op_io();
  regs.p.c ^= 1;
} break;

//ei
case 0xa0: {
  op_io();
  op_io();
  regs.p.i = 1;
} break;

//di
case 0xc0: {
  op_io();
  op_io();
  regs.p.i = 0;
} break;

//set0_dp
case 0x02: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x01;
  op_writedp(dp, rd);
} break;

//clr0_dp
case 0x12: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x01;
  op_writedp(dp, rd);
} break;

//set1_dp
case 0x22: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x02;
  op_writedp(dp, rd);
} break;

//clr1_dp
case 0x32: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x02;
  op_writedp(dp, rd);
} break;

//set2_dp
case 0x42: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x04;
  op_writedp(dp, rd);
} break;

//clr2_dp
case 0x52: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x04;
  op_writedp(dp, rd);
} break;

//set3_dp
case 0x62: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x08;
  op_writedp(dp, rd);
} break;

//clr3_dp
case 0x72: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x08;
  op_writedp(dp, rd);
} break;

//set4_dp
case 0x82: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x10;
  op_writedp(dp, rd);
} break;

//clr4_dp
case 0x92: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x10;
  op_writedp(dp, rd);
} break;

//set5_dp
case 0xa2: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x20;
  op_writedp(dp, rd);
} break;

//clr5_dp
case 0xb2: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x20;
  op_writedp(dp, rd);
} break;

//set6_dp
case 0xc2: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x40;
  op_writedp(dp, rd);
} break;

//clr6_dp
case 0xd2: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x40;
  op_writedp(dp, rd);
} break;

//set7_dp
case 0xe2: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x80;
  op_writedp(dp, rd);
} break;

//clr7_dp
case 0xf2: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x80;
  op_writedp(dp, rd);
} break;

//push_a
case 0x2d: {
  op_io();
  op_io();
  op_writestack(regs.a);
} break;

//push_x
case 0x4d: {
  op_io();
  op_io();
  op_writestack(regs.x);
} break;

//push_y
case 0x6d: {
  op_io();
  op_io();
  op_writestack(regs.y);
} break;

//push_p
case 0x0d: {
  op_io();
  op_io();
  op_writestack(regs.p);
} break;

//pop_a
case 0xae: {
  op_io();
  op_io();
  regs.a = op_readstack();
} break;

//pop_x
case 0xce: {
  op_io();
  op_io();
  regs.x = op_readstack();
} break;

//pop_y
case 0xee: {
  op_io();
  op_io();
  regs.y = op_readstack();
} break;

//pop_p
case 0x8e: {
  op_io();
  op_io();
  regs.p = op_readstack();
} break;

//mul_ya
case 0xcf: {
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  ya = regs.y * regs.a;
  regs.a = ya;
  regs.y = ya >> 8;
//result is set based on y (high-byte) only
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
} break;

//div_ya_x
case 0x9e: {
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  op_io();
  ya = regs.ya;
//overflow set if quotient >= 256
  regs.p.v = !!(regs.y >= regs.x);
  regs.p.h = !!((regs.y & 15) >= (regs.x & 15));
  if(regs.y < (regs.x << 1)) {
  //if quotient is <= 511 (will fit into 9-bit result)
    regs.a = ya / regs.x;
    regs.y = ya % regs.x;
  } else {
  //otherwise, the quotient won't fit into regs.p.v + regs.a
  //this emulates the odd behavior of the S-SMP in this case
    regs.a = 255    - (ya - (regs.x << 9)) / (256 - regs.x);
    regs.y = regs.x + (ya - (regs.x << 9)) % (256 - regs.x);
  }
//result is set based on a (quotient) only
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

