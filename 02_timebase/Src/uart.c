#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "stm32f4xx.h"

#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE	115200

#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<13)

#define SR_TXE			(1U<<7)


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch);


int __io_putchar(int ch)
{
	uart_write(ch);
	return ch;
}

void uart_tx_init(void)
{
	/*Enable clock access to GPIOA*/
	 RCC->AHB1ENR  |= GPIOAEN;

	/*Set PA2 mode to alternate function mode*/
	 GPIOA->MODER &=~(1U<<4);
	 GPIOA->MODER |=(1U<<5);

	/*Set alternate function type to AF7 (UART2_TX)*/
	 GPIOA->AFR[0] |=(1U<<8);
	 GPIOA->AFR[0] |=(1U<<9);
	 GPIOA->AFR[0] |=(1U<<10);
	 GPIOA->AFR[0] &= ~(1U<<11);

	/*Enable clock access to UART*/
	 RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	 uart_set_baudrate(APB1_CLK,UART_BAUDRATE);

	/*Configure transfer direction*/
	 USART2->CR1 = CR1_TE;

	/*Enable UART module*/
	 USART2->CR1 |= CR1_UE;
}

static void uart_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	 while(!(USART2->SR & SR_TXE)){}

	/*Write to transmit data register*/
	 USART2->DR =  (ch & 0XFF);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk,baudrate);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}


//#include <stdio.h>
//#include <stdint.h>
//#include "uart.h"
//#include "stm32f4xx.h"
//
//#define GPIOAEN			(1U<<0)
//#define UART2EN			(1U<<17)
//
//#define SYS_FREQ		16000000	// 16 Mhz. Default for Nucleo
//#define APB1_CLK		SYS_FREQ
//#define UART_BAUDRATE	115200		// standard rate
//
//#define CR1_TE			(1U<<3)
//#define CR1_UE			(1U<<13)
//
//#define SR_TXE			(1U<<7)
//
//static uint16_t compute_uart_db(uint32_t periph_clk, uint32_t baudrate);
//static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
//static void uart_write(int ch);
//
///*
// * 	Allow us to use printf
// */
//int __io_putchar(int ch)
//{
//	uart_write(ch);
//	return ch;
//}
//
///*
// *	Notes:
// *		USART: Universal Synchronous Asynchronous Transmitter
// *
// *		We need to enable clock access to UART to use it. (Asynchronous option).
// *		That's why on the block diagram we find USART2 to work with.
// *		Pick USART2 because RX and TX are connected to the line we plug into the computer.
// *		As opposed to USART1 that requires a different cord.
// *		USART2 is connected to APB1.
// *
// *		USART2 configure RX and TX as AF (alternate function).
// *
// *
// * 		rx line on GPIO pins for receiving.
// * 		tx lines on GPIO pins for transmitting.
// *
// * 		We find this information in stm32f411ce.pdf in the block diagram.
// *
// * 		Because we are doing alternate function, we look at Table 9. for Alternate function mapping.
// * 		Identify that PA2 and PA3 map to USART2_TX AND USART2_RX under AF07.
// * 		That is, set PA2->AF07 to get USART2_TX functionality. etc.
// *
// * 	Purpose:
// * 		Allows us to see calls to the kernel such as a thread executing.
// *
// */
//void uart_tx_init(void)
//{
//	/* 	Configure UART:
//	 *  	1) Enable clock access to GPIOA.
//	 * 		2) Set PA2 mode to alternate function mode.
//	 * 			These bits are written by software to configure the I/O direction mode.
//				00: Input (reset state)
//				01: General purpose output mode
//				10: Alternate function mode
//				11: Analog mode
//	 * 		3) Set alternate function type to AF7 (UART2_TX).
//	 * */
//	// (1)
//	RCC->AHB1ENR  |= GPIOAEN;
//
//	// (2) 	PA2 so MODER2
//	// 		Alternate mode so 5->1 and 4->0.
//	GPIOA->MODER &= ~(1U<<4);
//	GPIOA->MODER |=(1U<<5);
//
//	// (3) 	AFRL2 occupies bits 8-11. That is 11 10 9 8.
//	// 		AF7: 0111. That is 11->0, 10->1, 9->1, 8->1
//	GPIOA->AFR[0] |= (1U<<8);
//	GPIOA->AFR[0] |= (1U<<9);
//	GPIOA->AFR[0] |= (1U<<10);
//	GPIOA->AFR[0] &= ~(1U<<11);
//
//	/*
//	 * 	Enable UART itself:
//	 * 		1) Enable clock access to USART.
//	 * 		2) Configure baud rate.
//	 * 		3) Configure transfer direction.
//	 * 		4) Enable the UART module.
//	 */
//	// (1)
//	RCC->APB1ENR |= UART2EN;
//	// (2)
//	uart_set_baudrate(APB1_CLK, UART_BAUDRATE);
//	// (3) Not using "or" so we can clear everything in this register.
//	USART2->CR1 = CR1_TE;
//	// (4) Use or so we can add this to what we created above.
//	USART2->CR1 |= CR1_UE;
//
//
//}
//
//static void uart_write(int ch)
//{
//	/*
//	 * 	1 - Make sure the transmit data register is empty.
//	 * 		- rm0383-stm32f41...pdf find USART_SR status register (19.6.1)
//	 * 		- bit 7 (TXE)
//	 * 		- 0: data is not transfered to the shift register
//	 * 		- 1: data is ""
//	 * 	2 - Write to transmit data register.
//	 * 		- Transmit 8 bits
//	 */
//
//	// (1) 	Check bit in status register.
//	// 		If our bit is not set to one, stay here until it is.
//	while(!(USART2->SR & SR_TXE)){}
//
//	// (2)	If it is set to 1
//	USART2->DR = (ch & 0XFF);
//}
//
//static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
//{
//	USART2->BRR = compute_uart_db(periph_clk, baudrate);
//}
//
///*
// * 	Computer number of symbols transmitted per second
// */
//static uint16_t compute_uart_db(uint32_t periph_clk, uint32_t baudrate)
//{
//	return ((periph_clk + (baudrate/2U))/baudrate);
//}
//
//
//
