//
// Created by Maicon on 13/04/2019.
//

#ifndef FILMES_MOVIE_H
#define FILMES_MOVIE_H

#include <stdio.h>
#include "bool.h"

static short id = 0;

typedef struct {
    int id;
    char title[150];
    char gender[150];
    char releaseDate[150];
    char durationTime[150];
    char rating[150];
} Movie;

int importFromFile(FILE *file, Movie **movies);

Bool movieToRegStr(Movie movie, char *regStr);

FILE *exportToBinaryFile(Movie **movies, int size);

#endif //FILMES_MOVIE_H
