//
// Created by Maicon on 13/04/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "commands.h"
#include "movie.h"
#include "keyoffset.h"

KeyOffset *keyOffsetArr;
int numRegs;

Bool import(char *fileName) {
    FILE *sourceFile = fopen(fileName, "r");
    Movie *movies;
    if (sourceFile != NULL) {
        numRegs = importMoviesFromTxtFile(sourceFile, &movies);
        if (numRegs > 0) {
            exportMoviesToBinaryFile(movies, numRegs);
            fclose(sourceFile);
            free(movies);
            return true;
        }
    } else {
        fprintf(stderr, "Nao e possivel ler o arquivo \"%s\"\n", fileName);
    }

    fclose(sourceFile);
    return false;
}

short readRec(char *recbuff, FILE *fd) {
    short rec_lgth;

    if (fread(&rec_lgth, sizeof(rec_lgth), 1, fd) == 0) // get record length
        return 0;

    rec_lgth = fread(recbuff, sizeof(char), rec_lgth, fd); // read record
    recbuff[rec_lgth] = '\0';

    return rec_lgth;
}

int findReg(KeyOffset *keyOffset, char *res) {
    FILE *dataFile = fopen(DATA_FILE_NAME, "rb");
    fseek(dataFile, keyOffset->offset, SEEK_SET);
    int bytesSize = readRec(res, dataFile);
    fclose(dataFile);

    return bytesSize;
}

void findFilme(int key) {
    char reg[500];

    numRegs = importKeyOffsetsFromBinaryFile(&keyOffsetArr);
    sortKeyOffsets(keyOffsetArr, numRegs);
    KeyOffset *keyOffset = findKeyOffset(keyOffsetArr, numRegs, key);
    if (keyOffset != NULL) {
        int bytesSize = findReg(keyOffset, reg);
        printf("%s (%d bytes)\n", reg, bytesSize);
    } else {
        printf("Erro: registro nao encontrado!\n");
    }
}

Bool performOperation(char *fileName) {
    FILE *operations = fopen(fileName, "r");

    char lineStr[500], *operation;
    int key;

    while (fgets(lineStr, 499, operations) != NULL) {
        operation = strtok(lineStr, " \n");

        switch (atoi(operation)) {
            case 1:
                key = atoi(strtok(NULL, " \n"));
                printf("Busca pelo registro de chave \"%d\"\n", key);
                findFilme(key);
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                break;
        }
    }

    return false;
}

