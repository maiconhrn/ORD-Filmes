//
// Created by Maicon on 27/04/2019.
//

#include <string.h>
#include <stdlib.h>

#include "keyoffset.h"

int compareKeyOffset(const void *a, const void *b) {
    return ((const KeyOffset *) a)->key - ((const KeyOffset *) b)->key;
}

void sortKeyOffsets(KeyOffset *keyOffsetArr, int size) {
    qsort(keyOffsetArr, size, sizeof(KeyOffset), compareKeyOffset);
}

KeyOffset *findKeyOffset(KeyOffset *keyOffsetArr, int size, int key) {
    return (KeyOffset *) bsearch(&key, keyOffsetArr, size, sizeof(KeyOffset), compareKeyOffset);
}

void exportKeyOffsetsToBinaryFile(KeyOffset *keyOffsetArr, int size) {
    FILE *indexFile = fopen(INDEX_FILE_NAME, "wb");

    fwrite(&size, sizeof(int), 1, indexFile);
    for (int i = 0; i < size; ++i) {
        fwrite(&keyOffsetArr[i].key, sizeof(int), 1, indexFile);
        fwrite("|", sizeof(char), 1, indexFile);
        fwrite(&keyOffsetArr[i].offset, sizeof(int), 1, indexFile);
        fwrite("|", sizeof(char), 1, indexFile);
    }

    fclose(indexFile);
}

int importKeyOffsetsFromBinaryFile(KeyOffset **keyOffsetArr) {
    FILE *indexFile = fopen(INDEX_FILE_NAME, "rb");

    if (indexFile == NULL) {
        printf("Erro: Nao existe arquivo de indices");
    } else {
        int numRegs;
        fread(&numRegs, sizeof(int), 1, indexFile);

        *keyOffsetArr = (KeyOffset *) malloc(numRegs * sizeof(KeyOffset));

        for (int i = 0; i < numRegs; ++i) {
            fread(&(*keyOffsetArr)[i].key, sizeof(int), 1, indexFile);
            fseek(indexFile, 1, SEEK_CUR);
            fread(&(*keyOffsetArr)[i].offset, sizeof(int), 1, indexFile);
            fseek(indexFile, 1, SEEK_CUR);
        }

        fclose(indexFile);

        return numRegs;
    }
    return 0;
}
