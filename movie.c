//
// Created by Maicon on 13/04/2019.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "movie.h"

Movie convertFromStr(char *str) {
    Movie movie;
    char token[] = ";\n";

    movie.id = atoi(strtok(str, token));
    strcpy(movie.title, strtok(NULL, token));
    strcpy(movie.gender, strtok(NULL, token));
    strcpy(movie.releaseDate, strtok(NULL, token));
    strcpy(movie.durationTime, strtok(NULL, token));
    strcpy(movie.rating, strtok(NULL, token));

    return movie;
}

int importFromFile(FILE *file, Movie **movies) {
    char lineStr[500];

    int i = 0;
    while (fgets(lineStr, 499, file) != NULL) {
        (*movies)[i] = convertFromStr(lineStr);
        i++;
    }

    return i;
}

Bool movieToRegStr(Movie movie, char *regStr) {
    char idStr[10];
    itoa(id++, idStr, 10);

    if (id == 1) {
        regStr[0] = '|';
    } else {
        regStr[0] = '\0';
    }

    strcat(regStr, idStr);
    strcat(regStr, "|");
    strcat(regStr, movie.title);
    strcat(regStr, "|");
    strcat(regStr, movie.gender);
    strcat(regStr, "|");
    strcat(regStr, movie.releaseDate);
    strcat(regStr, "|");
    strcat(regStr, movie.durationTime);
    strcat(regStr, "|");
    strcat(regStr, movie.rating);
    strcat(regStr, "|");

    return true;
}

FILE *exportToBinaryFile(Movie **movies, int size) {
    char reg[500];
    FILE *file = fopen("dados.dat", "wb");

    for (int i = 0; i < size; ++i) {
        movieToRegStr((*movies)[i], reg);
        fwrite(reg, 1, strlen(reg) * sizeof(char), file);
    }

    fclose(file);

    file = fopen("dados.dat", "rb");

    return file;
}
