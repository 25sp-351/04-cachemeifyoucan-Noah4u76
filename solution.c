#include <stdio.h>
#include "pieces.h"

void print_solution(int rod_length, Piece pieces[], int cuts[], int total_value)
{
    if (total_value == 0) {
        printf("Remainder: %d\n", rod_length);
        printf("Value: 0\n");
        return;
    }

    int remainder = rod_length;
    int pieceCount[MAX_PIECES] = {0};

    while (remainder > 0 && cuts[remainder] != -1) {
        pieceCount[cuts[remainder]]++;
        remainder -= pieces[cuts[remainder]].length;
    }

    //   "<n> @ <size> = <total value>"
    for (int ix = 0; ix < MAX_PIECES && pieces[ix].length > 0; ix++) {
        if (pieceCount[ix] > 0) {
            int pieceTotal = pieceCount[ix] * pieces[ix].price;
            printf("%d @ %d = %d\n",
                   pieceCount[ix],
                   pieces[ix].length,
                   pieceTotal);
        }
    }
    
    printf("Remainder: %d\n", remainder);
    printf("Value: %d\n", total_value);
}
