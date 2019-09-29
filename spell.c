#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "dictionary.h"

void print_hash_table(hashmap_t hash_table[]) {

    for (int i=0; i<HASH_SIZE; i++) {
		fprintf(stdout, "Bucket %d ===========\n", i);
		//hashmap_t probe_head = hash_table[1999];
		if ( hash_table[i] != NULL) {
			hashmap_t probe_head = hash_table[i];
			do {
				fprintf(stdout,"%s->", probe_head->word);
				probe_head = probe_head -> next;
			} while(probe_head->next != NULL);
			fprintf(stdout,"%s\n", probe_head->word);
			fflush(stdout);
			free(probe_head);
		} else {
			fprintf(stdout,"Bucket is empty\n");
		}
		fflush(stdout);
    }
}

void print_bucket(int bucket, hashmap_t hashtable[]) {
	hashmap_t temp = hashtable[bucket];
	if (temp != NULL) {
		do {
			fprintf(stdout,"%s->", temp->word);
			temp = temp -> next;
		} while(temp->next != NULL);
		fprintf(stdout,"%s\n", temp->word);
	} else {
		fprintf(stdout,"Bucket is empty");
	}
}

bool isNumber(const char* word)
{	
	bool result = true;
	for (int loop=0; loop<strlen(word); loop++) {
		if (!isdigit(word[loop])){
			result = false;
			break;
		}
	}
	return result;
}

bool isASpecialCharacter(char c) {

	int value = (int)c;
	if ( (value>=65 && value <=90) || (value>=97 && value <=122))
		return false;
	else
		return true;
}

void read_file(FILE *fp) {

    char buffer[1024];
    while( fgets(buffer, 1024, fp) != NULL ) {
        fprintf(stdout, "%s\n", buffer);
    }
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

	int result = EXIT_FAILURE;

	if	(fp == NULL) {
        perror("Unable to open file!");
		return result;
    }

	print_bucket(414, hashtable);

	char file_char;
	int current_counter = 0;
	int misspelled_counter = 0;
	char *word = malloc(46*sizeof(char));
	int line_counter = 0;
	// read_file(fp);
	do {
		file_char = (char)fgetc(fp);
		// printf("%c", file_char);
		if (isASpecialCharacter(file_char)) {
			// printf("SC");
			// printf("\nSpecial character found. Current counter: %d \n", line_counter);
			if (file_char == '\n')
				line_counter++;
			if (current_counter == 0) {
				continue;
			} else {
				// fprintf(stdout, "(%s) will be checked\n", word);
				if (check_word(word, hashtable)) {
					current_counter = 0;
					word = malloc(46*sizeof(char));
				} else {
					misspelled[misspelled_counter++] = word;
					fprintf(stdout, "%s is wrong on line %d\n", word, line_counter);
					word = malloc(46*sizeof(char));
					current_counter = 0;
				}
				//printf("\n");
			}
		} else {
			word[current_counter++] = file_char;
		}
	} while(file_char != EOF);

	return result;
}

/** 
 * Check if the word is valid or not 
 * 
 * Map hash bucket value to word and see if word exists in aspell
 **/
bool check_word(const char* word, hashmap_t hashtable[]) {

	bool valid_word = false;

	if (strlen(word) > LENGTH) {
		fprintf(stderr, "%s is not a word", word);
	}

	char* temp_word = malloc((strlen(word) + 1)*sizeof(char));
	memcpy(temp_word, word, strlen(word));
	temp_word[strlen(word)] = '\0';
	
	if (isNumber(temp_word)) {
		return valid_word;
	}
	
	for (int loop=0; loop<strlen(word); loop++) {
		temp_word[loop] = tolower(word[loop]);
	}

	int hashvalue = hash_function(temp_word);
	fprintf(stdout, "(%s) in bucket %d\n", temp_word, hashvalue);

	if (hashtable[hashvalue] != NULL) {
		hashmap_t bucket_probe = hashtable[hashvalue];
		do {
			if (strncmp(bucket_probe->word, temp_word, strlen(temp_word)) == 0) {
				valid_word = true;
				break;
			} else {
				bucket_probe = bucket_probe -> next;
			}
		} while(bucket_probe -> next != NULL);
		if (strncmp(bucket_probe->word, temp_word, strlen(temp_word)) == 0) {
				valid_word = true;
		}
		free(bucket_probe);
		free(temp_word);
	}

	if (!valid_word) {
		print_bucket(hashvalue, hashtable);
	}

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
	int highest_bucket = 0;
	int word_count = 0;
	do {
		char buffer[max_buffer_size];
		while( fgets(buffer, max_buffer_size, dictionary) != NULL ) {
			word_count++;
			char *word = (char *)malloc(sizeof(strlen(buffer)));
			memcpy(word, buffer, strlen(buffer) + 1);
			word[strlen(word) - 1] = '\0';
			// fprintf(stdout, "%s\n", buffer);
			for (int loop=0; loop<strlen(word); loop++) {
				word[loop] = tolower(word[loop]);
			}
			int bucket_number = hash_function(word);
			// if (bucket_number == 5) {
			// 	printf("%s\n", word);
			// }
			if (bucket_number > highest_bucket)
				highest_bucket = bucket_number;
			// fprintf(stdout, "%s %d\n", buffer, bucket_number);
			int linked_list_pos = 0;
			hashmap_t probe_head = hashtable[bucket_number];
			if (probe_head != NULL) {
				// Append Node to end of the list when bucket is not empty
				while (probe_head->next != NULL) {
					//fprintf(stdout, "%s ->", probe_head->word);
					probe_head = probe_head->next;
					linked_list_pos++;
				}
				hashmap_t new_node = (node *)malloc(sizeof(node));
				new_node->next = NULL;
				//memcpy(probe_head->word, word, strlen(buffer));
				snprintf(new_node->word,sizeof(new_node->word),"%s", word);
				probe_head->next = new_node;
			} else {
				// Insert word into an empty bucket
				hashtable[bucket_number] = (node *)malloc(sizeof(node));
				hashtable[bucket_number]->next = NULL;
				snprintf(hashtable[bucket_number]->word,sizeof(hashtable[bucket_number]->word),"%s", word);
				//memcpy(probe_head->word, buffer, strlen(buffer));
			}
			// fprintf(stdout, "%s %d %d\n", word, bucket_number, linked_list_pos);
			free(word);
		}
		// fprintf(stdout, "Words counted %d\n", word_count);
		// fprintf(stdout, "Highest bucket number %d\n", highest_bucket);
		// fprintf(stdout, "Debug: read dictionary changed\n");
		read_dictionary = false;
	} while(read_dictionary);
	
	// print_bucket(414, hashtable);
	//print_hash_table(hashtable);

	fprintf(stdout, "Debug: maximum Buffer size: %d\n", max_buffer_size);
	fclose(dictionary);

	// if ( fclose(dictionary) != 0) {
	// 	perror("Dictionary closing error");
	// }
	load = true;
	return load;
}


