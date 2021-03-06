#include "msp.h"
#define SYSTICK_STCSR (*((volatile unsigned long *)0xE000E010))
#define SYSTICK_STRVR (*((volatile unsigned long *)0xE000E014))
#define SYSTICK_STCVR (*((volatile unsigned long *)0xE000E018))
	
char in;

void SysTick_Init(void);
void SysTick_wait(uint32_t delay);
void SysTick_wait1ms(uint32_t delay);
void UART0_init(void);
void UART0_send(char c);

int main(void) {
	__disable_irq();
	UART0_init();
	NVIC_SetPriority(EUSCIA0_IRQn, 1);
	NVIC_EnableIRQ(EUSCIA0_IRQn);
	__enable_irq();
	while (1) {
	}
}

void SysTick_Init(void) {
	SYSTICK_STCSR = 0;
	SYSTICK_STRVR = 0x00FFFFFF;
	SYSTICK_STCVR = 0;
	SYSTICK_STCSR = 0x00000005;
}

void UART0_init(void){
	EUSCI_A0->CTLW0 |= 1;
	EUSCI_A0->MCTLW = 0;
	EUSCI_A0->CTLW0 = 0x0081;
	EUSCI_A0->BRW = 26;
	P1->SEL0 |= 0x0C;
	P1->SEL1 &= ~0x0C;
	EUSCI_A0->CTLW0 &= ~1;
	EUSCI_A0->IE |= 1;
}

void EUSCIA0_IRQHandler(void){
	in = EUSCI_A0->RXBUF;
	UART0_send(in);
}

void SysTick_wait(uint32_t delay) {
	SYSTICK_STRVR = delay - 1;
	SYSTICK_STCVR = 0;
	while ((SYSTICK_STCSR & 0x00010000) == 0) {
	}
}

void SysTick_wait1ms(uint32_t delay) {
	uint32_t i;
	for (i = 0; i < delay; i++) {
		SysTick_wait(3000);
	}
}

void UART0_send(char out) {
	while(!(EUSCI_A0->IFG & 0x02)){}
		EUSCI_A0->TXBUF = out;
}
