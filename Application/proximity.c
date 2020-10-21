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
#include "proximity.h"
#include "adc.h"
#include "app.h"
#include "log.h"
#include "timer.h"
#include <stdbool.h>
#include <stddef.h>
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
*   \brief Definição da janela de leitura de mãos presente para o sensor de 
*          proximidade 
*/
#define HANDS_PRESENT_VOLTAGE_HIGHER_VALUE   2.30
#define HANDS_PRESENT_VOLTAGE_LOWER_VALUE    1.75
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
static bool hands_present(void);

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
/**
 * Thread para aferição se existe uma mão no sensor de proximidade
 * 
 * @param pt - estrutura básica protothread
 */
PT_THREAD(taskProximity(struct pt *pt))
{
    PT_BEGIN(pt);

    while(true) {
      /* Aguarda mão presente no sensor */
      PT_WAIT_UNTIL(pt, hands_present());

      app_set_signal(APP_SIGNAL_HANDS_PRESENT);
      /* Yield para tratar sinal na app */
      PT_YIELD(pt);

      /* Aguarda finalizar a vazão do alcool ou a mão ser retirada */
      PT_WAIT_UNTIL(pt, timer_expired(TMR_1MS_FLOW) || hands_present() == false);

      if (hands_present() == false) {
        /* Mão retirada antes do tempo máximo, para o fluxo */
        app_set_signal(APP_SIGNAL_STOP_FLOW);
      } else {
				/* Tempo maximo de vazão, aguarda mão ser retirada para nova liberação */
				PT_WAIT_UNTIL(pt, hands_present() == false);
			}
    }

    PT_END(pt);
}
/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/
/**
 * Verifica se existe mão presente no sensor
 *  
 * @return true - mãos presente
 * @return false - não existe mão presente
 */
static bool hands_present(void) {
  float adcValue = adc_get_value() * ADC_RESOLUTION;
  return (((adcValue) < HANDS_PRESENT_VOLTAGE_HIGHER_VALUE) && 
				 	((adcValue) > HANDS_PRESENT_VOLTAGE_LOWER_VALUE));
}
/*******************************************************************************
* END OF FILE
*******************************************************************************/
