#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void print_reg(const Reg* r) {
}

void free_reg(Reg* r) {
    if (r->nomeEstacao) { free(r->nomeEstacao); r->nomeEstacao = NULL; }
    if (r->nomeLinha)   { free(r->nomeLinha);   r->nomeLinha   = NULL; }
}

void free_header(Head* h) {
    /* header não tem ponteiros, nada a fazer */
}