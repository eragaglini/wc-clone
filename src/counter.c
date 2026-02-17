#include <stdio.h>
#include <ctype.h>
#include "counter.h"

void count_all(FILE* fp, long long* lines, long long* words, long long* bytes) {
    int c;
    int in_word = 0;
    
    *lines = 0;
    *words = 0;
    *bytes = 0;

    while ((c = fgetc(fp)) != EOF) {
        (*bytes)++; // Ogni fgetc riuscito è un byte

        if (c == '\n') {
            (*lines)++;
        }

        if (isspace((unsigned char)c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            (*words)++;
        }
    }
}
