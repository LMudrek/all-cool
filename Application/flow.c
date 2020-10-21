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
#include "flow.h"
#include "pwm.h"
#include "timer.h"
#include "interrupt.h"
#include "pins.h"
#include "app.h"
#include "log.h"
#include <stddef.h>
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
 *  \brief Definição da estrutura dos pinos conectados as chaves de controle do
 *  PWM e tempo de liberação
 */
#define FLOW_PWM_SW_PINS_BITS     (FLOW_PWM_SW1_BIT | FLOW_PWM_SW2_BIT)
#define FLOW_PWM_TIMER_PINS_BITS  (FLOW_TIMER_SW1_BIT | FLOW_TIMER_SW2_BIT)
#define FLOW_PWM_SW_PINS	        {FLOW_SW_PORT, FLOW_PWM_SW_PINS_BITS}
#define FLOW_TIMER_SW_PINS	      {FLOW_SW_PORT, FLOW_PWM_TIMER_PINS_BITS}

/**
 *  \brief Definição do timer de controle do tempo de liberação e a base do mesmo
 *  em milisegundos
 */
#define FLOW_TIMER        TMR_1MS_FLOW
#define FLOW_BASE_TIMER   1

/**
*   \brief Periodo em segundos do PWM
*/
#define FLOW_PWM_PERIOD   0.01        

/**
*   \brief Configuração de cada tempo em alta do PWM, atrelado ao estados das 
*   chaves de controle
*/
#define FLOW_DUTY_CONFIG_1_HIGH   0.006
#define FLOW_DUTY_CONFIG_2_HIGH   0.007
#define FLOW_DUTY_CONFIG_3_HIGH   0.008
#define FLOW_DUTY_CONFIG_4_HIGH   0.009
#define FLOW_DUTY_CONFIG_LOW(high)    (FLOW_PWM_PERIOD - high)

/**
*   \brief Tempo de liberação para cada configuração das chaves de controle
*/
#define FLOW_TIMER_CONFIG_1   0.6
#define FLOW_TIMER_CONFIG_2   0.8
#define FLOW_TIMER_CONFIG_3   1
#define FLOW_TIMER_CONFIG_4   1.2

/**
*   \brief Inicialização da struct "flowConfig_t"
*/
#define FLOW_CONFIG_INIT_T    {FLOW_CONFIG_1, FLOW_CONFIG_1}

/**
*   \brief Enum das configurações das chaves de controle
*/
typedef enum flowConfigEnum_t {
  FLOW_CONFIG_1 = 0,
  FLOW_CONFIG_2,
  FLOW_CONFIG_3,
  FLOW_CONFIG_4,
  FLOW_CONFIG_NUM,
} flowConfigEnum_t;

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
/**
*   \brief Estrutura para manipular o PWM (vazão do alcool)
*/
typedef struct flowPwm_t {
  /* Dados do PWM (tempo em alta/baixa e pino de output do PWM) */
	pwm_t pwm;
  /* Configuração da chave de controle */
	pin_t pin;
  /* Sinal a ser setado quando identificar configuração */
	appSignal_t appSignal;
} flowPwm_t;

/**
*   \brief Estrutura para manipular o timer (tempo de vazão do alcool)
*/
typedef struct flowTimer_t {
  /* Dados do timer (base temporal e handler para timeout) */ 
  timerWithCallback_t timer;
  /* Configuração da chave de controle */
	pin_t pin;
  /* Sinal a ser setado quando identificar configuração */
  appSignal_t appSignal;  
} flowTimer_t;

