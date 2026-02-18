#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include "counter.h"

/**
 * Funzione di utilità per stampare i risultati formattati.
 */
void print_results(long long lines, long long words, long long bytes, 
                   int do_l, int do_w, int do_c, const char* name) {
    if (do_l) printf("%8lld ", lines);
    if (do_w) printf("%8lld ", words);
    if (do_c) printf("%8lld ", bytes);
    printf("%s\n", name);
}

int main(int argc, char* argv[]) {
    int opt;
    // Flag per le opzioni
    int do_bytes = 0, do_lines = 0, do_words = 0;
    int no_opts = 1;

    // 1. Parsing delle opzioni con getopt
    while ((opt = getopt(argc, argv, "clw")) != -1) {
        no_opts = 0;
        switch (opt) {
            case 'c': do_bytes = 1; break;
            case 'l': do_lines = 1; break;
            case 'w': do_words = 1; break;
            default:
                fprintf(stderr, "Uso: %s [-clw] [file1 file2 ...]\n", argv[0]);
                return 1;
        }
    }

    // Se non ci sono flag, l'impostazione predefinita è -clw
    if (no_opts) {
        do_bytes = do_lines = do_words = 1;
    }

    // Variabili per i conteggi
    long long l_count, w_count, b_count;

    // 2. Gestione dell'Input
    if (optind == argc) {
        /* CASO A: Nessun file passato, leggiamo dallo Standard Input */
        count_all(stdin, &l_count, &w_count, &b_count);
        print_results(l_count, w_count, b_count, do_lines, do_words, do_bytes, "");
    } 
    else {
        /* CASO B: Uno o più file passati come argomenti */
        long long total_l = 0, total_w = 0, total_b = 0;
        int files_processed = 0;

        for (int i = optind; i < argc; i++) {
            FILE* fp = fopen(argv[i], "r");
            if (fp == NULL) {
                perror(argv[i]); // Stampa l'errore specifico (es. Permission denied)
                continue;        // Salta al file successivo
            }

            count_all(fp, &l_count, &w_count, &b_count);
            print_results(l_count, w_count, b_count, do_lines, do_words, do_bytes, argv[i]);

            // Accumulo per il totale finale
            total_l += l_count;
            total_w += w_count;
            total_b += b_count;
            files_processed++;

            fclose(fp);
        }

        // Se abbiamo processato più di un file, stampiamo la riga "total"
        if (files_processed > 1) {
            print_results(total_l, total_w, total_b, do_lines, do_words, do_bytes, "total");
        }
    }

    return 0;
}