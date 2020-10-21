/*******************************************************************************
* Leonardo Mudrek de Almeida
* UTFPR - CT
*
*
* License : MIT
*******************************************************************************/
#ifndef LOG_H
#define LOG_H

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdint.h>
/*******************************************************************************
* DEFINES E ENUMS
*******************************************************************************/
/**
*   \brief Macro para transformar texto em string
*/
#define is_string(c) #c

/**
*   \brief Macros para escrita de logs junto com variáveis
*/
#define log_printf(a) log_write((uint8_t *)is_string(a))
#define log_write1(a, b) log_write_variable((uint8_t *)is_string(a), (uint8_t)b, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)1)
#define log_write2(a, b, c) log_write_variable((uint8_t *)is_string(a), (uint8_t)b, (uint8_t)c, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)2)
#define log_write3(a, b, c, d) log_write_variable((uint8_t *)is_string(a), (uint8_t)b, (uint8_t)c, (uint8_t)d, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)3)
#define log_write4(a, b, c, d, e) log_write_variable((uint8_t *)is_string(a), (uint8_t)b, (uint8_t)c, (uint8_t)d, (uint8_t)e, (uint8_t)0, (uint8_t)0, (uint8_t)4)
#define log_write5(a, b, c, d, e, f) log_write_variable((uint8_t *)is_string(a), (uint8_t)b, (uint8_t)c, (uint8_t)d, (uint8_t)e, (uint8_t)f, (uint8_t)0, (uint8_t)5)
#define log_write6(a, b, c, d, e, f, g) log_write_variable((uint8_t *)is_string(a), (uint8_t)b, (uint8_t)c, (uint8_t)d, (uint8_t)e, (uint8_t)f, (uint8_t)g, (uint8_t)6)

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/

/*******************************************************************************
* FUNÇÕES EXPORTADAS
*******************************************************************************/
void log_init(void);
void log_write(uint8_t *bufferPtr);
void log_write_variable(uint8_t *bufferPtr,
                        const uint8_t dado1,
                        const uint8_t dado2,
                        const uint8_t dado3,
                        const uint8_t dado4,
                        const uint8_t dado5,
                        const uint8_t dado6,
                        const uint8_t numVar);
/*******************************************************************************
* END OF FILE
*******************************************************************************/
#endif