/**
*   \brief Estrutura de configuração da vazão (tempo e quantidade de vazão)
*/
typedef struct flowConfig_t {
  flowConfigEnum_t pwm;
  flowConfigEnum_t timer;
} flowConfig_t;
/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/
static void init_driver(void);
static void flow_pwm_sw_handler(void);
static void flow_timer_sw_handler(void);
static void flow_timer_handler(void);
static flowConfig_t get_flow_config(void);
/*******************************************************************************
* CONSTANTES
*******************************************************************************/
/**
*   \brief Tabela de configuração para o PWM
*/
static const flowPwm_t flowPwm [FLOW_CONFIG_NUM] = {
  /* FLOW_CONFIG_1 - SW1 and SW2 open */
	{
    .pin = NULL_PIN,
    .pwm = {
			.msHigh = TEMPO(FLOW_DUTY_CONFIG_1_HIGH, TIME_BASE_PWM_TIMER),
			.msLow  = TEMPO(FLOW_DUTY_CONFIG_LOW(FLOW_DUTY_CONFIG_1_HIGH), TIME_BASE_PWM_TIMER),
			.pin = FLOW_PWM_PIN,
		},
    .appSignal = APP_SIGNAL_NONE,
  },
  /* FLOW_CONFIG_2 - SW1 pressed */
	{
    .pin = FLOW_PWM_SW1_PIN,
    .pwm =   {
			.msHigh = TEMPO(FLOW_DUTY_CONFIG_2_HIGH, TIME_BASE_PWM_TIMER),
			.msLow  = TEMPO(FLOW_DUTY_CONFIG_LOW(FLOW_DUTY_CONFIG_2_HIGH), TIME_BASE_PWM_TIMER),
			.pin = FLOW_PWM_PIN,
		},
    .appSignal = APP_SIGNAL_SW1_FLOW_PWM,
  },
  /* FLOW_CONFIG_3 - SW2 pressed */
	{
    .pin = FLOW_PWM_SW2_PIN,
    .pwm = {
			.msHigh = TEMPO(FLOW_DUTY_CONFIG_3_HIGH, TIME_BASE_PWM_TIMER),
			.msLow  = TEMPO(FLOW_DUTY_CONFIG_LOW(FLOW_DUTY_CONFIG_3_HIGH), TIME_BASE_PWM_TIMER),
			.pin = FLOW_PWM_PIN,
		},
    .appSignal = APP_SIGNAL_SW2_FLOW_PWM,
  },
  /* FLOW_CONFIG_4 - SW1 and SW2 pressed */
	{
    .pin = FLOW_PWM_SW_PINS,
    .pwm = {
			.msHigh = TEMPO(FLOW_DUTY_CONFIG_4_HIGH, TIME_BASE_PWM_TIMER),
			.msLow  = TEMPO(FLOW_DUTY_CONFIG_LOW(FLOW_DUTY_CONFIG_4_HIGH), TIME_BASE_PWM_TIMER),
      .pin = FLOW_PWM_PIN,
		},
  },
};

/**
*   \brief Tabela de configuração para o timer
*/
static const flowTimer_t flowTimer[FLOW_CONFIG_NUM] = {
  {
    .pin = NULL_PIN,
    .timer = {
      .timerEnum = FLOW_TIMER,
      .timeout = TEMPO(FLOW_TIMER_CONFIG_1, FLOW_BASE_TIMER),
      .timeoutHandler = flow_timer_handler,
    },
    .appSignal = APP_SIGNAL_NONE,
  },
  {
    .pin = FLOW_TIMER_SW1_PIN,
    .timer = {
      .timerEnum = FLOW_TIMER,
      .timeout = TEMPO(FLOW_TIMER_CONFIG_2, FLOW_BASE_TIMER),
      .timeoutHandler = flow_timer_handler,
    },
    .appSignal = APP_SIGNAL_SW1_FLOW_TIMER,
  },
  {
    .pin = FLOW_TIMER_SW2_PIN,
    .timer = {
      .timerEnum = FLOW_TIMER,
      .timeout = TEMPO(FLOW_TIMER_CONFIG_3, FLOW_BASE_TIMER),
      .timeoutHandler = flow_timer_handler,
    },
    .appSignal = APP_SIGNAL_SW2_FLOW_TIMER,
  },
  {
    .pin = FLOW_TIMER_SW_PINS,
    .timer = {
      .timerEnum = FLOW_TIMER,
      .timeout = TEMPO(FLOW_TIMER_CONFIG_4, FLOW_BASE_TIMER),
      .timeoutHandler = flow_timer_handler,
    },
  }
};
/*******************************************************************************
* VARIÁVEIS
*******************************************************************************/
/**
*   \brief Tabela de configuração para as interrupções das chaves de controle
*/
static interrupt_t portEInterrupt[FLOW_CONFIG_NUM + 1] = {
  {
    .pin = FLOW_PWM_SW1_PIN,
    .handler = flow_pwm_sw_handler,
  },
  {
    .pin = FLOW_PWM_SW2_PIN,
    .handler = flow_pwm_sw_handler,
  },
  {
    .pin = FLOW_TIMER_SW1_PIN,
    .handler = flow_timer_sw_handler,
  },
  {
    .pin = FLOW_TIMER_SW2_PIN,
    .handler = flow_timer_sw_handler,
  },
  /* NULL */
  {
    .pin = NULL_PIN,
    .handler = NULL,
  }
};

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
/**
 * Inicializa interface que irá controlar a vazão do alcool 
 * 
 */
