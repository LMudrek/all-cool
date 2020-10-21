/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
*
* License : MIT
*******************************************************************************/
#ifndef TIMER_H
#define TIMER_H

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
*   \brief Número de timers de 10ms
*/
#define TMR_1MS_NUM   (TMR_1MS_END - TMR_1MS_INIT - 1)

/**
*   \brief Número de timers de 10ms
*/
#define TMR_10MS_NUM  (TMR_10MS_END - TMR_10MS_INIT - 1)

/**
*   \brief Número de timers de 100ms
*/
#define TMR_100MS_NUM (TMR_100MS_END - TMR_100MS_INIT - 1)

/**
*   \brief Número de timers de 1s
*/
#define TMR_1S_NUM    (TMR_1S_END - TMR_1S_INIT - 1)

/**
*   \brief  Tempo para carregar timers que não devem ser decrementados
*/
#define SEM_TEMPORIZACAO 0

/**
 *  Tempo para carregar timers nas bases requesitadas
 */
#define TEMPO(tempoS, baseMs) (tempoS * (1000u / baseMs))

/**
*   \brief  Identificador dos timers
*
*   Para adicionar novos timers, basta inserir um nome para o seu indicador
* entre os delimitadores TMR_base_INIT e TMR_base_END para a base desejada.\n
*   As bases que ainda não possuem identificadores são preenchidas com
* TMR_base_VAZIO apenas para evitar um warning de compilação.
*/
typedef enum timer_t
{
  TMR_1MS_INIT = 0,
  TMR_1MS_PWM,
  TMR_1MS_FLOW,
  TMR_1MS_END,

  TMR_10MS_INIT,
  TMR_10MS_VAZIO,
  TMR_10MS_END,

  TMR_100MS_INIT,
  TMR_100MS_APP,
  TMR_100MS_END,

  TMR_1S_INIT,
  TMR_1S_VAZIO,
  TMR_1S_END,

  TMR_NUM,
} timer_t;
/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
/**
*   \brief Callback para tratar timeout do timer
*/
typedef void (*timerCallback_t)(void);

/**
*   \brief Estrutura de controle para um timer
*/
typedef struct timerWithCallback_t
{
  /* Timer ao qual será utilizado */
  timer_t timerEnum;
  /* Valor a ser carregado no timer */
	uint32_t timeout;
  /* Handler para o timeout */
	timerCallback_t timeoutHandler;
} timerWithCallback_t;
/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
void timer_init(void);
bool timer_set(const timer_t timerNum, const uint32_t timeout, timerCallback_t timeoutHandler);
bool timer_expired(const timer_t timerNum);
/*******************************************************************************
* END OF FILE
*******************************************************************************/
#endif
