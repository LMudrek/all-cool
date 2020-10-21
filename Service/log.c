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
#include "UART.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
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
 * Inicializa LOG
 */
void log_init(void)
{
  UART_Init();

  log_printf(Log init OK!);
}

/**
 * Escreve string na UART 
 * 
 * @param bufferPtr - ponteiro para a string a ser escrita
 */
void log_write(uint8_t *bufferPtr)
{
	if (bufferPtr == NULL) {
		return;
	}
	
  uint32_t idx = 0;
  while (bufferPtr[idx] != NULL)
  {
    UART_OutChar(bufferPtr[idx++]);
  }

  UART_OutChar('\n');
}

/*
*  Salva texto do log junto com a variavel 
*/
void log_write_variable(uint8_t *bufferPtr,
                        const uint8_t dado1,
                        const uint8_t dado2,
                        const uint8_t dado3,
                        const uint8_t dado4,
                        const uint8_t dado5,
                        const uint8_t dado6,
                        const uint8_t numVar)
{
  uint8_t bufferPtrTemp[50];

  memcpy(bufferPtrTemp, bufferPtr, strlen((char *)bufferPtr));

  if (numVar == 1)
  {
    sprintf((char *)bufferPtrTemp, (char *)bufferPtr, dado1);
  }
  else if (numVar == 2)
  {
    sprintf((char *)bufferPtrTemp, (char *)bufferPtr, dado1, dado2);
  }
  else if (numVar == 3)
  {
    sprintf((char *)bufferPtrTemp, (char *)bufferPtr, dado1, dado2, dado3);
  }
  else if (numVar == 4)
  {
    sprintf((char *)bufferPtrTemp, (char *)bufferPtr, dado1, dado2, dado3, dado4);
  }
  else if (numVar == 5)
  {
    sprintf((char *)bufferPtrTemp, (char *)bufferPtr, dado1, dado2, dado3, dado4, dado5);
  }
  else if (numVar == 6)
  {
    sprintf((char *)bufferPtrTemp, (char *)bufferPtr, dado1, dado2, dado3, dado4, dado5, dado6);
  }

  log_write(bufferPtrTemp);
}
/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/*******************************************************************************
* END OF FILE
*******************************************************************************/
