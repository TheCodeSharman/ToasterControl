#include "gcodes/M997.h"

bool M997::match() {
    if ( GCode.HasWord('M') ) {
       return ( (int)GCode.GetWordValue('M') == 997 );
    }
    return false;
}

/* See reference docs:
     https://www.st.com/resource/en/application_note/cd00167594-stm32-microcontroller-system-memory-boot-mode-stmicroelectronics.pdf

    In addition to patterns described above, user can execute bootloader by performing a jump
    to system memory from user code. Before jumping to bootloader user must:
    • Disable all peripheral clocks
    • Disable used PLL
    • Disable interrupts
    • Clear pending interrupts

*/
void M997::execute() {
    
    // Deinitialize the STM32 - resets PLL and disables interrupts
    HAL_RCC_DeInit();
    HAL_DeInit();
    
    // Shutdown the SysTick peripheral clock - oddly HAL_DeInit()
    // isn't enough??
    SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

    // System bootloader is in internal ROM this remaps the
    // memory to page in the ROM.
    __HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

    // We want to do the following:
    //
    // __set_MSP(*((uint32_t*) 0x00000000));
    //
    // __set_MSP() is supposed to inline the MSR instruction as in:
    //
    //    __ASM volatile ("MSR msp, %0" : : "r" (topOfStack) : );
    //
    // but for some reason GCC messes up this inline assembly using extended asm 
    // syntax with the stack address as an input operand.
    //
    // The work around is to avoid using input register parameters and load the
    // new top of stack from address 0 directly in assembly:
    __ASM volatile ("movs r3, #0\nldr r3, [r3, #0]\nMSR msp, r3\n" : : : "r3" );

    // Jump to the address at 0x00000004
    ((void (*)(void)) *((uint32_t*) 0x00000004))();
}