
# 📂 Gerenciador de Banco de Dados em C (Mini-SGBD)

Este projeto é uma implementação em linguagem C de um sistema de gerenciamento de dados baseado em arquivos. O objetivo do projeto é realizar operações de leitura, gravação e busca de registros relacionados às estações de metrô e trens (CPTM) de São Paulo, simulando o comportamento de um Banco de Dados Relacional (SGBD) por debaixo dos panos.

O código foi aprovado com nota máxima (100%) nos testes automatizados da disciplina de Algoritmos e Estruturas de Dados II, lidando com alocação dinâmica de memória, manipulação de arquivos em nível de byte e otimização de buscas.

## 🚀 Funcionalidades Implementadas

O sistema atua em 4 frentes principais, análogas a comandos da linguagem SQL:

* **[1] Parser e Gravação Binária (`CREATE TABLE` + `INSERT`):** Lê uma base de dados bruta de um arquivo `.csv`, trata campos nulos/vazios e converte os dados para um arquivo binário (`.bin`). A gravação garante que cada registro tenha exatamente **80 bytes** fixos, preenchendo os espaços sobressalentes com lixo de memória (`$`) e controlando o *byte offset* no cabeçalho (17 bytes).
* **[2] Recuperação de Dados (`SELECT *`):** Lê o arquivo binário gerado, pulando o cabeçalho e reconstruindo dinamicamente as structs em memória para listar todos os registros ativos, saltando corretamente o lixo de memória com `fseek`.
* **[3] Busca Sequencial com Filtros Múltiplos (`SELECT ... WHERE`):** Permite buscar registros específicos passando um ou mais campos como critério (ex: buscar todas as estações da linha "Azul"). Lida com conversão segura de strings com aspas e números.
* **[4] Busca Direta O(1) por RRN:** A funcionalidade mais otimizada do projeto. Permite buscar um registro diretamente pelo seu Número Relativo de Registro (RRN). Como o tamanho do registro é fixo, o programa calcula o deslocamento exato no disco (`17 + (RRN * 80)`) e acessa o dado instantaneamente em complexidade **O(1)**, sem precisar ler o arquivo todo.

## 🛠️ Tecnologias e Conceitos Aplicados

* **Linguagem C:** Manipulação de ponteiros e alocação dinâmica (`malloc`, `free`).
* **File I/O:** Manipulação de arquivos em modo leitura/escrita de binários (`fread`, `fwrite`, `fseek`).
* **Estruturas de Dados:** Uso de `structs` para modelagem do cabeçalho e dos registros.
* **Arquitetura:** Código totalmente modularizado em múltiplos arquivos `.c` e `.h` para separação de responsabilidades.
* **Automação:** Uso de `Makefile` para rotinas de compilação.

## ⚙️ Como executar o projeto

Certifique-se de ter o compilador `gcc` e o `make` instalados na sua máquina.

1. Clone este repositório.
2. Compile o código usando o Makefile:
   ```bash
   make all
