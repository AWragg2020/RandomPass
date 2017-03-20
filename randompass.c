//Author: Adrian Wragg Ruiz (https://github.com/Liam2016)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//declaration of functions
char *generateRandomPass();
int *randNoRep(int limit);
void buildFinalChars(char *final_chars, int conf, int start_pos, int list, int list_length);
void genSimplePass(char *ret, int input_group, int length);

#define az "abcdefghijklmnopqrstuvwxyz"
#define AZ "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define nmbrs "0123456789"
#define otherchars ",.-;:_<>\\|@#~{[]}!\"$&/()=?+*^'"
const char *alph_list[4] = {az, AZ, nmbrs, otherchars};

int main(int argc, char** argv)
{
	if(argc==1 || (argc==2 && strcmp(argv[1],"-h")!=0) || argc>4){
		printf("\nInvalid syntax. Use -h for help.\n");
	}
	else if(argc==2){
		printf("\nRandom password generator v1.0\nAuthor:\tAdrian Wragg (https://github.com/Liam2016)\n---------");
		printf("\nSyntax:\t randompass [ -h | length mode [aA0?] ]\n\t· -h: this message\n\t· length: number between 1-3\n");
		printf("\t\t· 1 -> length of output password = 8\n\t\t· 2 -> length of output password = 16\n\t\t· 3 -> length of output password = 32\n");	
		printf("\t· mode: number between 0-4\n\t\t· 0 -> output password will be composed of random characters\n\t\t");
		printf("· 1 -> output password will be composed of a-z characters\n\t\t");
		printf("· 2 -> output password will be composed of a-zA-Z characters\n\t\t");
		printf("· 3 -> output password will be composed of a-zA-Z0-9 characters\n\t\t");
		printf("· 4 -> custom mode. Refer to next argument.\n");
		printf("\t· aA0?: four digit number that will set how many character of what group will the output password have if mode == 4\n");
		printf("\t\t· a: number between 0-9. The output password will have this many a-z characters\n");
		printf("\t\t· A: number between 0-9. The output password will have this many A-Z characters\n");
		printf("\t\t· 0: number between 0-9. The output password will have this many 0-9 characters\n");
		printf("\t\t· ?: number between 0-9. The output password will have this many special characters\n\n");
	}
	else if(argc==3){
		char *ret = generateRandomPass(argv[1],argv[2],"");
		printf("%s\n",ret);
	}
	else{
		char *ret = generateRandomPass(argv[1],argv[2],argv[3]);
		printf("%s\n",ret);
	}
	return 0;
}

