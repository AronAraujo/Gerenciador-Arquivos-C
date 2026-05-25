#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_bin.h"

// Função para ler 1 registro do arquivo binário
int ler_reg_bin(FILE* f, Reg* r) {
    if (fread(&r->removido, sizeof(char), 1, f) != 1) return 0; // tenta ler o campo removido, se não conseguir, sinaliza que chegou ao fim do arquivo

    //lê os campos inteiros
    fread(&r->proximo, sizeof(int), 1, f);
    fread(&r->codEstacao, sizeof(int), 1, f);
    fread(&r->codLinha, sizeof(int), 1, f);
    fread(&r->codProxEstacao, sizeof(int), 1, f);
    fread(&r->distProxEstacao, sizeof(int), 1, f);
    fread(&r->codLinhaIntegra, sizeof(int), 1, f);
    fread(&r->codEstIntegra, sizeof(int), 1, f);

    int bytes_lidos = 29; // contabiliza a leitura de 29 bytes (1 de controle + 28 de registros)

    // Lendo nomeEstacao

    // Para campos de tamanho variável segue o seguinte fluxo (que se aplica para a leitura do nomeLinha abaixo)
    fread(&r->tamNomeEstacao, sizeof(int), 1, f); // lê o indicador de tamanho do campo
    bytes_lidos += 4; // soma os 4 bytes lidos do indicador de tamanho
    
    // caso o tamanho a ser lido seja maior que 0, malloca com o tamanho do campo + 1 byte pro terminador
    // caso o tamanho a ser lido seja 0, o ponteiro da string na estutura vira null
    if (r->tamNomeEstacao > 0) {
        r->nomeEstacao = malloc(r->tamNomeEstacao + 1);
        fread(r->nomeEstacao, sizeof(char), r->tamNomeEstacao, f);
        r->nomeEstacao[r->tamNomeEstacao] = '\0';
        bytes_lidos += r->tamNomeEstacao;
    } else {
        r->nomeEstacao = NULL;
    }

    // Lendo nomeLinha
    fread(&r->tamNomeLinha, sizeof(int), 1, f);
    bytes_lidos += 4;
    if (r->tamNomeLinha > 0) {
        r->nomeLinha = malloc(r->tamNomeLinha + 1);
        fread(r->nomeLinha, sizeof(char), r->tamNomeLinha, f);
        r->nomeLinha[r->tamNomeLinha] = '\0';
        bytes_lidos += r->tamNomeLinha;
    } else {
        r->nomeLinha = NULL;
    }

    // Pula o lixo ($) até completar os 80 bytes certinho
    fseek(f, 80 - bytes_lidos, SEEK_CUR);

    return 1;
}

// Imprime os dados conforme a especificação do PDF
void imprimir_reg(Reg* r) {
    if (r->removido == '1') return;

    if (r->codEstacao == -1) printf("NULO ");
    else printf("%d ", r->codEstacao);

    if (r->tamNomeEstacao == 0 || r->nomeEstacao == NULL) printf("NULO ");
    else printf("%s ", r->nomeEstacao);

    if (r->codLinha == -1) printf("NULO ");
    else printf("%d ", r->codLinha);

    if (r->tamNomeLinha == 0 || r->nomeLinha == NULL) printf("NULO ");
    else printf("%s ", r->nomeLinha);

    if (r->codProxEstacao == -1) printf("NULO ");
    else printf("%d ", r->codProxEstacao);

    if (r->distProxEstacao == -1) printf("NULO ");
    else printf("%d ", r->distProxEstacao);

    if (r->codLinhaIntegra == -1) printf("NULO ");
    else printf("%d ", r->codLinhaIntegra);

    if (r->codEstIntegra == -1) printf("NULO\n");
    else printf("%d\n", r->codEstIntegra);
}