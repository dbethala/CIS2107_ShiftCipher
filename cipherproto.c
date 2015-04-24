#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_STR_LEN 200
char character_shift(char, int); 
int is_valid_word(FILE*, FILE*);


char* string_shift(char* in_str, int shift_val){
	//String shift takes a pointer to string or a character array and an integer value. The function passes each character through the character_shift function aand shifts each character by the value taken in as the parameter. The new string is returned.  Only the
	//26 members of the alphabet are shifted. This function ignores punctuation and case.
	
	//The new string to be built and its initialization.
	char* ret_str;
	ret_str = malloc(300);
	
	//The character passed into the character_shift function.
	char local_char;
	
	//A local variable used to point to the new string being built.
	int i = 0;

	//The loop executes until the null character (/0) is encountered. It passes each character to the character_shift function and places the shifted character into a new string.  The pointer to the string is then incremented.
	while (*in_str != '\0'){
		//Storing an individual character into a local variable.
		local_char = *in_str;
		
		//Only the letters of the alphabet will be passed to be shifted. If a character encountered is a space or a punctuation mark, it is automatically placed into the new string without alteration.
		if(isalpha(local_char)){
			
			//Calling character_shift and storing the value in local_char
			local_char = character_shift(local_char, shift_val);

			//Storing the newly-shifted character into the return string.
			ret_str[i] = local_char;

			//Incrementing both the return string and the pointer to the input string.
			in_str++; i++;
		}
		
		//Provision to place non-alphabetic characters into the new string.
		else{
			ret_str[i] = local_char;
			i++; in_str++;
		}
	}
	//Null-termination of the new string and its return statement.
	ret_str[i] = '\0';
	return ret_str;
}


char character_shift(char in_let, int shift_val){
	//Accepts a character and integer value as parameters and returns a character shifed by the value of the integer.
	
	//Creating a local variable
	int local_val = 0;
	
	//Provision to check for an empty character. Prints an error message if a null character is passed to the function.
	if (in_let == '\0'){
		printf("No input character to shift.");
	}
	else{
		//The loop executes while the local variable is less than the desired shift value.
		while (local_val < shift_val){	
		
			//Shift the character.
			in_let--;
		
			//Statements to preserve the case of the character and maintain the range of the alphabet. '@' precedes 'A' with respect to ASCII values. 
			//Similarly, '`' precedes 'a' in the same manner. This provides for the loop that begins at 'A' or 'a' and ends at 'Z' or 'z'.
			if (in_let == '@'){
				in_let = 'Z';
			}								
			else if (in_let == '`'){								
				in_let = 'z';
			}
			
			//Increment the local variable.			
			local_val++;	
		}

		//Return the shifted character to the caller.
		return in_let;
	}
}


int is_valid_word(FILE* input_file, FILE* dictionary_file){
	/*Function takes two parameters as input: a file pointer to an input file and a pointer to a reference file (dictionary file). The function tokenizes every word in a the input file and 
	  compares it to every word in the dictionary file.  It returns the total number of words that were found in the dictionary file.*/
	
	//Returns file pointers to the beinning of each file
	rewind(input_file); rewind(dictionary_file);	

	//Local string to store each word gathered from a dictionary file
	char dictionary_line [300];
	
	//Local string to store each line from an input file
	char local_string [400];

	//Local string that stores each tokenized word
	char tokenized_word[200];

	//Various local variables used to increment pointers. Variable 'a' is used to store the value of strcasecmp.
	int a; int i = 0; int j = 0; int k = 0;

	//The value returned by the function. Stores the total number of words found in the dictionary.
	int valid_word_count = 0;
	
	//While loop executes while the end of the input file isn't encountered. Each line from the input file is read and processed by the following loops.
	while(!feof(input_file)){
		
		//Storing a line into a local variable
		fgets(local_string, 400, input_file);

		//Loops through the local string until the null character is encountered. Each word is tokenized using non-letters as delimiters..
		while(local_string[j] != '\0'){
			
			//Storing each word into a local string.
			tokenized_word[i] = local_string[j];
			i++; j++;
			
			//Loop only executes if the next character in string copied from the file is a punctutation or space.
			if (!isalpha(local_string[j])){
				
				//Appending the null character to the end of the tokenized word.
				tokenized_word[i] = '\0';
				
				//Gathering a word from the dictionary file and appending the null character to the end of the string.
				fgets(dictionary_line, 300, dictionary_file);
				dictionary_line[strlen(dictionary_line) - 1] = '\0';
				
				//Loop executes through the entire dictionary file until a matching word is encountered. 
				while(!feof(dictionary_file)){
					
					//Local variable used to store the return value of strcasecmp (0 if the two strings match);
					a = strcasecmp(tokenized_word, dictionary_line);

					if(a == 0){
						valid_word_count++;	//Incrementing the return value
						j++;			//Incrementing to the beginning of the next word from the input string.
						i = 0;			//Resetting the pointer of the tokenized string
					
						break; 
					}
					else{
						fgets(dictionary_line, 300, dictionary_file);
						dictionary_line[strlen(dictionary_line) - 1] = '\0';
					}
				}
				rewind(dictionary_file);
			}
		}
	}
	return valid_word_count;
}




int main (int argc, char** argv){
	FILE* input_file = fopen(argv[1], "r+");
	if(argv[1] == NULL){
		printf(stderr, "Input file not found");
		return 0;
	}
	FILE* dictionary_file = fopen(argv[2], "r");
	if(argv[2] == NULL){
		dictionary_file = fopen("/usr/share/dict/words", "r");
	}
	FILE* output_file = fopen("output.txt", "r+");
		
	char input_string [300];
	char* shifted_string;

	int shift_counter = 0;
	int valid_word_count;
	
	while(shift_counter < 26){
		while(!feof(input_file)){
			fgets(input_string, 300, input_file);
			shifted_string = string_shift(input_string, shift_counter);
			fprintf(output_file, "%s\n", shifted_string);
		}
		rewind(output_file);

		valid_word_count = is_valid_word(output_file, dictionary_file);
		rewind(output_file);

		if(valid_word_count >= 10){
			printf("The program output the decoded text to a file called 'output.txt' and determined  a shift value of %d.\n", shift_counter);
			break;
		}
		else{
			shift_counter++;
			rewind(output_file);
			rewind(input_file);
			printf("Working...\n");
		} 
	}
	fclose(input_file);
	fclose(dictionary_file);
	fclose(output_file);
	return 0;
	}
	
