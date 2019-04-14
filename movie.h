//
// Created by Maicon on 13/04/2019.
//

#ifndef FILMES_MOVIE_H
#define FILMES_MOVIE_H

#include <stdio.h>
#include "bool.h"

typedef struct {
    int id;
    char *title;
    char *gender;
    char *releaseDate;
    char *durationTime;
    char *rating;
} Movie;

int importFromFile(FILE *file, Movie **movies);

Bool movieToRegStr(Movie movie, char regStr) {
    
}

#endif //FILMES_MOVIE_H
