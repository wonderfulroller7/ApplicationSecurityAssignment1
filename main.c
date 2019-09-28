#include<stdlib.h>
#include<stdio.h>
#include "dictionary.h"

int main(int argc, char **argv) {

    hashmap_t* hash_table;
    int result = EXIT_FAILURE;
    if (load_dictionary(argv[1], hash_table)) {
        result = EXIT_SUCCESS;
    }
    return result;
}