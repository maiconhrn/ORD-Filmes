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
    movie.title = strtok(NULL, token);
    movie.gender = strtok(NULL, token);
    movie.releaseDate = strtok(NULL, token);
    movie.durationTime = strtok(NULL, token);
    movie.rating = strtok(NULL, token);

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
