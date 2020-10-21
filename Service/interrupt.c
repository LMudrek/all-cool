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
#include "interrupt.h"
#include "port.h"
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
*   \brief Buffer de ponteiros cada port e suas interrupções
*/
static interrupt_t * portInterruptPtr[PORT_NUM] = {NULL};

/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/

/**
 * Inicializa posição no buffer de controle de interrupções
 * 
 * @param bufferPtr - ponteiro a ser carregado
 */
void interrupt_port_init(interrupt_t * bufferPtr) {
	if (bufferPtr != NULL) {
		portInterruptPtr[bufferPtr->pin.port] = bufferPtr;
	}
}

/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/**
 * Realiza tratamento nas interrupções de um port
 * 
 * @param port - port a ter sua interrupção tratada
 */
void interrupt_handler(port_t port) {
  /* Recupera ponteiro do port */
  const interrupt_t * bufferPtr = portInterruptPtr[port];

  /* Verifica quais pinos possuem interrupção pendente */
  const uint32_t interruptPins = port_get_interrupt_pins(port);
	
	if (bufferPtr != NULL) {
		for (uint32_t idx = 0; bufferPtr[idx].pin.bit != NULL; idx++) {
			if (interruptPins & bufferPtr[idx].pin.bit) {
        /* Pino tem interrupção pendente */
				if (bufferPtr[idx].handler != NULL) {
          /* Trata interrupção */
					bufferPtr[idx].handler();
				}
			}  
		}
	}

  /* Limpa flag, indicando que pinos tiveram interrupção tratada */
  port_set_interrupt_acknowledge(port, interruptPins);
}

/**
 * Handler para interrupções no PORT E
 * 
 */
void GPIOPortE_Handler(void) {
  interrupt_handler(PORT_E);
}

/**
 * Handler para interrupções no PORT B
 * 
 */
void GPIOPortB_Handler(void) {
  interrupt_handler(PORT_B);
}
/*******************************************************************************
* END OF FILE
*******************************************************************************/
