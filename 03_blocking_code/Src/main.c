#include "led.h"
#include "uart.h"
#include "timebase.h"

void motor_run(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);

int main(void)
{
	led_init();
	uart_tx_init();
	timebase_init();

	while(1)
	{
		delay(1);
		printf("A second just occurred.\n\r");
		// printf("Hello from STM32F4.........\n\r");

	}
}

void motor_run(void)
{
	printf("motor is starting...\n\r");
}

void motor_stop(void)
{
	printf("motor is stopping...\n\r");
}

void valve_open(void)
{
	printf("Valve is opening...\n\r");
}

void valve_close(void)
{
	printf("Valve is closing...\n\r");
}
