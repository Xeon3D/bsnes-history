//===============
//opcode_move.bpp
//===============


























//=============
//opcode_pc.bpp
//=============















//===============
//opcode_read.bpp
//===============


















//==============
//opcode_rmw.bpp
//==============







//===============
//opcode_misc.bpp
//===============














//

//===============
//opcode_move.bpp
//===============

void op_mov_a_x();

void op_mov_a_y();

void op_mov_x_a();

void op_mov_y_a();

void op_mov_x_sp();

void op_mov_sp_x();


void op_mov_a_const();

void op_mov_x_const();

void op_mov_y_const();


void op_mov_a_ix();

void op_mov_a_ixinc();


void op_mov_a_dp();

void op_mov_x_dp();

void op_mov_y_dp();


void op_mov_a_dpx();

void op_mov_x_dpy();

void op_mov_y_dpx();


void op_mov_a_addr();

void op_mov_x_addr();

void op_mov_y_addr();


void op_mov_a_addrx();

void op_mov_a_addry();


void op_mov_a_idpx();

void op_mov_a_idpy();

void op_mov_dp_dp();

void op_mov_dp_const();

void op_mov_ix_a();

void op_mov_ixinc_a();


void op_mov_dp_a();

void op_mov_dp_x();

void op_mov_dp_y();


void op_mov_dpx_a();

void op_mov_dpy_x();

void op_mov_dpx_y();


void op_mov_addr_a();

void op_mov_addr_x();

void op_mov_addr_y();


void op_mov_addrx_a();

void op_mov_addry_a();


void op_mov_idpx_a();

void op_mov_idpy_a();


void op_movw_ya_dp();

void op_movw_dp_ya();


void op_mov1_c_bit();

void op_mov1_bit_c();


//=============
//opcode_pc.bpp
//=============

void op_bra();

void op_beq();

void op_bne();

void op_bcs();

void op_bcc();

void op_bvs();

void op_bvc();

void op_bmi();

void op_bpl();


void op_bbs0();

void op_bbc0();

void op_bbs1();

void op_bbc1();

void op_bbs2();

void op_bbc2();

void op_bbs3();

void op_bbc3();

void op_bbs4();

void op_bbc4();

void op_bbs5();

void op_bbc5();

void op_bbs6();

void op_bbc6();

void op_bbs7();

void op_bbc7();


void op_cbne_dp();

void op_cbne_dpx();

void op_dbnz_dp();

void op_dbnz_y();

void op_jmp_addr();

void op_jmp_iaddrx();

void op_call();

void op_pcall();


void op_tcall_0();

void op_tcall_1();

void op_tcall_2();

void op_tcall_3();

void op_tcall_4();

void op_tcall_5();

void op_tcall_6();

void op_tcall_7();

void op_tcall_8();

void op_tcall_9();

void op_tcall_10();

void op_tcall_11();

void op_tcall_12();

void op_tcall_13();

void op_tcall_14();

void op_tcall_15();


void op_brk();

void op_ret();

void op_reti();


//===============
//opcode_read.bpp
//===============

void op_adc_a_const();

void op_and_a_const();

void op_cmp_a_const();

void op_cmp_x_const();

void op_cmp_y_const();

void op_eor_a_const();

void op_or_a_const();

void op_sbc_a_const();


void op_adc_a_ix();

void op_and_a_ix();

void op_cmp_a_ix();

void op_eor_a_ix();

void op_or_a_ix();

void op_sbc_a_ix();


void op_adc_a_dp();

void op_and_a_dp();

void op_cmp_a_dp();

void op_cmp_x_dp();

void op_cmp_y_dp();

void op_eor_a_dp();

void op_or_a_dp();

void op_sbc_a_dp();


void op_adc_a_dpx();

void op_and_a_dpx();

void op_cmp_a_dpx();

void op_eor_a_dpx();

void op_or_a_dpx();

void op_sbc_a_dpx();


void op_adc_a_addr();

void op_and_a_addr();

void op_cmp_a_addr();

void op_cmp_x_addr();

void op_cmp_y_addr();

void op_eor_a_addr();

void op_or_a_addr();

void op_sbc_a_addr();


void op_adc_a_addrx();

void op_adc_a_addry();

void op_and_a_addrx();

void op_and_a_addry();

void op_cmp_a_addrx();

void op_cmp_a_addry();

void op_eor_a_addrx();

void op_eor_a_addry();

void op_or_a_addrx();

void op_or_a_addry();

void op_sbc_a_addrx();

void op_sbc_a_addry();


void op_adc_a_idpx();

void op_and_a_idpx();

void op_cmp_a_idpx();

void op_eor_a_idpx();

void op_or_a_idpx();

void op_sbc_a_idpx();


void op_adc_a_idpy();

void op_and_a_idpy();

void op_cmp_a_idpy();

void op_eor_a_idpy();

void op_or_a_idpy();

void op_sbc_a_idpy();


void op_adc_ix_iy();

void op_and_ix_iy();

void op_cmp_ix_iy();

void op_eor_ix_iy();

void op_or_ix_iy();

void op_sbc_ix_iy();


void op_adc_dp_dp();

void op_and_dp_dp();

void op_cmp_dp_dp();

void op_eor_dp_dp();

void op_or_dp_dp();

void op_sbc_dp_dp();


void op_adc_dp_const();

void op_and_dp_const();

void op_cmp_dp_const();

void op_eor_dp_const();

void op_or_dp_const();

void op_sbc_dp_const();


void op_addw_ya_dp();

void op_subw_ya_dp();

void op_cmpw_ya_dp();


void op_and1_bit();

void op_and1_notbit();

void op_eor1_bit();

void op_not1_bit();

void op_or1_bit();

void op_or1_notbit();


//==============
//opcode_rmw.bpp
//==============

void op_inc_a();

void op_inc_x();

void op_inc_y();

void op_dec_a();

void op_dec_x();

void op_dec_y();

void op_asl_a();

void op_lsr_a();

void op_rol_a();

void op_ror_a();


void op_inc_dp();

void op_dec_dp();

void op_asl_dp();

void op_lsr_dp();

void op_rol_dp();

void op_ror_dp();


void op_inc_dpx();

void op_dec_dpx();

void op_asl_dpx();

void op_lsr_dpx();

void op_rol_dpx();

void op_ror_dpx();


void op_inc_addr();

void op_dec_addr();

void op_asl_addr();

void op_lsr_addr();

void op_rol_addr();

void op_ror_addr();


void op_tset_addr_a();

void op_tclr_addr_a();


void op_incw_dp();

void op_decw_dp();


//===============
//opcode_misc.bpp
//===============

void op_nop();

void op_sleep();

void op_stop();

void op_xcn();

void op_daa();

void op_das();


void op_clrc();

void op_clrp();

void op_setc();

void op_setp();

void op_clrv();

void op_notc();


void op_ei();

void op_di();


void op_set0_dp();

void op_clr0_dp();

void op_set1_dp();

void op_clr1_dp();

void op_set2_dp();

void op_clr2_dp();

void op_set3_dp();

void op_clr3_dp();

void op_set4_dp();

void op_clr4_dp();

void op_set5_dp();

void op_clr5_dp();

void op_set6_dp();

void op_clr6_dp();

void op_set7_dp();

void op_clr7_dp();


void op_push_a();

void op_push_x();

void op_push_y();

void op_push_p();


void op_pop_a();

void op_pop_x();

void op_pop_y();

void op_pop_p();


void op_mul_ya();

void op_div_ya_x();



