#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "busca_especifica.h"

int atende_criterios(Reg* r, int m, char nomesCampos[][50], char valoresCampos[][50]) {
   
    // percorre os m filtros a serem aplicados, e para cada filtro ela:
    for (int i = 0; i < m; i++) {

        // identifica o nome do campo e o valor de busca correspondente 
        char* campo = nomesCampos[i]; 
        char* valorStr = valoresCampos[i];
        
        // Converte o valorStr para inteiro, ou -1 se for "NULO"
        int valorInt = (strcmp(valorStr, "NULO") == 0 || strcmp(valorStr, "") == 0) ? -1 : atoi(valorStr);

        // Compara todos os campos inteiros verificando. Se retornar 0 é pq não atende aos critérios
        if (strcmp(campo, "codEstacao") == 0) {
            if (r->codEstacao != valorInt) return 0;
        } 
        else if (strcmp(campo, "codLinha") == 0) {
            if (r->codLinha != valorInt) return 0;
        } 
        else if (strcmp(campo, "codProxEstacao") == 0) {
            if (r->codProxEstacao != valorInt) return 0;
        } 
        else if (strcmp(campo, "distProxEstacao") == 0) {
            if (r->distProxEstacao != valorInt) return 0;
        } 
        else if (strcmp(campo, "codLinhaIntegra") == 0) {
            if (r->codLinhaIntegra != valorInt) return 0;
        } 
        else if (strcmp(campo, "codEstIntegra") == 0) {
            if (r->codEstIntegra != valorInt) return 0;
        } 

        // verifica os campos de texto

        else if (strcmp(campo, "nomeEstacao") == 0) {

            // Verifica se o usuário está buscando por NULO. Ai vê se o tamanho do registro é maior que zero. Se for, retorna 0
            if (valorInt == -1) { 
                if (r->tamNomeEstacao > 0) return 0;
            
            // Se o usuário está buscando por um campo não nulo, verificamos se o registro:
                // 1 - é nulo - e retorna 0 se for
                // 2 - é compativel com o solicitado. Somente se for, não retorna 0.
            } else {
                if (r->nomeEstacao == NULL || strcmp(r->nomeEstacao, valorStr) != 0) return 0;
            }
        } 
        else if (strcmp(campo, "nomeLinha") == 0) {
            if (valorInt == -1) { // Buscando por NULO
                if (r->tamNomeLinha > 0) return 0;
            } else {
                if (r->nomeLinha == NULL || strcmp(r->nomeLinha, valorStr) != 0) return 0;
            }
        }
    }
    return 1; // Se passou por todos os filtros sem retornar 0, é porque atende!
}