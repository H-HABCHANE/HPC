#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank, size, n, i, count = 0, global_count;
    double x, y, z, pi, start_time, end_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // set the number of points to use
    n = 10000000;

    // start timer
    if (rank == 0)
        start_time = MPI_Wtime();

    // initialize random number generator
    srand(time(NULL) + rank);

    // generate random points and count those inside the circle
    for (i = 0; i < n; i++)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = x * x + y * y;
        if (z <= 1)
            count++;
    }

    // combine counts from all processes
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // calculate pi and print result
    if (rank == 0)
    {
        pi = 4.0 * global_count / (n * size);
        printf("pi = %f\n", pi);

        // end timer
        end_time = MPI_Wtime();
        printf("elapsed time = %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}

