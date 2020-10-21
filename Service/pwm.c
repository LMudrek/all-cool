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
#include "pwm.h"
#include "timer.h"
#include "io.h"
#include "log.h"
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
*   \brief Ponteiro para a estrutura de controle do PWM 
*/
static pwm_t * pwmPtr = NULL;

/**
*   \brief Variavel de indicação do funcionamento do PWM
*/
static bool pwmEnable = false;
/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/
void pwm_init_high(void);
void pwm_init_low(void);
void pwm_handler_high(void);
void pwm_handler_low(void);
/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
/**
 * Set valores na estrutura do PWM
 * 
 * @param pwmSet - estrutura a ser setada
 * @param enable - habilita ou não PWM
 */
void pwm_set(pwm_t * pwmSet, bool enable) {
  pwmPtr = pwmSet;
  pwmEnable = enable;
  
  // Inicia forma de onde quadrada
  io_write_pin(pwmPtr->pin, true);

  pwm_init_high();
}

/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/**
 * Inicializa timer para controlar tempo em alta
 */
void pwm_init_high(void) {
  pwmEnable ? timer_set(TMR_1MS_PWM, pwmPtr->msHigh, pwm_handler_low) : 
							io_write_pin(pwmPtr->pin, false);
}

/**
 * Inicializa timer para controlar tempo em baixa
 */
void pwm_init_low(void) {
  pwmEnable ? timer_set(TMR_1MS_PWM, pwmPtr->msLow, pwm_handler_high) : 
              io_write_pin(pwmPtr->pin, false);
}

/**
 * Handler para timeout no tempo de alta da onda
 */
void pwm_handler_high(void) {
  io_write_pin(pwmPtr->pin, true);
	pwm_init_high();
}

/**
 * Handler para timeout no tempo de baixa da onda
 */
void pwm_handler_low(void) {
  io_write_pin(pwmPtr->pin, false);
	pwm_init_low();
}
/*******************************************************************************
* END OF FILE
*******************************************************************************/
