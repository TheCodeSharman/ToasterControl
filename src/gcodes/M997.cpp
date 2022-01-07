#include "gcodes/M997.h"

bool M997::match() {
    if ( GCode.HasWord('M') ) {
       return ( (int)GCode.GetWordValue('M') == 997 );
    }
    return false;
}

void M997::execute() {
    HAL_RCC_DeInit();
    HAL_DeInit();
    SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
    __HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();
    __set_MSP(*((uint32_t*) 0x00000000));
    //__ASM volatile ("MSR msp, %0" : "r" (*((uint32_t*) 0x00000000)));
    ((void (*)(void)) *((uint32_t*) 0x00000004))();
}