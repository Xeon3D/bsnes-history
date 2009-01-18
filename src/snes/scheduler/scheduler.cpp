#ifdef SNES_CPP

Scheduler scheduler;

void threadentry_cpu() { cpu.enter(); }
void threadentry_smp() { smp.enter(); }
void threadentry_ppu() { ppu.enter(); }
void threadentry_dsp() { dsp.enter(); }

void Scheduler::enter() {
  switch(clock.active) {
    case THREAD_CPU: co_switch(thread_cpu); break;
    case THREAD_SMP: co_switch(thread_smp); break;
    case THREAD_PPU: co_switch(thread_ppu); break;
    case THREAD_DSP: co_switch(thread_dsp); break;
  }
}

void Scheduler::exit() {
  co_switch(thread_snes);
}

void Scheduler::init() {
  clock.cpu_freq = snes.region() == SNES::NTSC
                 ? snes.config.cpu.ntsc_clock_rate
                 : snes.config.cpu.pal_clock_rate;
  clock.smp_freq = snes.region() == SNES::NTSC
                 ? snes.config.smp.ntsc_clock_rate
                 : snes.config.smp.pal_clock_rate;

  clock.active = THREAD_CPU;
  clock.cpuppu = 0;
  clock.cpusmp = 0;
  clock.smpdsp = 0;

  if(thread_cpu) co_delete(thread_cpu);
  if(thread_smp) co_delete(thread_smp);
  if(thread_ppu) co_delete(thread_ppu);
  if(thread_dsp) co_delete(thread_dsp);

  thread_snes = co_active();
  thread_cpu  = co_create(65536 * sizeof(void*), threadentry_cpu);
  thread_smp  = co_create(65536 * sizeof(void*), threadentry_smp);
  thread_ppu  = co_create(65536 * sizeof(void*), threadentry_ppu);
  thread_dsp  = co_create(65536 * sizeof(void*), threadentry_dsp);
}

Scheduler::Scheduler() {
  thread_snes = 0;
  thread_cpu  = 0;
  thread_smp  = 0;
  thread_ppu  = 0;
  thread_dsp  = 0;
}

#endif  //ifdef SNES_CPP
