#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "dictionary.h"

void print_hash_table(hashmap_t hash_table[]) {

    for (int i=0; i<HASH_SIZE; i++) {
        hashmap_t probe_head = hash_table[i];
        while( probe_head != NULL || probe_head->next != NULL ) {
            fprintf(stdout,"%s->", probe_head->word);
        }
        printf("\n");
    }
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

	int result = EXIT_SUCCESS;
	return result;
}

bool check_word(const char* word, hashmap_t hashtable[]) {

	bool valid_word = true;

	return valid_word;
}

// Load into the hash dictionary
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

	bool load = false;
	FILE *dictionary = fopen(dictionary_file, "r");
	if ( dictionary == NULL ) {
		perror( "Dictionary file error" );
		return load;
	}
	for (int i=0; i< HASH_SIZE; i++) {
		hashtable[i] = NULL;
	}
	int max_buffer_size = LENGTH + 1;
	bool read_dictionary = true;
	do {
		char buffer[max_buffer_size];
		while( fgets(buffer, max_buffer_size, dictionary) != NULL ) {
			buffer[strlen(buffer) - 1] = '\0';
			int bucket_number = hash_function(buffer);
			//fprintf(stdout, "%s %d\n", buffer, bucket_number);
			hashmap_t probe_head = hashtable[bucket_number];
			if (probe_head != NULL) {
				// Append Node to end of the list when bucket is not empty
				hashmap_t head = probe_head;
				while (head->next != NULL) {
					head = head->next;
				}
				hashmap_t new_node = (node *)malloc(sizeof(node));
				new_node->next = NULL;
				memcpy(new_node->word, buffer, strlen(buffer));
				head->next = new_node;
			} else {
				// Insert word into an empty bucket
				probe_head = (node *)malloc(sizeof(node));
				probe_head->next = NULL;
				memcpy(probe_head->word, buffer, strlen(buffer));
			}
		}
		fprintf(stdout, "Debug: read dictionary changed\n");
		read_dictionary = false;
	} while(read_dictionary);

	print_hash_table(hashtable);

	fprintf(stdout, "Debug: maximum Buffer size: %d\n", max_buffer_size);

	if ( fclose(dictionary) != 0) {
		perror("Dictionary closing error");
	}
	load = true;
	return load;
}


