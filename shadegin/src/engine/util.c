#include "util.h"

ull generate_random_id()
{
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL)); // Seed the random number generator with the current time
        initialized = 1;
    }

    // Generate a random 64-bit unsigned integer
    ull id = ((ull)rand() << 32) | rand();
    return id;
}
