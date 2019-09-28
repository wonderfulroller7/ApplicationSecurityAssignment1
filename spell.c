#include<stdlib.h>
#include<stdio.h>
#include<string.h>
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

	bool load = false;
	fprintf(stdout, "%s\n", dictionary_file);
	FILE *dictionary = fopen(dictionary_file, "r");
	if ( dictionary == NULL ) {
		perror( "Dictionary file error" );
		return load;
	}
	hashtable = NULL;
	int max_buffer_size = LENGTH + 1;
	bool read_dictionary = true;
	int counter = 0;
	do {
		char buffer[max_buffer_size];
		while( fgets(buffer, max_buffer_size, dictionary) != NULL ) {
			buffer[strlen(buffer) - 1] = '\0';
			fprintf(stdout, "%s\n", buffer);
			hashmap_t temp_node = (node *)malloc(sizeof(node));
			char temp_value[strlen(buffer)]; 
			strncpy(temp_value, buffer, strlen(buffer));
			hashtable[counter] = temp_node;
			counter++;
		}
		fprintf(stdout, "Debug: read dictionary changed\n");
		read_dictionary = false;
	} while(read_dictionary);

	fprintf(stdout, "Debug: maximum Buffer size: %d\n", max_buffer_size);

	if ( fclose(dictionary) != 0) {
		perror("Dictionary closing error");
	}
	load = true;
	return load;
}
