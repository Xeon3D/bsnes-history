alwaysinline void op_io();
alwaysinline uint8 op_read(unsigned addr);
alwaysinline void op_write(unsigned addr, uint8 data);

uint8 mmc_read(unsigned addr);
void mmc_write(unsigned addr, uint8 data);

uint8 mmc_cpu_read(unsigned addr);
void mmc_cpu_write(unsigned addr, uint8 data);

uint8 mmc_sa1_read(unsigned addr);
void mmc_sa1_write(unsigned addr, uint8 data);
