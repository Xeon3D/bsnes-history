#define g65816_flags_ora_b() g65816_testn(gx816->regs.a.b & 0x80);   g65816_testz(gx816->regs.a.b == 0)
#define g65816_flags_ora_w() g65816_testn(gx816->regs.a.w & 0x8000); g65816_testz(gx816->regs.a.w == 0)

#define g65816_inst_ora_b() gx816->regs.a.b |= gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr)
#define g65816_inst_ora_w() gx816->regs.a.w |= gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr)

void g65816_op_ora_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_ora_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_ora_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ora_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ora_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_idpb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDP);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_idpw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDP);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_ildpb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDP);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_ildpw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDP);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_longb(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONG);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_ora_longw(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONG);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_ora_longxb(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONGX);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_ora_longxw(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONGX);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_ora_addryb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ora_addryw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ora_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_ora_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_ora_idpxb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPX);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_ora_idpxw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPX);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_ora_idpyb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPY);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2 | TIMING_CONDITION4);
}

void g65816_op_ora_idpyw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPY);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2 | TIMING_CONDITION4);
}

void g65816_op_ora_ildpyb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDPY);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_ildpyw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDPY);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ora_srb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_SR);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_ora_srw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_SR);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_ora_isryb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_ISRY);
  g65816_inst_ora_b();
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_ora_isryw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_ISRY);
  g65816_inst_ora_w();
  g65816_flags_ora_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_ora_constb(void) {
g65816_prefetch(1);
  gx816->regs.a.b |= arg;
  g65816_flags_ora_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
}

void g65816_op_ora_constw(void) {
g65816_prefetch(2);
  gx816->regs.a.w |= arg;
  g65816_flags_ora_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
}