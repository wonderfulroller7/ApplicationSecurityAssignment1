#include<stdlib.h>
#include<stdio.h>
#include "dictionary.h"

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

	int result = EXIT_SUCCESS;
	return result;
}

bool check_word(const char* word, hashmap_t hashtable[]) {

	bool valid_word = true;
	
	return valid_word;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

	bool load = true;
	printf("%s\n", dictionary_file);
	FILE *dictionary = fopen(dictionary_file, "r");
	if ( dictionary == NULL ) {
		perror( "Dictionary file error" );
		load = false;
	} else {
		printf("Dictionary file loading is a success\n");
	}
	return load;
}
