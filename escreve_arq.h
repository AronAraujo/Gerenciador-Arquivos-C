#pragma once
#include <stdio.h>
#include "structs.h"

/*!
 * @brief Escreve o registro de cabeçalho no arquivo binário (17 bytes).
 * @param f Arquivo binário aberto para escrita.
 * @param h Estrutura Head com os dados.
 */
void escreve_header_bin(FILE* f, Head* h);

/*!
 * @brief Escreve um registro de dados no arquivo binário (exatos 80 bytes).
 * @param f Arquivo binário aberto para escrita.
 * @param r Estrutura Reg com os dados lidos.
 */
void escreve_reg_bin(FILE* f, Reg* r);