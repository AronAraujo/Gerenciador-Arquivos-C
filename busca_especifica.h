#pragma once
#include "structs.h"

/*!
 * @brief Verifica se um registro atende aos critérios de busca especificados.
 * @return 1 se atende a todos os critérios, 0 caso contrário.
 */
int atende_criterios(Reg* r, int m, char nomesCampos[][50], char valoresCampos[][50]);