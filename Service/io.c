/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
*
* License : MIT
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "io.h"
#include "adc.h"
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

/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
/**
 * Inicializa PORTs e ADC
 * 
 */
void io_init(void) {
  portB_init();										 		// initialize PortB
  portE_init();										 		// initialize PortE
  portF_init();										 		// initialize PortF

  adc_init();
}

/**
 * Escreve valor digital em um pino
 * 
 * @param pin - pino a ser manipulado
 * @param set - valor a ser carregado no pino
 * @return true - conseguiu identificar o pino e manipular
 * @return false - não conseguiu identificar o pino 
 */
bool io_write_pin(pin_t pin, bool set) {
  volatile uint32_t * portPtr = port_get_ptr(pin.port);
  if (portPtr == NULL) {
    return false;
  }

  *portPtr = set ? (*portPtr | pin.bit) : (*portPtr & ~(pin.bit));

  return true;
} 

/**
 * Lê valor digital em um pino
 * 
 * @param pin - pino a ser lido
 * @return true - conseguiu identificar o pino e manipular
 * @return false - não conseguiu identificar o pino 
 */
bool io_read_pin(pin_t pin) {
  return *port_get_ptr(pin.port) & pin.bit;
}

/**
 * Realiza leitura de um PORT inteiro
 * 
 * @param port - port a ser lido
 * @return uint32_t - valor do port 
 */
uint32_t io_read_port(port_t port) {
  return *port_get_ptr(port);
}
/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/*******************************************************************************
* END OF FILE
*******************************************************************************/
