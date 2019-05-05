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

int importKeyOffsetsFromBinaryFile(KeyOffset **keyOffsetArr);

void exportKeyOffsetsToBinaryFile(KeyOffset *keyOffsetArr, int size);

void sortKeyOffsetsByKey(KeyOffset *keyOffsetArr, int size);

void sortKeyOffsetsByOffset(KeyOffset *keyOffsetArr, int size);

KeyOffset *findKeyOffset(KeyOffset *keyOffsetArr, int size, int key);

int addKeyOffset(KeyOffset **keyOffsetArr, int size, KeyOffset *keyOffset);

int removeKeyOffset(KeyOffset **keyOffsetArr, int size, KeyOffset *keyOffset);

#endif //FILMES_KEYOFFSET_H
