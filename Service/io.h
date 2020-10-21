/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
*
* License : MIT
*******************************************************************************/
#ifndef IO_H
#define IO_H

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "port.h"
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
*   \brief Definição das mascaras para cada bit 
*/
#define PIN_0_MASK              0x01
#define PIN_1_MASK              0x02
#define PIN_2_MASK              0x04
#define PIN_3_MASK              0x08
#define PIN_4_MASK              0x010
#define PIN_5_MASK              0x020

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
/**
*   \brief Estrutura de um pino
*/
typedef struct pin_t {
  /* Port ao qual o pino pertence */
  port_t port;
  /* Bit ao qual o pino pertence no port */
  uint32_t bit;
} pin_t;

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
void io_init(void);
bool io_write_pin(pin_t pin, bool set);
bool io_read_pin(pin_t pin);
uint32_t io_read_port(port_t port);
/*******************************************************************************
* END OF FILE
*******************************************************************************/
#endif
