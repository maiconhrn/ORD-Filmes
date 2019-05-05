//
// Created by Maicon on 13/04/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

/* Acadêmico: Maicon Henrique Rodrigues do Nascimento
 * RA: 102504
 *
 * Sistema desenvolvido baseado em SO Microsoft Windows.
 *
 * Arquivos de dados e índices são armazenados no mesmo
 * diretório do executável da aplicação.
 *
 * O arquivo de dados tem um cabeçalho contendo 8 bytes.
 * Os 4 primeiros bytes armazenam um inteiro representando
 * a cabeça da LED.
 * Os outro 4 bytes armazenam um inteiro representando o
 * número de registros que o arquivo possui.
 * Os registros são precedidos por um short int contendo 2 bytes
 * representando seu tamanho em bytes.
 *
 * O arquivo de índices tem um cabeçalho contendo 4 bytes,
 * representando o numero de registros armazenados pelo
 * arquivo de dados.
 * Ele armazenas estruturas de tamanho fixo que fazem
 * referencia a registros armazenados no arquivo de dados.
 * Um inteiro (4 bytes) contendo a chave do registro
 * e um inteiro (4 bytes) contendo o byte offset do registro no
 * arquivo de dados.
 */
int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-i") == 0) {
        printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);
        if (import(argv[2])) {
            printf("Importacao executada com sucesso!\n");
        }
    } else if (strcmp(argv[1], "-e") == 0) {
        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        performOperation(argv[2]);
    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
}