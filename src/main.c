#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include "counter.h"

int main(int argc, char* argv[]) {
    int opt;
    // Flag per le opzioni
    int do_bytes = 0, do_lines = 0, do_words = 0;
    
    // Se non vengono passate opzioni, il comportamento standard di wc 
    // è mostrare tutto (-clw)
    int no_opts = 1;

    // Parsing delle opzioni
    while ((opt = getopt(argc, argv, "clw")) != -1) {
        no_opts = 0;
        switch (opt) {
            case 'c': do_bytes = 1; break;
            case 'l': do_lines = 1; break;
            case 'w': do_words = 1; break;
            default:
                fprintf(stderr, "Uso: %s [-clw] [file]\n", argv[0]);
                return 1;
        }
    }

    // Se l'utente non ha specificato flag, li attiviamo tutti
    if (no_opts) {
        do_bytes = 1;
        do_lines = 1;
        do_words = 1;
    }

    // Il nome del file è il primo argomento dopo le opzioni
    char* file_name = argv[optind];
    FILE* fp;

    if (file_name == NULL) {
        // Se non c'è un file, leggiamo dallo Standard Input (per pipe come: cat file | ccwc)
        fp = stdin;
        file_name = ""; // Non stampiamo il nome se leggiamo da stdin
    } else {
        fp = fopen(file_name, "r");
        if (fp == NULL) {
            perror("Errore apertura file");
            return 1;
        }
    }

    // Variabili per i risultati
    long long lines_count, words_count, bytes_count;

    // Eseguiamo la scansione unica
    count_all(fp, &lines_count, &words_count, &bytes_count);

    // Stampa dei risultati in base ai flag attivati
    // L'ordine standard di wc è: linee, parole, byte
    if (do_lines) printf("%8lld ", lines_count);
    if (do_words) printf("%8lld ", words_count);
    if (do_bytes) printf("%8lld ", bytes_count);
    
    printf("%s\n", file_name);

    if (fp != stdin) {
        fclose(fp);
    }

    return 0;
}