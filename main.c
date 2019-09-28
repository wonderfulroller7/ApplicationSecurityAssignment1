#include<stdlib.h>
#include<stdio.h>
#include "dictionary.h"

int main(int argc, char **argv) {

    hashmap_t* hash_table;
    int result = EXIT_FAILURE;
    if (load_dictionary("wordlist.txt", hash_table)) {
        result = EXIT_SUCCESS;
    }
    return result;
}