void flow_init(void) {
  /* Inicializa driver da bomba peristaltica */
  init_driver();
  /* Inicializa interrupções dos pinos das chaves de controle */
  interrupt_port_init(portEInterrupt);
}

/**
 * Habilita vazão
 * 
 * @param enable - booleano para controlar se inicia ou para a vazão de alcool 
 */
void flow_enable(bool enable) {
  /* Verifica configuração das chaves de controle */
  flowConfig_t flowConfig = get_flow_config();

  if (enable) {
    /* Se deve iniciar vazão, configura timer e PWM */
    log_write2(Flow enable - PWM: %d / Timer: %d, flowConfig.pwm, flowConfig.timer);
		pwm_set((pwm_t*)&flowPwm[flowConfig.pwm].pwm, true);
    timer_set(flowTimer[flowConfig.timer].timer.timerEnum,
              flowTimer[flowConfig.timer].timer.timeout,
              flowTimer[flowConfig.timer].timer.timeoutHandler);
  } else {
    /* Se deve parar a vazão, limpa estruturas */

		pwm_set((pwm_t*)&flowPwm[flowConfig.pwm].pwm, false);
    timer_set(flowTimer[flowConfig.timer].timer.timerEnum,
              SEM_TEMPORIZACAO,
              NULL);
  }
}

/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/
/**
 * Inicializa os pinos de controle do driver (L2963D) da bomba 
 * 
 */
static void init_driver(void) {
	pin_t pin1 = FLOW_DRIVER_INPUT_1;
  io_write_pin(pin1, true);
	
	pin_t pin2 = FLOW_DRIVER_INPUT_2;
  io_write_pin(pin2, false);
}

/**
 * Verifica os status das chaves de configuração de vazão e tempo
 * 
 * @return flowConfig_t - estrutura preenchida com as configurações
 */
static flowConfig_t get_flow_config(void) {
  /* Inicializa estrutura */
  flowConfig_t config = FLOW_CONFIG_INIT_T;
  
  /* Lê porta das chaves */
	uint32_t switchValue = io_read_port(FLOW_SW_PORT);
	
	for (flowConfigEnum_t idx = FLOW_CONFIG_1; idx < FLOW_CONFIG_NUM; idx ++) {
    if ((switchValue & FLOW_PWM_SW_PINS_BITS) == flowPwm[idx].pin.bit) {
      /* Chaves iguais ao determinado na tabela, define configuração */
      config.pwm = idx;
    }

    if ((switchValue & FLOW_PWM_TIMER_PINS_BITS) == flowTimer[idx].pin.bit) {
      /* Chaves iguais ao determinado na tabela, define configuração */
      config.timer = idx;
    }
  }

  return config;
}

/**
 * Handler para tratar interrupção gerada pelas chaves de configuração do PWM
 * 
 */
static void flow_pwm_sw_handler(void) {
	flowConfigEnum_t flowConfig = get_flow_config().pwm;

  app_set_signal(flowPwm[flowConfig].appSignal);
}

/**
 * Handler para tratar interrupção gerada pelas chaves de configuração do timer
 * 
 */
static void flow_timer_sw_handler(void){
  flowConfigEnum_t flowConfig = get_flow_config().timer;

  app_set_signal(flowTimer[flowConfig].appSignal);
}

/**
 * Handler para timeout da liberação do alcool
 * 
 */
static void flow_timer_handler(void){
  app_set_signal(APP_SIGNAL_STOP_FLOW);
}
/*******************************************************************************
* END OF FILE
*******************************************************************************/
