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

int importMoviesFromTxtFile(FILE *file, Movie **movies) {
    char lineStr[500];

    *movies = (Movie *) malloc(500 * sizeof(Movie));

    int i = 0;
    while (fgets(lineStr, 499, file) != NULL) {
        (*movies)[i] = convertFromStr(lineStr);
        i++;
    }

    return i;
}

Bool movieToRegStr(Movie movie, char *regStr) {
    char id[11];
    itoa(movie.id, id, 10);

    strcpy(regStr, id);
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

Bool exportMoviesToBinaryFile(Movie *movies, int size) {
    char reg[500];
    FILE *dataFile = fopen(DATA_FILE_NAME, "wb");
    int regOffset = sizeof(int);
    KeyOffset *keyOffsetArr = (KeyOffset *) malloc(size * sizeof(KeyOffset));

//    fwrite("|", 1 * sizeof(char), 1, dataFile);
    fwrite(&size, sizeof(int), 1, dataFile);
    for (int i = 0; i < size; ++i) {
        movieToRegStr(movies[i], reg);
        short regSize = (short) strlen(reg);

        keyOffsetArr[i].key = movies[i].id;
        keyOffsetArr[i].offset = regOffset;

        regOffset += regSize + sizeof(regSize);

        fwrite(&regSize, sizeof(short), 1, dataFile);
        fwrite(reg, strlen(reg) * sizeof(char), 1, dataFile);
    }
    fwrite("|", 1 * sizeof(char), 1, dataFile);

    exportKeyOffsetsToBinaryFile(keyOffsetArr, size);

    fclose(dataFile);

    return true;
}
