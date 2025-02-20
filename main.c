#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"
#include "input.h"
#include "rodcutting.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pieces_file>\n", argv[0]);
        return 1;
    }

    // Read (length, value) pairs from the specified file
    Piece pieces[MAX_PIECES];
    int pieceCount = 0;
    if (read_pieces_from_file(argv[1], pieces, &pieceCount) != 0) {
        fprintf(stderr, "Error reading pieces.\n");
        return 1;
    }

    // Now read rod lengths from stdin (one per line) until EOF
    int rod_length;
    printf("Enter rod lengths, one per line. Ctrl+Z or Ctrl+D to end.\n");
    while (scanf("%d", &rod_length) == 1) {
        solve_rod_cutting(pieces, pieceCount, rod_length);
        printf("\n");
    }

    return 0;
}