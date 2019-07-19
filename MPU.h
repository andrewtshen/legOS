#ifndef MPU_H
#define MPU_H

struct MPU
{
    volatile unsigned int TYPE;
    volatile unsigned int CTRL;
    volatile unsigned int RNR;
    volatile unsigned int RBAR;
    volatile unsigned int RASR;
    volatile unsigned int RBAR_A1;
    volatile unsigned int RBAR_A2;
    volatile unsigned int RBAR_A3;
    volatile unsigned int RASR_A1;
    volatile unsigned int RASR_A2;
    volatile unsigned int RASR_A3;
};

#define MPU ((struct MPU*) 0xE000ED90)

void MPU_setup();

#endif
