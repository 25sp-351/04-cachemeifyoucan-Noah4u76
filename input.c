// input.c
#include "input.h"
#include <stdio.h>
#include <stdlib.h>

int read_pieces_from_file(const char* filename, Piece pieces[], int *count)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return 1;  // non-zero indicates error
    }

    *count = 0;
    while (fscanf(fp, "%d %d", &pieces[*count].length, &pieces[*count].price) == 2) {
        (*count)++;
        if (*count >= MAX_PIECES) {
            fprintf(stderr, "Warning: Reached MAX_PIECES (%d)\n", MAX_PIECES);
            break;
        }
    }

    fclose(fp);
    return 0; // zero indicates success
}
