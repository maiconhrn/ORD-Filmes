//
// Created by Maicon on 27/04/2019.
//

#include <string.h>
#include <stdlib.h>

#include "keyoffset.h"

int compareKeyOffset(const void *a, const void *b) {
    return ((const KeyOffset *) a)->key - ((const KeyOffset *) b)->key;
}

void sortKeyOffset(KeyOffset **keyOffsetArr, int size) {
    qsort(keyOffsetArr, size, sizeof(KeyOffset), compareKeyOffset);
}

KeyOffset *findKeyOffset(KeyOffset **keyOffsetArr, int size, int key) {
    return (KeyOffset *) bsearch(&key, keyOffsetArr, size, sizeof(KeyOffset), compareKeyOffset);
}

void exportKeyOffsetsToBinaryFile(KeyOffset **keyOffsetArr, int size) {
    FILE *indexFile = fopen(INDEX_FILE_NAME, "wb");

    fwrite(&size, 1, 1 * sizeof(int), indexFile);
    for (int i = 0; i < size; ++i) {
        fwrite(&((*keyOffsetArr)[i].key), 1, sizeof(int), indexFile);
        fwrite("|", 1, sizeof(char), indexFile);
        fwrite(&((*keyOffsetArr)[i].offset), 1, sizeof(int), indexFile);
        fwrite("|", 1, sizeof(char), indexFile);
    }
    fclose(indexFile);
}

short readReck(char *recbuff, FILE *fd) {
    short rec_lgth;

    if (fread(&rec_lgth, sizeof(rec_lgth), 1, fd) == 0) // get record length
        return 0;

    rec_lgth = fread(recbuff, sizeof(char), rec_lgth, fd); // read record
    recbuff[rec_lgth] = '\0';

    return rec_lgth;
}

void fillFromReg(KeyOffset *keyOffset, char *reg) {
    keyOffset->key = atoi(strtok(reg, "|"));
    keyOffset->offset = atoi(strtok(NULL, "|"));
}

int importIndexesFromBinaryFile(KeyOffset **keyOffsetArr) {
    FILE *indexFile = fopen(INDEX_FILE_NAME, "rb");

    char reg[100];
    int numRegs;
    fread(&numRegs, 1, sizeof(int), indexFile);

    *keyOffsetArr = (KeyOffset *) malloc(numRegs * sizeof(KeyOffset));

    for (int i = 0; i < numRegs; ++i) {
        readReck(reg, indexFile);
        fillFromReg(&(*keyOffsetArr)[i], reg);
    }

    return numRegs;
}
