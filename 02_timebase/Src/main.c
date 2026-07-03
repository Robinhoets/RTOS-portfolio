//#include "led.h"
//#include "uart.h"
//
//
//
//
//int main(void)
//{
//	led_init();
//	uart_tx_init();
//
//	while(1)
//	{
//		printf("Hello from STM32F4.........\n\r");
//
//	}
//}
#include "led.h"
#include "uart.h"
#include <stdio.h>



int main(void)
{
	led_init();
	uart_tx_init();

	while(1)
	{
//			printf("Hello from STM32F4.........\n\r");
//
//			for (volatile int i = 0; i < 1000000; i++);
        led_on();
        printf("Hello from STM32F4\r\n");

        for (volatile int i = 0; i < 1000000; i++);

        led_off();

        for (volatile int i = 0; i < 1000000; i++);
	}
}
