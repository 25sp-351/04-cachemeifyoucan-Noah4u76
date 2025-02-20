#include <stdio.h>
#include "pieces.h"  // for MAX_PIECES and Piece struct

/**
 * Prints the final cutting solution.
 *
 * \param rod_length The original rod length.
 * \param pieces     The array of piece definitions (length, price).
 * \param cuts       An array that indicates which piece index was chosen at each sub-length.
 * \param total_value The final maximum value from cutting the rod.
 */
void print_solution(int rod_length, Piece pieces[], int cuts[], int total_value)
{
    // If no value, we can still report remainder and 0
    if (total_value == 0) {
        printf("Remainder: %d\n", rod_length);
        printf("Value: 0\n");
        return;
    }

    // pieceCount array to track how many of each piece was used
    int remainder = rod_length;
    int pieceCount[MAX_PIECES] = {0};

    // Reconstruct which pieces were chosen from the 'cuts' table
    while (remainder > 0 && cuts[remainder] != -1) {
        pieceCount[cuts[remainder]]++;
        remainder -= pieces[cuts[remainder]].length;
    }

    // Print each piece that was used, in the specified format:
    //   "<n> @ <size> = <total value>"
    for (int i = 0; i < MAX_PIECES && pieces[i].length > 0; i++) {
        if (pieceCount[i] > 0) {
            int pieceTotal = pieceCount[i] * pieces[i].price;
            printf("%d @ %d = %d\n",
                   pieceCount[i],
                   pieces[i].length,
                   pieceTotal);
        }
    }

    // Print remainder and total
    printf("Remainder: %d\n", remainder);
    printf("Value: %d\n", total_value);
}
