/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
*
* License : MIT
*******************************************************************************/

#ifndef PINS_H
#define PINS_H

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include "io.h"

/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
*   \brief  Definição pino nulo
*/
#define NULL_PIN              {(port_t)0xFF, NULL}

/**
*   \brief  Definição do pino para o botão de start
*/
#define START_BUTTON          {PORT_B, PIN_1_MASK}

/**
*   \brief  Definição dos pinos para o driver L293D 
*/
#define FLOW_DRIVER_INPUT_1   {PORT_B, PIN_2_MASK}
#define FLOW_DRIVER_INPUT_2   {PORT_B, PIN_3_MASK}
#define FLOW_PWM_PIN          {PORT_F, PIN_2_MASK}

/**
*   \brief  Definição para os pinos de sinalização via LED
*/
#define LED_POWER             {PORT_B, PIN_4_MASK}
#define LED_FLOW              {PORT_B, PIN_5_MASK}

/**
*   \brief  Definição dos pinos para as chaves de configuração da vazão e do tempo
*/
#define FLOW_SW_PORT            PORT_E
#define FLOW_PWM_SW1_BIT        PIN_0_MASK
#define FLOW_PWM_SW2_BIT        PIN_1_MASK
#define FLOW_TIMER_SW1_BIT      PIN_2_MASK
#define FLOW_TIMER_SW2_BIT      PIN_3_MASK

#define FLOW_PWM_SW1_PIN      {FLOW_SW_PORT, FLOW_PWM_SW1_BIT}
#define FLOW_PWM_SW2_PIN      {FLOW_SW_PORT, FLOW_PWM_SW2_BIT} 
#define FLOW_TIMER_SW1_PIN    {FLOW_SW_PORT, FLOW_TIMER_SW1_BIT} 
#define FLOW_TIMER_SW2_PIN    {FLOW_SW_PORT, FLOW_TIMER_SW2_BIT} 

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/

/*******************************************************************************
* END OF FILE
*******************************************************************************/
#endif
