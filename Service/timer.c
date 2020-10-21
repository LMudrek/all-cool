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
#include "timer.h"
#include "io.h"
#include "SysTick.h"
#include "PLL.h"
#include <stddef.h>
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
*   \brief Definição do periodo do SysTick (1ms - 80MHz)
*/
#define SYSTICK_PERIOD_1MS 80000

/**
*   \brief Coeficiente de inicialização da base inicial 
*/
#define BASE_INIT			1

/**
*   \brief Coeficiente de adição para gerar de base 10ms
*/
#define BASE_10MS			TEMPO(0.01, BASE_INIT)

/**
*   \brief Coeficiente de adição para gerar de base 100ms
*/
#define BASE_100MS		TEMPO(0.1, BASE_INIT)

/**
*   \brief Coeficiente de adição para gerar de base 1s
*/
#define BASE_1S				TEMPO(1, BASE_INIT)

/**
*   \brief Ordenação bases
*/
typedef enum timerBaseSequence_t {
	TIMER_1MS,
	TIMER_10MS,
	TIMER_100MS,
	TIMER_1S,
	TIMER_BASE_NUM,
	TIMER_NULL = INT32_MAX,
} timerBaseSequence_t;

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
typedef struct timerProperty_t {
	uint32_t base;
	timer_t init;
	timer_t end;
} timerProperty_t;

/*******************************************************************************
* CONSTANTES
*******************************************************************************/
static const timerProperty_t timerProperty[TIMER_BASE_NUM] = {
	/* TMR_1MS */
	{
		.base = BASE_INIT, 	
		.init = TMR_1MS_INIT,
		.end = TMR_1MS_END,
	},
	/* TMR_10MS */
	{
		.base = BASE_10MS, 	
		.init = TMR_10MS_INIT,
		.end = TMR_10MS_END,
	},
	/* TMR_100MS */
	{
		.base = BASE_100MS, 	
		.init = TMR_100MS_INIT,
		.end = TMR_100MS_END,
	},
	/* TMR_1S */
	{
		.base = BASE_1S, 	
		.init = TMR_1S_INIT,
		.end = TMR_1S_END,
	},
};

/*******************************************************************************
* VARIÁVEIS
*******************************************************************************/
/**
*   \brief Base de timers do sistema
*/
static volatile uint32_t timerBase[TIMER_BASE_NUM];

/**
*   \brief Pool de timers do sistema
*/
static volatile timerWithCallback_t timer[TMR_NUM];

/*******************************************************************************
* PROTÓTIPOS DE FUNÇÕES
*******************************************************************************/
static timerProperty_t get_timer_property(const timer_t timerNum);
static void init_timer_base(void);
/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
/**
 * Inicializa timer base  
 */
void timer_init(void)
{
	PLL_Init();  

	/* Periodo SysTick na menor base temporal da estrutura do timer */                 		
	SysTick_Init(SYSTICK_PERIOD_1MS);			

	init_timer_base();
}

/**
 * Handler do SysTick
 */
void SysTick_Handler(void)
{
	for (timerBaseSequence_t baseIdx = (timerBaseSequence_t)0; baseIdx < TIMER_BASE_NUM; baseIdx++) {

		timerProperty_t timerConfig = timerProperty[baseIdx];

		/* Verifica se deve tratar a base da posição do buffer */
		if (--timerBase[baseIdx] == 0)	{
			for (timer_t timerIdx = (timer_t)(timerConfig.init + 1); timerIdx < timerConfig.end; timerIdx ++) {
				/* Verifica se existe algum timer a ser decrementado */
				if (timer[timerIdx].timeout != 0) {
					if (--timer[timerIdx].timeout == 0)
					{
						/* Caso tenha dado timeout, executa handler cadastrado */
						if (timer[timerIdx].timeoutHandler != NULL)
						{
							timer[timerIdx].timeoutHandler();
						}
					}
				}
			}
			/* Recarrega base */
			timerBase[baseIdx] = timerConfig.base;
		}
	}
}

/**
*   \brief  Dispara ou reseta timer
*
*   A função seta o valor do timer. Para verificar se o timer estourou,
* utilize timer_expired
*
*   \param  timerNum    Identificador do timer
*   \param  timeout     Timeout na base do timer
*   \retval true    Timer ajustado com sucesso
*   \retval false   Identificador inválido
*/
bool timer_set(const timer_t timerNum, const uint32_t timeout, timerCallback_t timeoutHandler)
{
	uint32_t timerBase = get_timer_property(timerNum).base;

	if (timerBase == TIMER_NULL) {
		return false;
	}

	timer[timerNum].timeout = timeout;
	timer[timerNum].timeoutHandler = timeoutHandler;

	return true;
}

/**
*   \brief  Verifica se timer estourou
*
*   A função verifica se o timer chegou a zero.
*
*   \param  timerNum Identificador do timer
*   \retval true    Timer estourou
*   \retval false   Timer não estourou ou identificador inválido
*/

bool timer_expired(const timer_t timerNum)
{
	uint32_t timerBase = get_timer_property(timerNum).base;

	if (timerBase == TIMER_NULL) {
		return false;
	}

	return timer[timerNum].timeout == SEM_TEMPORIZACAO;
}
/*******************************************************************************
* FUNÇÕES LOCAIS
*******************************************************************************/

/**
 * Get propriedade de base e posição no buffer do timer requisitado
 * 
 * @param timerNum Identificador do timer
 * @return timerProperty_t estrutura de propriedades do timer requisitado
 */
static timerProperty_t get_timer_property(const timer_t timerNum) {
	if ((timerNum > TMR_1MS_INIT) &&
			(timerNum < TMR_1MS_END))
	{
		return timerProperty[TIMER_1MS];
	}
	else if ((timerNum > TMR_10MS_INIT) &&
					 (timerNum < TMR_10MS_END))
	{
		return timerProperty[TIMER_10MS];
	}
	else if ((timerNum > TMR_100MS_INIT) &&
					 (timerNum < TMR_100MS_END))
	{
		return timerProperty[TIMER_100MS];
	}
	else if ((timerNum > TMR_1S_INIT) &&
					 (timerNum < TMR_1S_END))
	{
		return timerProperty[TIMER_1S];
	}
	else
	{
		timerProperty_t timerNull;
		timerNull.base = TIMER_NULL;
		return timerNull;
	}
}

/**
 * Inicializa array de bases dos timers
 * 
 */
static void init_timer_base(void) {
	for (timerBaseSequence_t baseIdx = (timerBaseSequence_t)0; baseIdx < TIMER_BASE_NUM; baseIdx++) {
		timerBase[baseIdx] = timerProperty[baseIdx].base;
	}
}
/*******************************************************************************
* END OF FILE
*******************************************************************************/
