#include "rodcutting.h"
#include "solution.h"  // We call print_solution from here
#include <stdio.h>
#include <stdlib.h>

void solve_rod_cutting(Piece pieces[], int count, int rod_length) {
    int *values = (int *)malloc((rod_length + 1) * sizeof(int));
    int *cuts   = (int *)malloc((rod_length + 1) * sizeof(int));

    if (values == NULL || cuts == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize arrays
    for (int currentLength = 0; currentLength <= rod_length; currentLength++) {
        values[currentLength] = 0;
        cuts[currentLength]   = -1;
    }

    // Build the table values
    for (int currentLength = 1; currentLength <= rod_length; currentLength++) {
        for (int pieceIndex = 0; pieceIndex < count; pieceIndex++) {
            if (pieces[pieceIndex].length <= currentLength) {
                int newValue = pieces[pieceIndex].price
                               + values[currentLength - pieces[pieceIndex].length];
                if (newValue > values[currentLength]) {
                    values[currentLength] = newValue;
                    cuts[currentLength]   = pieceIndex;
                }
            }
        }
    }

    // Print final solution (cuts, remainder, total value)
    print_solution(rod_length, pieces, cuts, values[rod_length]);

    // Clean up
    free(values);
    free(cuts);
}