/*
*	function generateRandomPass
*	-
*	Input: 
*		input_length: integer between 1-3. The length of the output password will be 2^(2+input_length)
*		mode: integer between 0-4
*			0 -> standard mode (all chars to pick)
*			1 -> a-z
*			2 -> a-zA-Z
*			3 -> a-zA-Z0-9
*			4 -> custom (user gets to choose how many instances of each group of characters the password will have)
*		conf: 4 digits string. If the mode selected is 4, these 4 digits will determine the amount of lowercase characters (first digit),
*		uppercase characters (second digit), number characters (third character) and special characters (fourth digit). The sum of these
*		four digits must be equal to the length of the password.
*	Output: pseudo-randomly generated password
*/
char *generateRandomPass(char *input_length, char *mode, char *conf){

	int input_length_int;
	char *ret;
	if(strcmp(input_length,"1")==0 || strcmp(input_length,"2")==0 || strcmp(input_length,"3")==0){

		if(strcmp(input_length,"1")==0) input_length_int=1;
		if(strcmp(input_length,"2")==0) input_length_int=2;
		if(strcmp(input_length,"3")==0) input_length_int=3;
	
		int length=(int)pow(2,input_length_int+2);
		ret=malloc(length+1);

		if(strcmp(mode,"0")==0) //totally random
			genSimplePass(ret, 4, length);

		else if(strcmp(mode,"1")==0) //lowercase chars
			genSimplePass(ret, 1, length);

		else if(strcmp(mode,"2")==0) //lowercase and uppercase chars
			genSimplePass(ret, 2, length);

		else if(strcmp(mode,"3")==0) //all letters plus numbers
			genSimplePass(ret, 3, length);

		else if(strcmp(mode,"4")==0){ //user gets to choose

			int conf1=conf[0]-'0', conf2=conf[1]-'0', conf3=conf[2]-'0', conf4=conf[3]-'0';

			if 	(	(strlen(conf)==4)
					&& (conf1 + conf2 + conf3 + conf4 == length)
					&& (conf1 >= 0) && (conf2 >= 0) && (conf3 >= 0) && (conf4 >= 0)
					&& (conf1 <= 9) && (conf2 <= 9) && (conf3 <= 9) && (conf4 <= 9)		)
			{
				
				char final_chars[length], i;

				buildFinalChars(final_chars, conf1, 0, 0, 26);
				buildFinalChars(final_chars, conf2, conf1, 1, 26);
				buildFinalChars(final_chars, conf3, conf1+conf2, 2, 10);
				buildFinalChars(final_chars, conf4, conf1+conf2+conf3, 3, 30);

				int *positions=randNoRep(length);

				for(i=0; i<length; i++){
					memcpy(ret+sizeof(char)*positions[i],final_chars+sizeof(char)*i,1);
				}
			}

			else{
				printf("Bad input: %s\nUse -h for help.\n",conf);
			}
		}

		else{
			printf("Unkown mode: %s\n. Use -h for help.\n",mode);
		}
	}

	else{
		printf("Unkown length. Use -h for help.\n");
	}

	return ret;
}

/*
*	function randNoRep
*	-
*	Input:
*		size: integer that specifies the length o the resulting array
*	Output: array containing every number between 0 and the desired size randomly sorted without repetition
*/
int *randNoRep(int size){
	int *array=calloc(sizeof(int),size);

	for (int i = 0; i < size; i++) {
    	array[i] = i;
	}

	for (int i = 0; i < size; i++) {
    	int temp = array[i];
    	int randomIndex = rand() % size;
    	array[i] = array[randomIndex];
    	array[randomIndex] = temp;
	}
	return array;
}

/*
*	function buildFinalChars
*	Concatenates a series of characters whose quantity is determined by input 'conf' to a string 'final_chars' 
*	-
*	Input:
*		final_chars: string to which concatenate chars
*		conf: amount of chars to concatenate to string
*		start_pos: the start position at which the characters will be concatenated
*		list_index: integer indicates what kind of characters will be added (lowercase chars, uppercase chars, numbers or special chars)
*		list_length: length of the list chosen, used for counting iterations in a loop
*/
void buildFinalChars(char *final_chars, int conf, int start_pos, int list_index, int list_length){
	int i;
	srand(time(0));
	for(i=0; i<conf; i++){
		memcpy(final_chars+sizeof(char)*(i+start_pos),alph_list[list_index]+sizeof(char)*(rand()%list_length),1);
	}
}

/*
*	function genSimplePass
*	Concatenates a randomly chosen amount of characters to a string
*	-
*	Input:
*		ret: the string to which the characters will be concatenated
*		input_group: integer that will indicate what groups will the characters be take from.
*			1 -> lowercase chars
*			2 -> lowercase and uppercase chars
*			3 -> lowercase chars, uppercase chars and numbers
*			4 -> lowercase and uppercase chars, numbers and special chars
*		length:integer that indicates the resulting password's length
*/
void genSimplePass(char *ret, int input_group, int length){
	srand(time(0));
	int i, group, position;
	for(i=0; i<length; i++){
		group=rand()%input_group;
		position=rand()%(int)strlen(alph_list[group]);
		memcpy(ret+sizeof(char)*i, alph_list[group]+sizeof(char)*position,1);
	}
}