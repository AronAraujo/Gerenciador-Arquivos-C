#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ler_arq.h"
#include "structs.h"

// Checa se o campo está vazio ou se é apenas um "espaço em branco"
int is_null(char* token) {
    if (token == NULL || token[0] == '\0') return 1;
    // Verifica se tem só espaços/quebras de linha (o tal do campo nulo do CSV)
    for (int i = 0; token[i] != '\0'; i++) {
        if (token[i] != ' ' && token[i] != '\r' && token[i] != '\n') return 0;
    }
    return 1;
}

// Avança no texto até achar uma vírgula
char* ler_campo(char** linha) {
    if (*linha == NULL) return NULL;
    char* inicio = *linha;
    char* fim = strchr(inicio, ','); 
    
    if (fim != NULL) {
        *fim = '\0'; 
        *linha = fim + 1; 
    } else {
        *linha = NULL; 
    }
    return inicio;
}

/*!
 * @brief Checa se chegou ao fim do arquivo (PULANDO AS LINHAS FANTASMAS)
 */
char check_eof(FILE* f) {
    char ch = fgetc(f);
    // Vai comendo os Enters vazios no final do arquivo
    while (ch == '\n' || ch == '\r') {
        ch = fgetc(f);
    }
    if (ch == EOF) return 0;
    fseek(f, -1, SEEK_CUR); // Volta 1 char para trás se achou uma letra de verdade
    return 1;
}

/*!
 * @brief Lê o cabeçalho do CSV.
 */
Head ler_header_csv(FILE* f) {
    char buf[300]; 
    if (fgets(buf, 300, f) == NULL) return (Head){0}; // descarta a primeira linha (que tem os nomes das colunas)
    // inicializa uma nova Srtuct c/ valores padrão 
    Head h; 
    h.status = '0';
    h.topo = -1;
    h.proxRRN = 0;
    h.nroEstacoes = 0;
    h.nroParesEstacao = 0;
    return h;
}

/*!
 * @brief Lê um registro do arquivo CSV e aplica a regra dos nulos.
 */
Reg ler_reg_csv(FILE* f) {
    char buf[300];

    //le a linha e armmazena no buffer, dps localiza onde coeça a quera de linha e substitui por caractere nulo
    fgets(buf, 300, f);
    buf[strcspn(buf, "\r\n")] = '\0'; 

    // define o estado dos registros
    Reg r;
    r.removido = '0';
    r.proximo = -1;

    char* ptr = buf; 
    char* token;

// extração de campos

    // 1. codEstacao
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    r.codEstacao = (!is_null(token)) ? atoi(token) : -1; // vê se o campo é nulo. Se tiver dados transforma em inteiro, se estiver vazio recebe -1

    // 2. nomeEstacao
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    
    // como é string, precisa usar malloc calcula o tamanho da string e aloca o tamanho necessário
    if (!is_null(token)) {
        r.nomeEstacao = malloc(strlen(token) + 1);
        strcpy(r.nomeEstacao, token);
        r.tamNomeEstacao = strlen(token);
    } else {
        r.nomeEstacao = NULL;
        r.tamNomeEstacao = 0;
    }

    // 3. codLinha
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    r.codLinha = (!is_null(token)) ? atoi(token) : -1; // vê se o campo é nulo. Se tiver dados transforma em inteiro, se estiver vazio recebe -1

    // 4. nomeLinha
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    // como é string, precisa usar malloc - calcula o tamanho da string e aloca o tamanho necessário
    if (!is_null(token)) {
        r.nomeLinha = malloc(strlen(token) + 1);
        strcpy(r.nomeLinha, token);
        r.tamNomeLinha = strlen(token);
    } else {
        r.nomeLinha = NULL;
        r.tamNomeLinha = 0;
    }

    // 5. codProxEstacao
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    r.codProxEstacao = (!is_null(token)) ? atoi(token) : -1; // vê se o campo é nulo. Se tiver dados transforma em inteiro, se estiver vazio recebe -1

    // 6. distProxEstacao
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    r.distProxEstacao = (!is_null(token)) ? atoi(token) : -1; // vê se o campo é nulo. Se tiver dados transforma em inteiro, se estiver vazio recebe -1

    // 7. codLinhaIntegra
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    r.codLinhaIntegra = (!is_null(token)) ? atoi(token) : -1; // vê se o campo é nulo. Se tiver dados transforma em inteiro, se estiver vazio recebe -1

    // 8. codEstIntegra
    token = ler_campo(&ptr); // chama funução ler_campo usando um ponteiro auxiliar que percorre o buffer
    r.codEstIntegra = (!is_null(token)) ? atoi(token) : -1; // vê se o campo é nulo. Se tiver dados transforma em inteiro, se estiver vazio recebe -1

    return r;
}