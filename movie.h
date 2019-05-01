//
// Created by Maicon on 13/04/2019.
//

#ifndef FILMES_MOVIE_H
#define FILMES_MOVIE_H

#include <stdio.h>
#include "bool.h"
#include "keyoffset.h"

#define DATA_FILE_NAME "dados.dat"

typedef struct {
    int id;
    char title[150];
    char gender[150];
    char releaseDate[150];
    char durationTime[150];
    char rating[150];
} Movie;

int importMoviesFromTxtFile(FILE *file, Movie **movies);

Bool movieToRegStr(Movie movie, char *regStr);

Bool exportMoviesToBinaryFile(Movie *movies, int size);

#endif //FILMES_MOVIE_H
