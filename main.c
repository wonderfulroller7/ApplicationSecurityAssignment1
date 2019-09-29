#include<stdlib.h>
#include<stdio.h>
#include "dictionary.h"

int main(int argc, char **argv) {

    node* hash_table[HASH_SIZE];
    int result = EXIT_FAILURE;
    if (argc!=3)
	{
		fprintf(stdout,"Incorrect Usage!\n");
		fprintf(stdout,"./spell_check <dictionary_file> <file_to_be_checked>\n");
	} 
    else {
        fprintf(stdout, "%s %s\n", argv[1], argv[2]);
        if (load_dictionary(argv[1], hash_table)) {
            printf("Dictionary loaded");
            result = EXIT_SUCCESS;
        }
    }
    return result;
}