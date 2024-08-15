#include "prime.h"
#include "hash_table.h"
#include <math.h>

/**
 *@brief return x is prime or not
 *@return
 *  1 - prime
 *  0 - not prime
 *  1 - undefiend (i.e x < 2)
 */
int is_prime(const int x) {
    if (x < 2) return -1;
    if (x < 4) return 1;
    if ((x % 2) == 0) return 0;
    for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
        if ((x % i) == 0) return 0;
    }
    return 1;
};

/**
 *@return the new prime number after x
 *@param x the number that want to get the next prime number after it
 */
int next_prime(int x) {
    while (is_prime(x) != 1) x++;
    return x;
};
