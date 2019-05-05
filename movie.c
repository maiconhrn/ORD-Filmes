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

int importMoviesFromTxtFile(char *fileName, Movie **movies) {
    FILE *sourceFile = fopen(fileName, "r");
    char lineStr[500];
    int i = 0;

    if (sourceFile != NULL) {
        *movies = (Movie *) malloc(500 * sizeof(Movie));

        while (fgets(lineStr, 499, sourceFile) != NULL) {
            (*movies)[i] = convertFromMovieStr(lineStr);
            i++;
        }

        fclose(sourceFile);
    } else {
        fprintf(stderr, "Nao e possivel ler o arquivo \"%s\"\n", fileName);
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

    sortKeyOffsetsByKey(keyOffsetArr, size);

    exportKeyOffsetsToBinaryFile(keyOffsetArr, size);

    fclose(dataFile);

    return true;
}

int findAvaliableOffsetToInsert(FILE *dataFile, short sizeRequested,
                                KeyOffset **keyOffsetArr, int *numReg) {
    int ledHead, prevOffset, offset, nextOffset, size;
    short regSize;

    size = importKeyOffsetsFromBinaryFile(keyOffsetArr);

    *numReg = size;

    fseek(dataFile, 0, SEEK_SET);
    fread(&ledHead, sizeof(int), 1, dataFile);

    sortKeyOffsetsByOffset(*keyOffsetArr, size);

    if (ledHead == -1) {
        fseek(dataFile, (*keyOffsetArr)[size - 1].offset, SEEK_SET);
        fread(&regSize, sizeof(short), 1, dataFile);
        return (*keyOffsetArr)[size - 1].offset + regSize + sizeof(short);
    } else {
        offset = ledHead;
        prevOffset = offset;
        do {
            fseek(dataFile, offset, SEEK_SET);
            fread(&regSize, sizeof(short), 1, dataFile);

            if (regSize > sizeof(short) + 1 && regSize == sizeRequested) {
                fseek(dataFile, 1, SEEK_CUR);
                fread(&nextOffset, sizeof(int), 1, dataFile);

                if (prevOffset != ledHead) {
                    fseek(dataFile, prevOffset + sizeof(short), SEEK_SET);
                    fwrite(&nextOffset, sizeof(int), 1, dataFile);
                } else {
                    fseek(dataFile, 0, SEEK_SET);
                    fwrite(&nextOffset, sizeof(int), 1, dataFile);
                }

//                return offset + regSize + sizeof(short);
                return offset;
            } else if (regSize > sizeof(short) + 1 && regSize > sizeRequested) {
                short newBytesSize = regSize - sizeRequested - sizeof(short);
                fseek(dataFile, -2, SEEK_CUR);
                fwrite(&newBytesSize, sizeof(short), 1, dataFile);

                return offset + newBytesSize + sizeof(short);
//                return offset;
            } else {
                prevOffset = offset;

                fseek(dataFile, 1, SEEK_CUR);
                fread(&offset, sizeof(int), 1, dataFile);
            }
        } while (offset != -1);
    }

    return -1;
}

Bool insertMovieToBinaryFyle(char *movieStr) {

    FILE *dataFile = fopen(DATA_FILE_NAME, "r+b");

    Movie movie = convertFromMovieStr(movieStr);

    char reg[500];
    movieToRegStr(movie, reg);

    int numReg, avaliableOffset;

    short regSize = (short) strlen(reg);

    printf("Insercao do registro de chave \"%d\" (%d bytes)\n", movie.id, regSize);

    KeyOffset *keyOffsetArr, keyOffset;

    if (dataFile != NULL) {

        avaliableOffset = findAvaliableOffsetToInsert(dataFile, regSize, &keyOffsetArr, &numReg);

        keyOffset.key = movie.id;
        keyOffset.offset = avaliableOffset;
        numReg = addKeyOffset(&keyOffsetArr, numReg, &keyOffset);

        fseek(dataFile, avaliableOffset, SEEK_SET);
        fwrite(&regSize, sizeof(short), 1, dataFile);
        fwrite(reg, regSize * sizeof(char), 1, dataFile);


        if (avaliableOffset >= keyOffsetArr[numReg - 2].offset) {
            printf("Local: fim do arquivo\n");
        } else {
            printf("Local: offset = %d bytes (%x)\n", avaliableOffset, avaliableOffset);
            printf("Tamanho do espaco reutilizado: %d bytes\n", regSize);
        }

        sortKeyOffsetsByKey(keyOffsetArr, numReg);
        exportKeyOffsetsToBinaryFile(keyOffsetArr, numReg);

        fclose(dataFile);
    }

    return true;
}

Bool removeMovieFromBinaryFyle(int key) {
    FILE *dataFile = fopen(DATA_FILE_NAME, "r+b");
    int size, ledHead;
    short regSize;
    KeyOffset *keyOffsetArr, *keyOffset;

    printf("Remocao do registro de chave \"%d\"\n", key);

    size = importKeyOffsetsFromBinaryFile(&keyOffsetArr);
    sortKeyOffsetsByKey(keyOffsetArr, size);
    keyOffset = findKeyOffset(keyOffsetArr, size, key);

    if (keyOffset != NULL) {
        fread(&ledHead, sizeof(int), 1, dataFile);

        fseek(dataFile, keyOffset->offset, SEEK_SET);
        fread(&regSize, sizeof(short), 1, dataFile);

        fseek(dataFile, 0, SEEK_CUR);
        fwrite("*", sizeof(char), 1, dataFile);
        fwrite(&ledHead, sizeof(int), 1, dataFile);

        fseek(dataFile, 0, SEEK_SET);
        fwrite(&(keyOffset->offset), sizeof(int), 1, dataFile);

        printf("Registro removido! (%d bytes)\n", regSize);
        printf("Posicao: offset = %d bytes\n", keyOffset->offset);

        sortKeyOffsetsByKey(keyOffsetArr, size);
        size = removeKeyOffset(&keyOffsetArr, size, keyOffset);

        exportKeyOffsetsToBinaryFile(keyOffsetArr, size);

    } else {
        printf("Erro: registro nao encontrado!\n");
    }

    fclose(dataFile);

    return true;
}
