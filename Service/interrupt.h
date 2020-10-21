/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
*
* License : MIT
*******************************************************************************/
#ifndef INTERRUPT_H
#define INTERRUPT_H

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "io.h"
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
/**
*   \brief Definição callback para tratamento de interrupções no pino
*/
typedef void (*interruptHandler_t)(void);

/**
*   \brief Estrutura de controle de interrupção
*/
typedef struct interrupt_t {
  /* Pino ao qual será tratada a interrupção */
  pin_t pin;
  /* Handler para quando ouver a interrupção */
  interruptHandler_t handler;
} interrupt_t;

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
void interrupt_port_init(interrupt_t * bufferPtr);
/*******************************************************************************
* END OF FILE
*******************************************************************************/
#endif