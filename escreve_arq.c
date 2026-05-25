#include <stdio.h>
#include <string.h>
#include "escreve_arq.h"

/*!
 * @brief Grava o Cabeçalho. Ele tem tamanho fixo de 17 bytes.
 */
void escreve_header_bin(FILE* f, Head* h) {
    // A especificação diz que devemos escrever campo a campo!
    fwrite(&h->status, sizeof(char), 1, f);
    fwrite(&h->topo, sizeof(int), 1, f);
    fwrite(&h->proxRRN, sizeof(int), 1, f);
    fwrite(&h->nroEstacoes, sizeof(int), 1, f);
    fwrite(&h->nroParesEstacao, sizeof(int), 1, f);
}

/*!
 * @brief Grava o Registro de Dados. Deve ter exatamente 80 bytes.
 */
void escreve_reg_bin(FILE* f, Reg* r) {
    int bytes_escritos = 0;

    // 1. Campos de controle e tamanho fixo
    fwrite(&r->removido, sizeof(char), 1, f); bytes_escritos += 1;
    fwrite(&r->proximo, sizeof(int), 1, f); bytes_escritos += 4;
    fwrite(&r->codEstacao, sizeof(int), 1, f); bytes_escritos += 4;
    fwrite(&r->codLinha, sizeof(int), 1, f); bytes_escritos += 4;
    fwrite(&r->codProxEstacao, sizeof(int), 1, f); bytes_escritos += 4;
    fwrite(&r->distProxEstacao, sizeof(int), 1, f); bytes_escritos += 4;
    fwrite(&r->codLinhaIntegra, sizeof(int), 1, f); bytes_escritos += 4;
    fwrite(&r->codEstIntegra, sizeof(int), 1, f); bytes_escritos += 4;

    // 2. Campo variável: Nome da Estação (Indicador de tamanho + String)
    fwrite(&r->tamNomeEstacao, sizeof(int), 1, f); bytes_escritos += 4;
    if (r->tamNomeEstacao > 0 && r->nomeEstacao != NULL) {
        // Gravamos a string SEM o '\0', logo usamos o tamanho exato da string
        fwrite(r->nomeEstacao, sizeof(char), r->tamNomeEstacao, f);
        bytes_escritos += r->tamNomeEstacao;
    }

    // 3. Campo variável: Nome da Linha (Indicador de tamanho + String)
    fwrite(&r->tamNomeLinha, sizeof(int), 1, f); bytes_escritos += 4;
    if (r->tamNomeLinha > 0 && r->nomeLinha != NULL) {
        // Gravamos a string SEM o '\0'
        fwrite(r->nomeLinha, sizeof(char), r->tamNomeLinha, f);
        bytes_escritos += r->tamNomeLinha;
    }

    // 4. Preenchimento de Lixo ($) para completar 80 bytes
    char lixo = '$';
    while (bytes_escritos < 80) {
        fwrite(&lixo, sizeof(char), 1, f);
        bytes_escritos++;
    }
}