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
    // arm-none-eabi-gcc 4.9.0 does not correctly inline this
    // MSP function, so we write it out explicitly here.
    //__set_MSP(*((uint32_t*) 0x00000000));
    #pragma GCC diagnostic ignored "-Wdeprecated"
    __ASM volatile ("movs r3, #0\nldr r3, [r3, #0]\nMSR msp, r3\n" : : : "r3", "sp");
    ((void (*)(void)) *((uint32_t*) 0x00000004))();
    #pragma GCC diagnostic pop
}