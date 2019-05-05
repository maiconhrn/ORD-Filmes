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

Bool import(char *fileName) {
    printf("\n");

    int numRegs;
    Movie *movies;
    numRegs = importMoviesFromTxtFile(fileName, &movies);
    if (numRegs > 0) {
        exportMoviesToBinaryFile(movies, numRegs);
        free(movies);
        return true;
    }

    return false;
}

void findMovie(int key) {
    char reg[500];

    if (findMovieInBinaryFile(key, reg)) {
        printf("%s (%d bytes)\n", reg, strlen(reg));
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

void performOperation(char *fileName) {
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
}

