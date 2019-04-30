//
// Created by Maicon on 27/04/2019.
//

#ifndef FILMES_KEYOFFSET_H
#define FILMES_KEYOFFSET_H

#include <stdio.h>

#define INDEX_FILE_NAME "index.dat"

typedef struct {
    int key;
    int offset;
} KeyOffset;

int importIndexesFromBinaryFile(KeyOffset **keyOffsetArr);

void exportKeyOffsetsToBinaryFile(KeyOffset **keyOffsetArr, int size);

void sortKeyOffset(KeyOffset **keyOffsetArr, int size);

KeyOffset *findKeyOffset(KeyOffset **keyOffsetArr, int size, int key);

#endif //FILMES_KEYOFFSET_H
