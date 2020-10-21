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
#include "app.h"
#include "log.h"
#include "flow.h"
#include "timer.h"
#include "io.h"
#include "pins.h"
#include <stdbool.h>
#include <stddef.h>
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
*   \brief  Base (milisegundos) do timer utilizado na APP
*/
#define BASE_TIMER_APP  100

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
/**
*   \brief  Callback para tratar os sinais setados
*/
typedef void(*appSignalCallback_t)(void);

/**
*   \brief  Struct para tratar sinais da APP
*/
typedef struct appSignalThread_t {
  /* Sinal que será verificado */
  appSignal_t signal;
  /* Função a ser chamada para tratar o sinal */
  appSignalCallback_t callback;
  /* String para efeitos de log para quando tratar o sinal */
  char * log;
} appSignalThread_t;

/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/
static PT_THREAD (taskLedBlinking(struct pt * pt));
static void start_flow(bool start);
static void handlerSignalStopFlow(void);
static void handlerSignalHandsPresent(void);
static void signal_handler(appSignal_t signal, const appSignalThread_t * appSignalHandler);
/*******************************************************************************
* CONSTANTES
*******************************************************************************/
/**
*   \brief  Tabela de tratamento dos sinais 
*/
static const appSignalThread_t appSignalHandler[] = {
  {
    .signal = APP_SIGNAL_SW_START,          
    .callback = NULL,
    .log = "SW start pressed"
  },
  {
    .signal = APP_SIGNAL_SW1_FLOW_PWM,          
    .callback = NULL,
    .log = NULL
  },
  {
    .signal = APP_SIGNAL_SW2_FLOW_PWM,          
    .callback = NULL,
    .log = NULL
  },
  {
    .signal = APP_SIGNAL_SW1_FLOW_TIMER,         
    .callback = NULL,
    .log = NULL
  },
  {
    .signal = APP_SIGNAL_SW2_FLOW_TIMER,          
    .callback = NULL,
    .log = NULL
  },
  {
    .signal = APP_SIGNAL_HANDS_PRESENT,          
    .callback = handlerSignalHandsPresent,
    .log = "Hands present"
  },
  {
    .signal = APP_SIGNAL_STOP_FLOW,          
    .callback = handlerSignalStopFlow,
    .log = "Stop flow"
  },    
  {
    .signal = APP_SIGNAL_NONE,       
    .callback = NULL,
    .log = NULL
  },
};
/*******************************************************************************
* VARIÁVEIS
*******************************************************************************/
/**
*   \brief  Variável para sinais a serem tratados 
*/
static volatile appSignal_t appSignal = APP_SIGNAL_NONE;

/**
*   \brief  Estrutura de controle da protothread 
*/
static struct pt taskLedBlinkingPt = {0};
/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
PT_THREAD(taskApp(struct pt *pt))
{
    PT_BEGIN(pt);

    /* Aguarda botão de início ser pressionado */
    while(PT_SCHEDULE(taskLedBlinking(&taskLedBlinkingPt)));

    /* Limpa variável para iniciar monitoramento */
    appSignal = APP_SIGNAL_NONE;

    while(true) {
      /* Aguarda ter um sinal para ter tratado */
      PT_WAIT_UNTIL(pt, appSignal != APP_SIGNAL_NONE);

      signal_handler(appSignal, appSignalHandler);
    }

    PT_END(pt);
}

/**
 * Seta sinal na app
 * 
 * @param signal - sinal a ser setado
 */
void app_set_signal(appSignal_t signal) {
  appSignal |= signal;
}

/**
 * Limpa sinal na app
 * 
 * @param signal 
 */
void app_clear_signal(appSignal_t signal) {
  appSignal &= ~signal;
}
/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/
/**
 * Thread para verificar início da aplicação 
 * 
 * @param pt - estrutura básica protothread
 */
static PT_THREAD (taskLedBlinking(struct pt * pt)) {
	static pin_t powerLedPin = LED_POWER;
	
  PT_BEGIN(pt);

  /* Aguarda pressionar botão de início  */
  while ((appSignal & APP_SIGNAL_SW_START) == false) {
    /* Enquanto botão não pressionado fica invertendo LED de sinalização (POWER) */
    
    io_write_pin(powerLedPin, !io_read_pin(powerLedPin));
    timer_set(TMR_100MS_APP, TEMPO(0.2, BASE_TIMER_APP), NULL);
    PT_WAIT_UNTIL(pt, (timer_expired(TMR_100MS_APP)) || (appSignal & APP_SIGNAL_SW_START));
  }

  /* Limpa sinal que ficou aguardando */ 
  app_clear_signal(APP_SIGNAL_SW_START);

  /* Aplicação pronta para dispensar alcool */
  log_printf(Ready to dispense);
  io_write_pin(powerLedPin, true);

  PT_END (pt);
}

static void signal_handler(appSignal_t signal, const appSignalThread_t * appSignalHandler) {
  for (uint32_t idx = 0; appSignalHandler[idx].signal != APP_SIGNAL_NONE; idx ++) {

    /* Verifica se sinal da tabela está setado */
    if (appSignal & appSignalHandler[idx].signal) {
      log_write((uint8_t *)appSignalHandler[idx].log);

      if (appSignalHandler[idx].callback !=  NULL) {
        /* Se existe um handler cadastrado, realiza tratamento do sinal */
        appSignalHandler[idx].callback();
      }

      /* Limpa sinal setado e tratado */
      app_clear_signal(appSignalHandler[idx].signal);
    }
  }
}

/**
 * Handler para tratar sinal de mãos presente
 */
static void handlerSignalHandsPresent(void) {
  start_flow(true);
}

/**
 * Handler para tratar sinal de finalização da liberação do alcool
 */
static void handlerSignalStopFlow(void) {
  start_flow(false);
}

/**
 * Manipula liberação do alcool 
 * 
 * @param start - booleano para indicar se deve liberar ou parar o fluxo de alcool
 */
static void start_flow(bool start) {
  /* Habilita fluxo */
  flow_enable(start);

  /* Sinaliza no LED */
	pin_t pin = LED_FLOW;
	io_write_pin(pin, start);
}
/*******************************************************************************
* END OF FILE
*******************************************************************************/
