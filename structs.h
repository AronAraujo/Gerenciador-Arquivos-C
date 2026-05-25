#pragma once

typedef enum status_ {
    OK = '1',
    ERR = '0'
} Status;

/* Cabeçalho - 17 bytes */
typedef struct header_ {
    char status;        /* 1 byte  */
    int topo;           /* 4 bytes */
    int proxRRN;        /* 4 bytes */
    int nroEstacoes;    /* 4 bytes */
    int nroParesEstacao;/* 4 bytes */
} Head;

/* Registro de dados - 80 bytes */
typedef struct reg_ {
    char removido;          /* 1 byte  */
    int proximo;            /* 4 bytes */
    int codEstacao;         /* 4 bytes */
    int codLinha;           /* 4 bytes */
    int codProxEstacao;     /* 4 bytes */
    int distProxEstacao;    /* 4 bytes */
    int codLinhaIntegra;    /* 4 bytes */
    int codEstIntegra;      /* 4 bytes */
    int tamNomeEstacao;     /* 4 bytes */
    int tamNomeLinha;       /* 4 bytes */
    char* nomeEstacao;      /* variável */
    char* nomeLinha;        /* variável */
} Reg;

void print_reg(const Reg* r);
void free_reg(Reg* r);
void free_header(Head* h);