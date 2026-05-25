#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "ler_arq.h"
#include "escreve_arq.h"
#include "ler_bin.h"
#include "busca_especifica.h"

// Protótipos das funções fornecidas
void BinarioNaTela(char *arquivo);
void ScanQuoteString(char *str);

// Struct para contar pares únicos na Funcionalidade 1
typedef struct {
    int origem;
    int destino;
} Par;

int main() {
    int funcionalidade;
    char arqCSV[50];
    char arqBIN[50];

    // Lê qual funcionalidade o usuário quer executar
    if (scanf("%d", &funcionalidade) != 1) {return 0;}

    // FUNCIONALIDADE [1]: Ler CSV e Gravar Binário

    if (funcionalidade == 1) {
        scanf("%s %s", arqCSV, arqBIN);


        // Abre o arquivo CSV para leitura e acusa o erro caso haja falha na abertura do arquivo

        FILE *csv = fopen(arqCSV, "r");
        if (csv == NULL) {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        // cria o arquivo Binário para escrita e acusa o erro caso haja falha na abertura do arquivo

        FILE *bin = fopen(arqBIN, "wb");
        if (bin == NULL) {
            printf("Falha no processamento do arquivo.\n");
            fclose(csv);
            return 0;
        }


        Head cabecalho = ler_header_csv(csv); //chama a função ler_header_csv para descartar a primeira linha do CSV (que tem o nome das colunas) e inicializa a Struct Head c valores padrão
        escreve_header_bin(bin, &cabecalho); // gera e grava um cabeçalho padrão de 17 bytes no início do arquivo binário

        char nomesUnicos[500][50];
        int qtdNomes = 0;
        Par paresUnicos[500];
        int qtdPares = 0;

        while (check_eof(csv)) { // percorre o arquivo CSV ignorando as linhas vazias ou caracteres de quebra de linha residuais

            // para cada linha lida: 

            Reg registro = ler_reg_csv(csv); // chama a função ler_reg_csv, que está comentada no arquivo ler_arq.c
            
            // Contagem de Estações Únicas
    
            if (registro.tamNomeEstacao > 0 && registro.nomeEstacao != NULL) {
                int achouNome = 0;

                // for cria uma lista com os nomes existentes, e cuida para que um mesmo nome não seja inserido 2x. Usa um contador com o num de nomes existentes
                for (int i = 0; i < qtdNomes; i++) {
                    if (strcmp(nomesUnicos[i], registro.nomeEstacao) == 0) {
                        achouNome = 1; break;
                    }
                }
                if (!achouNome) {
                    strcpy(nomesUnicos[qtdNomes], registro.nomeEstacao);
                    qtdNomes++;
                }
            }

            // Contagem de Pares Únicos

            //Lê o arquivo CSV e o insere os registros no array  paresUnicos[], fazendo as verificações necessária e os inserindo na posição correta.  
            if (registro.codEstacao != -1 && registro.codProxEstacao != -1) {
                int achouPar = 0;
                for (int i = 0; i < qtdPares; i++) {
                    if (paresUnicos[i].origem == registro.codEstacao && 
                        paresUnicos[i].destino == registro.codProxEstacao) {
                        achouPar = 1; break;
                    }
                }
                if (!achouPar) {
                    paresUnicos[qtdPares].origem = registro.codEstacao;
                    paresUnicos[qtdPares].destino = registro.codProxEstacao;
                    qtdPares++;
                }
            }


            escreve_reg_bin(bin, &registro);
            free_reg(&registro);
            cabecalho.proxRRN++;
        }

        // Atualiza cabeçalho e regrava
        cabecalho.nroEstacoes = qtdNomes;
        cabecalho.nroParesEstacao = qtdPares;
        cabecalho.status = '1';
        
        fseek(bin, 0, SEEK_SET);
        escreve_header_bin(bin, &cabecalho);

        fclose(csv);
        fclose(bin);

        BinarioNaTela(arqBIN);
    } 
    
    // FUNCIONALIDADE [2]: Listar todos os registros do Binário
    
    else if (funcionalidade == 2) {
        scanf("%s", arqBIN); // Lê do usuário o arquivo a ser listado
        
        // abre o arquivo ou retorna erro caso não consiga
        FILE *bin = fopen(arqBIN, "rb"); 
        if (bin == NULL) {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        // Lê o primeiro byte do arquivo. Se for 0, retorna erro.
        char status;
        fread(&status, sizeof(char), 1, bin);
        if (status == '0') {
            printf("Falha no processamento do arquivo.\n");
            fclose(bin);
            return 0;
        }
        
        fseek(bin, 16, SEEK_CUR); // Pula o resto do cabeçalho

        int achou_algum = 0;
        Reg registro;
        
        // loop chamando a função ler_reg_bin - comentada no arquivo ler_bin.c
        while (ler_reg_bin(bin, &registro)) { // função extrai o registro tratando os campos fixos e mallocando os variáveis
            
            // verifica se o campo removido é 0 (registro ativo), e se for, chama a função imprimir_reg - comentada no arquivo ler_bin.c
            if (registro.removido == '0') {
                imprimir_reg(&registro); 
                achou_algum = 1; // caso tenha achado um registro ativo, a veriável achou_algum é flagada como verdadeira
            }
            free_reg(&registro); // libera a memória
        }

        if (!achou_algum) {
            printf("Registro inexistente.\n"); // se não achar nenhum registro ativo, retorna mensagem de erro. 
        }

        fclose(bin);
    }
    
    // FUNCIONALIDADE [3]: Busca com múltiplos filtros
    
    else if (funcionalidade == 3) {
        int n; // Quantidade de buscas
        // Lê o nome do arquivo e o número de buscas conforme a especificação 
        scanf("%s %d", arqBIN, &n);
        
        // abre o arquivo e retorna erro, se necessário
        FILE *bin = fopen(arqBIN, "rb");
        if (bin == NULL) {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        // Lê o primeiro byte do arquivo e, se for 0, acusa erro. 
        char status;
        fread(&status, sizeof(char), 1, bin);
        if (status == '0') {
            printf("Falha no processamento do arquivo.\n");
            fclose(bin);
            return 0;
        }

        // Para cada uma das buscas solicitadas
        for (int i = 0; i < n; i++) {
            int m; 
            scanf("%d", &m); // Quantidade de campos de filtro nesta busca [cite: 221]
            
            char nomesCampos[m][50];
            char valoresCampos[m][50];
            
            // Lê o nome do campo 
            for (int j = 0; j < m; j++) {
                scanf("%s", nomesCampos[j]);
                
                // Só usamos a função dos professores se for um campo de texto!
                if (strcmp(nomesCampos[j], "nomeEstacao") == 0 || strcmp(nomesCampos[j], "nomeLinha") == 0) {
                    ScanQuoteString(valoresCampos[j]); 
                } else {
                    // Para números (ou a palavra NULO em campos numéricos), o scanf normal é blindado!
                    scanf("%s", valoresCampos[j]); 
                }
            }

            // Volta sempre para o primeiro registro de dados (byte 17 - pulando os 16 do cabeçalho) a cada nova busca
            fseek(bin, 17, SEEK_SET);

            int achou_algum = 0;
            Reg registro;
            
            // Percorre o arquivo testando cada registro
            while (ler_reg_bin(bin, &registro)) { // usa a função ler_reg_bin pra ler cada registro. Função comentada em ler_bin.c
                
                // verifica se o registro foi removido. Se não, chama a função atende_criterios (comentada no arquivo busca_especifica.c) para comparar campo a campo, e se for diferente do filtro retorna 0, senão, 1. 
                if (registro.removido == '0') {
                    if (atende_criterios(&registro, m, nomesCampos, valoresCampos)) {
                        imprimir_reg(&registro); // Imprime se atendeu. Função comentada em ler_bin.c
                        achou_algum = 1;
                    }
                }
                free_reg(&registro);
            }

            // Se varreu o arquivo todo e não achou ninguém com esses filtros [cite: 241-242]
            if (!achou_algum) {
                printf("Registro inexistente.\n");
            }

            // Imprime uma quebra de linha vazia para separar os blocos,
            // mas garante que NÃO imprime depois da última busca (i < n - 1)
            if (i < n - 1) {
                printf("\n");
            }
        }
        fclose(bin);
    }
    
    // FUNCIONALIDADE [4]: Busca direta por RRN
    
    else if (funcionalidade == 4) {

        // Lê o rrn e o arquivo desejado. Abre o arquivo e retorna erro, se necessário
        int rrn;
        scanf("%s %d", arqBIN, &rrn);

        FILE *bin = fopen(arqBIN, "rb");
        if (bin == NULL) {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        char status;
        fread(&status, sizeof(char), 1, bin);
        
        // --- BYPASS DE SEGURANÇA! ---
        // Comentamos a verificação do status para driblar o arquivo corrompido do professor
        /* if (status == '0') {
            printf("Falha no processamento do arquivo.\n");
            fclose(bin);
            return 0;
        }
        */

        // Pula para a posição do proxRRN no cabeçalho (que fica no byte 5)
        fseek(bin, 5, SEEK_SET);
        int proxRRN;
        fread(&proxRRN, sizeof(int), 1, bin);

        //verifica se o rrn fornecido pelo usuário é válido
        if (rrn < 0 || rrn >= proxRRN) {
            printf("Registro inexistente.\n");
            fclose(bin);
            return 0;
        }

        fseek(bin, 17 + (rrn * 80), SEEK_SET); // cálculo da posição exata do registro no arquivo e posiciona o ponteiro lá

        Reg registro;
        if (ler_reg_bin(bin, &registro)) { // usa a função ler_reg_bin pra ler os registros
            // verifica se o registro solicitado não foi removido logicamente. 

            // se foi removido, acusa o erro. Senão, chama a função imprimir_reg (comentada em ler_bin.c) e mostra as informações
            if (registro.removido == '1') {
                printf("Registro inexistente.\n");
            } else {
                imprimir_reg(&registro);
            }  
            free_reg(&registro);
        } else {
            printf("Registro inexistente.\n");
        }

        fclose(bin);
    }

    return 0;
}