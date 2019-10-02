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
			probe_head = NULL;
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
	temp = NULL;
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

bool isAPunctuation(char c) {

	bool result = false;
	if (c == '\'' || c =='.' || c==':' || c=='!' || c=='\"')
		result = true;
	return result;
}

bool isAnAlphabet(char c) {

	int value = (int)c;
	// printf("%d -> %c, ", value, c);
	if ((value>=65 && value <=90))
		return true;
	else
		return false; 
}


// Implement control character
bool isAsciiControlCharacter(char c) {

	int value = (int)c;
	if (value>=1 && value<=32) {
		return true;
	} else 
		return false;
}


bool isASpecialCharacter(char c) {

	if ( isAnAlphabet(c) 
		|| isAPunctuation(c)) 
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

int check_words(FILE* fp, hashmap_t hashtable[], char *misspelled[]) {

	int misspelled_counter = 0;

	if	(fp == NULL) {
        perror("Unable to open file!");
		return misspelled_counter;
    }

	char file_char;
	int current_counter = 0;
	char word[46];
	int line_counter = 0;
	// read_file(fp);
	do {
		file_char = (char)fgetc(fp);
		if (isAsciiControlCharacter(file_char)) {
			if (file_char == '\n') {
				line_counter++;
			}
			if (current_counter <= 45) { // encountered a word that is less than or just of length 45
				// printf("%s ", word);
				word[current_counter] = '\0';
				if (word[current_counter - 1] == '\'' || word[strlen(word) - 1] == '.') { // check if the last character is a punctuation
					word[current_counter - 1] = '\0';
				}
				if (current_counter == 0) {
					continue;
				}
				// printf("%s\n", word);
				if (check_word(word, hashtable)) {
					current_counter = 0;
					word[current_counter] = '\0';
				} else {
					misspelled[misspelled_counter] = calloc(current_counter+1, sizeof(char));
					snprintf(misspelled[misspelled_counter++],current_counter+1,"%s",word);
					current_counter = 0;
					word[current_counter] = '\0';
				}
			} else {
				misspelled[misspelled_counter] = calloc(strlen(word), sizeof(char));
				snprintf(misspelled[misspelled_counter++],strlen(word),"%s",word);
				current_counter = 0;
				word[current_counter] = '\0';
			}
		} else {
			// printf("Current Counter = %d Word = %s\n", current_counter, word);
			if (current_counter < 46) {
				word[current_counter] = file_char;
			}
			current_counter++;
		}
		// printf("Misspelled count = %d %c %d\n", misspelled_counter, file_char, (int)file_char);
	} while(file_char != EOF);

	// destroy_misspelled(misspelled, misspelled_counter);
	if ( fclose(fp) != 0) {
		perror("Text file closing error");
	}
	return misspelled_counter;
}

/** 
 * Check if the word is valid or not 
 * 
 * Map hash bucket value to word and see if word exists in aspell
 **/
bool check_word(const char* word, hashmap_t hashtable[]) {

	bool valid_word = false;
	int word_length = strlen(word);

	if (word_length > LENGTH) {
		fprintf(stderr, "%s is not a word", word);
	}

	char* temp_word = malloc((word_length + 1)*sizeof(char));
	memcpy(temp_word, word, word_length);
	temp_word[word_length] = '\0';
	
	if (isNumber(temp_word)) {
		return valid_word;
	}
	
	for (int loop=0; loop<word_length; loop++) {
		temp_word[loop] = tolower(word[loop]);
	}

	int hashvalue = hash_function(temp_word);

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
		bucket_probe = NULL;
	}

	// if (!valid_word) {
	// 	fprintf(stdout, "(%s) in bucket %d\n", temp_word, hashvalue);
	// 	print_bucket(hashvalue, hashtable);
	// }

	free(temp_word);

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
			char word[LENGTH + 1];
			memcpy(word, buffer, strlen(buffer) + 1);
			word[strlen(word) - 1] = '\0';
			for (int loop=0; loop<strlen(word); loop++) {
				word[loop] = tolower(word[loop]);
			}
			int bucket_number = hash_function(word);
			if (bucket_number > highest_bucket)
				highest_bucket = bucket_number;
			int linked_list_pos = 0;
			hashmap_t probe_head = hashtable[bucket_number];
			if (probe_head != NULL) {
				// Append Node to end of the list when bucket is not empty
				while (probe_head->next != NULL) {
					probe_head = probe_head->next;
					linked_list_pos++;
				}
				hashmap_t new_node = (hashmap_t)malloc(sizeof(node));
				new_node->next = NULL;
				snprintf(new_node->word, sizeof(new_node->word), "%s", word);
				probe_head->next = new_node;
			} else {
				// Insert word into an empty bucket
				hashtable[bucket_number] = (hashmap_t)malloc(sizeof(node));
				hashtable[bucket_number]->next = NULL;
				snprintf(hashtable[bucket_number]->word, sizeof(hashtable[bucket_number]->word), "%s", word);
			}
			word[0] = '\0';
			//free(word);
		}
		read_dictionary = false;
	} while(read_dictionary);

	// fprintf(stdout, "Debug: maximum Buffer size: %d\n", max_buffer_size);

	// print_bucket(441, hashtable);

	if ( fclose(dictionary) != 0) {
		perror("Dictionary closing error");
	}
	load = true;
	return load;
}


