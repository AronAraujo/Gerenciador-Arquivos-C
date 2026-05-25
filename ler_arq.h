#pragma once
#include <stdio.h>
#include "structs.h"

/*!
 * @brief Checa se chegou ao final do arq.
 * @param f Arquivo a ser checado.
 * @return 1 se ainda há dados, 0 se chegou no final.
 */
char check_eof(FILE* f);

/*!
 * @brief Lê o cabeçalho do arq CSV.
 * @param f Arquivo CSV aberto para leitura.
 * @return Head com os valores iniciais do cabeçalho.
 */
Head ler_header_csv(FILE* f);

/*!
 * @brief Lê um registro do arq CSV.
 * @param f Arquivo CSV aberto para leitura.
 * @return Reg com os dados lidos da linha.
 */
Reg ler_reg_csv(FILE* f);