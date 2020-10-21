/*******************************************************************************
This example accompanies the book
"Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
You may use, edit, run or distribute this file
as long as the above copyright notice remains
THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
For more information about my classes, my research, and my books, see
http://users.ece.utexas.edu/~valvano/
 
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "port.h"
#include "tm4c123gh6pm.h"
#include <stddef.h>
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/


/*******************************************************************************
* CONSTANTES
*******************************************************************************/

/*******************************************************************************
* VARIÁVEIS
*******************************************************************************/
/**
*   \brief Definição dos registradores de DATA de cada PORT do uc
*/
static volatile uint32_t * portRegisters[PORT_NUM] = {
  &GPIO_PORTA_DATA_R, &GPIO_PORTB_DATA_R, &GPIO_PORTC_DATA_R,
  &GPIO_PORTE_DATA_R, &GPIO_PORTE_DATA_R, &GPIO_PORTF_DATA_R
};

/**
*   \brief Definição dos registradores de interrupção cada PORT do uc
*/
static volatile uint32_t * portInterruptAcknowledgeRegister[PORT_NUM] = {
  &GPIO_PORTA_ICR_R, &GPIO_PORTB_ICR_R, &GPIO_PORTC_ICR_R,
  &GPIO_PORTD_ICR_R, &GPIO_PORTE_ICR_R, &GPIO_PORTF_ICR_R
};

/**
*   \brief Definição dos registradores de interrupção cada PORT do uc
*/
static volatile uint32_t * portInterruptPinsRegister[PORT_NUM] = {
  &GPIO_PORTA_RIS_R, &GPIO_PORTB_RIS_R, &GPIO_PORTC_RIS_R,
  &GPIO_PORTE_RIS_R, &GPIO_PORTE_RIS_R, &GPIO_PORTF_RIS_R
};
/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
/**
 * Inicializa PORT B
 */
void portB_init(void) {
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x02;                                // B clock
  delay = SYSCTL_RCGC2_R;                                // delay
  GPIO_PORTB_LOCK_R = 0x4C4F434B;                        // unlock PORTF
  GPIO_PORTB_CR_R |= 0x3E;                               // allow changes to PB1-5
  GPIO_PORTB_AMSEL_R &= ~0x3E;                           // disable analog functionality on PORTB
  GPIO_PORTB_PCTL_R &= ~0x3E;                            // configure PB1-5 as GPIO
  GPIO_PORTB_DIR_R |= 0x03C;                             // make PF2 - PF5 output
  GPIO_PORTB_AFSEL_R &= ~0x3E;                           // disable alt funct
  GPIO_PORTB_DEN_R |= 0x3E;                              // enable digital I/O
  GPIO_PORTB_PDR_R |= 0x02;                              // enable weak pull-down on PE1
  GPIO_PORTB_IS_R &= ~0x02;                              // (d) PF4 is edge-sensitive
  GPIO_PORTB_IBE_R &= ~0x02;                             // PF4 is not both edges
  GPIO_PORTB_IEV_R |= 0x02;                              // PF4 rising edge event
  GPIO_PORTB_ICR_R |= 0x02;                              // clear flag1
  GPIO_PORTB_IM_R |= 0x02;                               // arm interrupt on PB1
  NVIC_PRI0_R |= 0xA000;                                 // priority 5
  NVIC_EN0_R |= 0x02;                                    // enable interrupt 1 in NVIC
}

/**
 * Inicializa PORT E
 */
void portE_init(void) {
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x10;                                // E clock
  delay = SYSCTL_RCGC2_R;                                // delay
  GPIO_PORTE_LOCK_R = 0x4C4F434B;                        // unlock PORTE
  GPIO_PORTE_CR_R |= 0x0F;                               // allow changes to PE3-0
  GPIO_PORTE_AMSEL_R &= ~0x0F;                           // disable analog functionality on PORTE
  GPIO_PORTE_PCTL_R = 0x00000000;                        // configure PORTE as GPIO
  GPIO_PORTE_DIR_R &= ~0x0F;                             // make PE0 - PE3 input
  GPIO_PORTE_AFSEL_R &= ~0x0F;                           // disable alt funct
  GPIO_PORTE_DEN_R |= 0x0F;                              // enable digital I/O
  GPIO_PORTE_PDR_R |= 0x0F;                               // enable weak pull-down on PE0-PE3
  GPIO_PORTE_IS_R &= ~0x0F;                              // PE0-PE3 is edge-sensitive
  GPIO_PORTE_IBE_R |= 0x0F;                              // PE0-PE3 is both edges
  GPIO_PORTE_ICR_R |= 0x0F;                               // clear flag4
  GPIO_PORTE_IM_R |= 0x0F;                               // arm interrupt on PE0-PE3
  NVIC_PRI0_R |= 0xA0;                                   // priority 5
  NVIC_EN0_R |= 0x10;                                    // enable interrupt 4 in NVIC
}

/**
 * Inicializa PORT F
 */
void portF_init(void) {
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x20;                                // F clock
  delay = SYSCTL_RCGC2_R;                                // delay
  GPIO_PORTF_LOCK_R = 0x4C4F434B;                        // unlock PORTF
  GPIO_PORTF_CR_R |= 0x04;                               // allow changes to PF2
  GPIO_PORTF_AMSEL_R &= ~0x04;                           // disable analog functionality on PORTF
  GPIO_PORTF_PCTL_R &= ~0x04;                            // configure PF2 as GPIO
  GPIO_PORTF_DIR_R |= 0x04;                              // make PF2 output
  GPIO_PORTF_AFSEL_R &= ~0x04;                           // disable alt funct
  GPIO_PORTF_DEN_R |= 0x04;                              // enable digital I/O
}

/**
 * Retorna ponteiro para o registrador da porta solicitada
 * 
 * @param port - porta a ser realizado o get do registrado
 * @return volatile* - ponteiro para o registrador
 */
volatile uint32_t * port_get_ptr(port_t port) {
  return port < PORT_NUM ? portRegisters[port] : NULL;
}

/**
 * Retorna o valor do registrador de pinos com interrupções pendentes
 * 
 * @param port - porta a ser consultado o registrador
 * @return uint32_t - valor do registrador 
 */
uint32_t port_get_interrupt_pins(port_t port)
{
  return port < PORT_NUM ? *portInterruptPinsRegister[port] : NULL;
}

/**
 * Seta no registrador que o pino teve sua interrupção tratada
 * 
 * @param port - porta a ser manipulado o registrador
 * @param acknowledgeValue - valor a ser setado no registrador
 */
void port_set_interrupt_acknowledge(port_t port, uint32_t acknowledgeValue)
{ 
  *portInterruptAcknowledgeRegister[port] = acknowledgeValue;
}


/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/*******************************************************************************
* END OF FILE
*******************************************************************************/
