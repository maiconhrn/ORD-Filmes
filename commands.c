//
// Created by Maicon on 13/04/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "commands.h"
#include "movie.h"

Bool import(char *fileName) {
    FILE *file = fopen(fileName, "r");
    Movie *movies = (Movie *) malloc(500 * sizeof(Movie));;
    if (file != NULL) {
        int regNum = importFromFile(file, &movies);
        if (regNum > 0) {


            fclose(file);
            free(movies);
            return true;
        }
    } else {
        fprintf(stderr, "Nao e possivel ler o arquivo \"%s\"\n", fileName);
    }

    free(movies);
    fclose(file);
    return false;
}

