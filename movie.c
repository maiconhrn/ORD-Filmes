//
// Created by Maicon on 13/04/2019.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "movie.h"

Movie convertFromMovieStr(char *str) {
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
        (*movies)[i] = convertFromMovieStr(lineStr);
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
    int regOffset = 2 * sizeof(int), ledHead = -1;
    KeyOffset *keyOffsetArr = (KeyOffset *) malloc(size * sizeof(KeyOffset));

//    fwrite("|", 1 * sizeof(char), 1, dataFile);
    fwrite(&ledHead, sizeof(int), 1, dataFile);
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
//    fwrite("|", 1 * sizeof(char), 1, dataFile);

    sortKeyOffsetsByKey(keyOffsetArr, size);

    exportKeyOffsetsToBinaryFile(keyOffsetArr, size);

    fclose(dataFile);

    return true;
}

Bool insertMovieToBinaryFyle(char *movieStr) {
    FILE *dataFile = fopen(DATA_FILE_NAME, "r+b");

    Movie movie = convertFromMovieStr(movieStr);

    char reg[500];

    movieToRegStr(movie, reg);

    int ledHead, size;
    short regSize = (short) strlen(reg);

    KeyOffset *keyOffsetArr, keyOffset;
    size = importKeyOffsetsFromBinaryFile(&keyOffsetArr);


    if (dataFile != NULL) {
        fread(&ledHead, sizeof(int), 1, dataFile);
        if (ledHead == -1) {
            sortKeyOffsetsByOffset(keyOffsetArr, size);
            keyOffset.key = movie.id;
            keyOffset.offset = keyOffsetArr[size - 1].offset + regSize * sizeof(char) + sizeof(short);
            keyOffsetArr = (KeyOffset *) realloc(keyOffsetArr, (size + 1) * sizeof(KeyOffset));
            keyOffsetArr[size] = keyOffset;
            size++;

            fseek(dataFile, 0, SEEK_END);
            fwrite(&regSize, sizeof(short), 1, dataFile);
            fwrite(reg, regSize * sizeof(char), 1, dataFile);

            sortKeyOffsetsByKey(keyOffsetArr, size);
            exportKeyOffsetsToBinaryFile(keyOffsetArr, size);
        }

        fclose(dataFile);
    }

    return true;
}
