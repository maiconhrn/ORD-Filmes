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
    FILE *sourceFile = fopen(fileName, "r"),
            *dataFile;
    Movie *movies = (Movie *) malloc(500 * sizeof(Movie));;
    if (sourceFile != NULL) {
        int regNum = importFromFile(sourceFile, &movies);
        if (regNum > 0) {
            dataFile = exportToBinaryFile(&movies, regNum);

            fclose(dataFile);
            fclose(sourceFile);
            free(movies);
            return true;
        }
    } else {
        fprintf(stderr, "Nao e possivel ler o arquivo \"%s\"\n", fileName);
    }

    free(movies);
    fclose(sourceFile);
    return false;
}

