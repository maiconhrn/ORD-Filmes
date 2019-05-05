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
    printf("\n");

    Movie *movies;
    numRegs = importMoviesFromTxtFile(fileName, &movies);
    if (numRegs > 0) {
        exportMoviesToBinaryFile(movies, numRegs);
        free(movies);
        return true;
    }

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

void findMovie(int key) {
    char reg[500];

    printf("Busca pelo registro de chave \"%d\"\n", key);

    numRegs = importKeyOffsetsFromBinaryFile(&keyOffsetArr);
    KeyOffset *keyOffset = findKeyOffset(keyOffsetArr, numRegs, key);
    if (keyOffset != NULL) {
        int bytesSize = findReg(keyOffset, reg);
        printf("%s (%d bytes)\n", reg, bytesSize);
    } else {
        printf("Erro: registro nao encontrado!\n");
    }
}

void insertMovie(char *movieStr) {
    insertMovieToBinaryFyle(movieStr);
}

void removeMovie(int key) {
    removeMovieFromBinaryFyle(key);
}

Bool performOperation(char *fileName) {
    FILE *operations = fopen(fileName, "r");

    char lineStr[500], *operation, *movieStr;
    int key;

    while (fgets(lineStr, 499, operations) != NULL) {
        operation = strtok(lineStr, " \n");

        printf("\n");

        switch (atoi(operation)) {
            case 1:
                key = atoi(strtok(NULL, " \n"));
                findMovie(key);
                break;
            case 2:
                movieStr = strtok(NULL, "\n");
                insertMovie(movieStr);
                break;
            case 3:
                key = atoi(strtok(NULL, " \n"));
                removeMovie(key);
                break;
            default:
                break;
        }
    }

    fclose(operations);

    return false;
}

