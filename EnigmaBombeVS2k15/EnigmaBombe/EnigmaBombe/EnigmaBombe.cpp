// EnigmaBombe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <iomanip>

//**********************************
// Erik Vanlandingham              *
// Enigma Code Machine, M3         *
// February 10th, 2015             *
// Last Updated: December 25, 2015 *
//**********************************



//using namespace std;
//Helper Variables\\
//***************************************************************************************************************************************\\
        //User input
int hold = 0;
std::string input;
//Encrypted Output/Decrypted Output
std::string output = "";
//User Action Choice
int option;
//Base alphabet length
int alphabet_length = 25;
//Normal base alphabet length for the loop purposes and rotation purposes
int alphabet_normal_length = 26;
//array index shift when traversing the array
int array_index_shift = 1;
//array modulous shift
int modulous_shift = 1;
//User Option in the Settings
int settings_option;
//Base location in the normal Alphabet.
int base_location = 0;
//Location in the Plugboard.
int plug_board_location = 0;
//Location in the Reflector.
int reflector_base_location = 0;
//Number of rotors
int Number_of_Rotors = 8;
// To check if a Letter is in the Base for Sanatization
bool is_in_base;
//***************************** End Helper Variables ****************************\\
    
//Bombe Variables\\
//***************************************************************************************************************************************\\
         //Test words for Decryption, must be capital words and an  array is used 
//so that multiple words can be used if necessary
const int number_of_keywords = 7;
std::string keyword_array[number_of_keywords] = { "HITLE R","HI TLER","HIT LER", "HITL ER", "H ITLER","DERFUHRER", "WETTERBERICHT" };
//***************************** End Bombe Variables ****************************\\      

//Einstellung\\
//***************************************************************************************************************************************\\
        //Set the starting "base" key Ringstellung is a fixed start increase, numbers correspond to the alphabet
int rotor1_key = 0;
int rotor2_key = 0;
int rotor3_key = 0;

//at which letter does the next rotor turn?, is it double stepped?
//It does not seem to matter what the right most kick values are as it increments the others.
int rotor1_kick_value = 5;
//int rotor1_double_step = 0;

int rotor2_kick_value = 5;
//int rotor2_double_step = 7;

int rotor3_kick_value = 5;
//int rotor3_double_step = 15;

//This is the current position on the array, or the current circuit path as will be modified in the program.
int rotor1_position = 0;
int rotor2_position = 0;
int rotor3_position = 0;

//Where is the given start location, the start location will change as the rotor increment, the take the
//Ringstellung into account- the Ringstellung is a fixed wiring change. The start location is the physical 
//Rotation as 'keys' are pressed.
int rotor1_start = rotor1_key + rotor1_position;
int rotor2_start = rotor2_key + rotor2_position;
int rotor3_start = rotor3_key + rotor3_position;

//*****************************Ende Einstellung****************************\\
    
//Physische Optionen\\
//***************************************************************************************************************************************\\   
//0   1   2   3   4   5   6   7   8   9   10  11  12  13 14  15  16  17  18  19  20  21  22  23  24  25
char base[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
char Enc_Dec_plug_board[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
//The plugboard may vary, however, no letter may be repeated. On the Enigma there were physical connection that made it impossible
//To Assign a Character twice. There must be an Enigma Plugboard and a Bombe plugboard as bome changes the settings
//char Enc_Dec_plug_board[26] = {'V','F','M','U','E','B','G','Z','Y','Q','W','X','C','T','O','P','J','R','S','N','D','A','K','L','I','H'};
char plug_board[26] = { '*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*' };
char empty_plug_board[26] = { '*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*' };

//Choices of rotors begin here:
char I[26] = { 'E','K','M','F','L','G','D','Q','V','Z','N','T','O','W','Y','H','X','U','S','P','A','I','B','R','C','J' };
char II[26] = { 'A','J','D','K','S','I','R','U','X','B','L','H','W','T','M','C','Q','G','Z','N','P','Y','F','V','O','E' };
char III[26] = { 'B','D','F','H','J','L','C','P','R','T','X','V','Z','N','Y','E','I','W','G','A','K','M','U','S','Q','O' };
char IV[26] = { 'E','S','O','V','P','Z','J','A','Y','Q','U','I','R','H','X','L','N','F','T','G','K','D','C','M','W','B' };
char V[26] = { 'V','Z','B','R','G','I','T','Y','U','P','S','D','N','H','L','X','A','W','M','J','Q','O','F','E','C','K' };
char VI[26] = { 'J','P','G','V','O','U','M','F','Y','Q','B','E','N','H','Z','R','D','K','A','S','X','L','I','C','T','W' };
char VII[26] = { 'N','Z','J','H','G','R','C','X','M','Y','S','W','B','O','U','F','A','I','V','L','P','E','K','Q','D','T' };
char VIII[26] = { 'F','K','Q','H','T','L','X','O','C','B','J','S','P','D','Z','R','A','M','E','W','N','I','U','Y','G','V' };

char Rotors[8] = { *I,*II, *III, *IV, *V, *VI, *VII, *VIII };

//Choices of Reflectors, not all mirrors work properly, the source i got htem from had a few mistakes. Beta or Gamma may be bad
char mirrorA[26] = { 'E','J','M','Z','A','L','Y','X','V','B','W','F','C','R','Q','U','O','N','T','S','P','I','K','H','G','D' };
char mirrorB[26] = { 'Y','R','U','H','Q','S','L','D','P','X','N','G','O','K','M','I','E','B','F','Z','C','W','V','J','A','T' };
char mirrorC[26] = { 'F','V','P','J','I','A','O','Y','E','D','R','Z','X','W','G','C','T','K','U','Q','S','B','N','M','H','L' };

char Mirrors[3] = { *mirrorA, *mirrorB, *mirrorC };

//M4 Engima Rotor and Mirrors
char beta[26] = { 'L','E','Y','J','V','C','N','I','X','W','P','B','Q','M','D','R','T','A','K','Z','G','F','U','H','O','S' };
char gamma[26] = { 'F','S','O','K','A','N','U','E','R','H','M','B','T','I','Y','C','W','L','Q','P','Z','X','V','G','J','D' };
char mirrorBThin[26] = { 'E','N','K','Q','A','U','Y','W','J','I','C','O','P','B','L','M','D','X','Z','V','F','T','H','R','G','S' };
char mirrorCThin[26] = { 'R','D','O','B','J','N','T','K','V','E','H','M','L','F','C','W','Z','A','X','G','Y','I','P','S','U','Q' };

//*****************************Ende Physische Optionen****************************\\
    
//Current Config\\
//***************************************************************************************************************************************\\|   
char *rotor1 = I;
char *rotor2 = II;
char *rotor3 = III;
char *mirror = mirrorA;

//M4 Rotor
char *rotor4 = beta;
//*****************************End Current Config*********************************\\
    
//String Functions\\
//***************************************************************************************************************************************\\     
//pass s string in to strip away non alpha characters, and raise lowercase characters to UpperCase for comparison th the rotors
char add_to_plugboard(char char_base, char char_plugboard);
void fill_plugboard();
void reset_plugboard();

std::string sanatize(std::string string_in) {
	std::string input = string_in;
	std::string sanatized_input = "";
	//iterate through each character of the string
	for (int i = 0; i < input.length(); i++) {
		char character = input[i];
		//Check for spaces
		if (character == ' ') {
			continue;
		}
		//if the aplhabetical check fails, move to the next character in the string array
		if (isalpha(character) == false) {
			continue;
		}
		//if everything checks out- put the character to UpperCase 
		else {
			char sanatized_character = toupper(character);
			//append the string to retain on characters
			sanatized_input = sanatized_input + sanatized_character;
		}
	}
	return sanatized_input;
}

//Encryption/Decryption function
std::string enigma(std::string string_in) {
	//string to work with
	std::string sanatized_input = sanatize(string_in);
	//Start with the first element in the string
	for (int string_iterator = 0; string_iterator < sanatized_input.length(); string_iterator++) {
		//Character We will work with in this iteration
		char character = sanatized_input[string_iterator];
		//Increment the Rotors First!\\
	//*****************************************************************************************************\\|
		rotor1_start++;
		//check if rotor 2 needs to be turned
		if (rotor1_start == rotor2_kick_value) {
			//turn the rotor
			rotor2_start++;
			//error check and math for bounds
			if (rotor2_start > alphabet_length) {
				rotor2_start = ((rotor2_start % alphabet_length) - modulous_shift);
			}
			//if the rotor2 turn became the required value to turn rotor 3, turn rotor 3
			if (rotor2_start == rotor3_kick_value) {
				rotor3_start++;
				//error check for bounds
				if (rotor3_start > alphabet_length) {
					rotor3_start = ((rotor3_start % alphabet_length) - modulous_shift);
				}
			}
		}

		//Additional error checking, without it there are cases in which the encryption\decryption can be bugged
		//Loop rotors back around if the values are over alphabet_length = 25
		if (rotor1_start > alphabet_length) {
			rotor1_start = ((rotor1_start % alphabet_length) - modulous_shift);
		}
		if (rotor2_start > alphabet_length) {
			rotor2_start = ((rotor2_start % alphabet_length) - modulous_shift);
		}
		if (rotor3_start > alphabet_length) {
			rotor3_start = ((rotor3_start % alphabet_length) - modulous_shift);
		}

		//loop the start if it drops below 0
		if (rotor1_start < 0) {
			rotor1_start = rotor1_start + alphabet_normal_length;
		}
		if (rotor2_start < 0) {
			rotor2_start = rotor2_start + alphabet_normal_length;
		}
		if (rotor3_start < 0) {
			rotor3_start = rotor3_start + alphabet_normal_length;
		}

		//**************************End Physical Increment*************************************************\\|

		//Find the base, or normal Alphabet, loaction Number of the Character
		for (int base_iterator = 0; base_iterator < alphabet_normal_length; base_iterator++) {
			if (character == base[base_iterator]) {
				base_location = base_iterator;
				break;
			}
		}
		//**************************Find the Equivalent on the Plugboard Down**********************************\\| 
		character = Enc_Dec_plug_board[base_location];

											//Rotor 1 Down\\
	 //*****************************************************************************************************\\|             
		//get the position of the encypted character on rotor 1
		rotor1_position = base_location + rotor1_start;

		//error check for bounds
		if (rotor1_position < 0) {
			rotor1_position = alphabet_normal_length + rotor1_position;
		}

		if (rotor1_position > alphabet_length) {
			rotor1_position = ((rotor1_position % alphabet_length) - modulous_shift);
		}

		//second < 0 statement in case the previous if dropped the value below 0, this fixed a bugged case I encountered
		if (rotor1_position < 0) {
			rotor1_position = alphabet_length + rotor1_position;
		}

		//locate the location of the encrypted letter in the base      
		character = rotor1[rotor1_position];
		for (int iterator = 0; iterator < alphabet_normal_length; iterator++) {
			if (character == base[iterator]) {
				rotor1_position = iterator;
				break;
			}
		}

		//set the character for debugging purposes       
		character = base[rotor1_position];

											//Rotor 2 Down\\
	//*****************************************************************************************************\\|                                        
//Rinse and repeat of rotor 1, although the shift in the first rotor must be accounted for
		rotor2_position = rotor1_position - rotor1_start + rotor2_start;

		if (rotor2_position < 0) {
			rotor2_position = alphabet_normal_length + rotor2_position;
		}

		if (rotor2_position > alphabet_length) {
			rotor2_position = ((rotor2_position % alphabet_length) - modulous_shift);
		}

		if (rotor2_position < 0) {
			rotor1_position = alphabet_length + rotor1_position;
		}

		character = rotor2[rotor2_position];

		for (int iterator = 0; iterator < alphabet_normal_length; iterator++) {
			if (character == base[iterator]) {
				rotor2_position = iterator;
				break;
			}
		}

		character = base[rotor2_position];
											//Rotor 3 Down\\
	//*****************************************************************************************************\\|    
		//Rinse and repeat of Rotor 2 Down
		rotor3_position = rotor2_position - rotor2_start + rotor3_start;

		if (rotor3_position < 0) {
			rotor3_position = alphabet_normal_length + rotor3_position;
		}

		if (rotor3_position > alphabet_length) {
			rotor3_position = ((rotor3_position % alphabet_length) - modulous_shift);
		}

		if (rotor3_position < 0) {
			rotor3_position = alphabet_normal_length + rotor3_position;
		}

		character = rotor3[rotor3_position];

		for (int iterator = 0; iterator < alphabet_normal_length; iterator++) {
			if (character == base[iterator]) {
				rotor3_position = iterator;
				break;
			}
		}

		character = base[rotor3_position];
								  			//Mirror\\
//*****************************************************************************************************\\|  
		//Simple ceasar shift- one to one translation to another character
		reflector_base_location = rotor3_position - rotor3_start;
		character = mirror[reflector_base_location];

		for (int iterator = 0; iterator < alphabet_normal_length; iterator++) {
			if (character == mirror[iterator]) {
				reflector_base_location = iterator;
				break;
			}
		}
		//commented for debugging, appears to work without it
		//character = mirror[reflector_base_location];  
											//Rotor 3 UP\\
	//*****************************************************************************************************\\|          
		rotor3_position = reflector_base_location + rotor3_start;
		if (rotor3_position < 0) {
			rotor3_position = alphabet_normal_length + rotor3_position;
		}

		if (rotor3_position > alphabet_length) {
			rotor3_position = ((rotor3_position % alphabet_length) - modulous_shift);
		}

		character = mirror[rotor3_position];
		for (int iterator = 0; iterator < alphabet_normal_length; iterator++) {
			if (character == rotor3[iterator]) {
				rotor3_position = iterator;
				break;
			}
		}

		character = rotor3[rotor3_position];
		character = base[rotor3_position];
										//Rotor 2 UP\\
	 //*****************************************************************************************************\\| 
		rotor2_position = rotor3_position - rotor3_start + rotor2_start;
		if (rotor2_position < 0) {
			rotor2_position = alphabet_normal_length + rotor2_position;
		}
		// if the increment puts it over, then increment rotor 2
		if (rotor2_position > alphabet_length) {
			rotor2_position = ((rotor2_position % alphabet_length) - modulous_shift);
		}
		character = base[rotor2_position];
		for (int iterator = 0; iterator < alphabet_normal_length; iterator++) {
			if (character == rotor2[iterator]) {
				rotor2_position = iterator;
				break;
			}
		}

		character = rotor2[rotor2_position];

											//Rotor 1 UP\\
	//*****************************************************************************************************\\|
		rotor1_position = rotor2_position - rotor2_start + rotor1_start;
		if (rotor1_position < 0) {
			rotor1_position = alphabet_normal_length + rotor1_position;
		}
		// if the increment puts it over, then increment rotor 2
		if (rotor1_position > alphabet_length) {
			rotor1_position = ((rotor1_position % alphabet_length) - modulous_shift);
		}
		character = base[rotor1_position];
		for (int iterator = 0; iterator < alphabet_normal_length; iterator++) {
			if (character == rotor1[iterator]) {
				rotor1_position = iterator;
				break;
			}
		}

		character = rotor1[rotor1_position];

										//Plugboard UP\\
	//*****************************************************************************************************\\|       
		plug_board_location = rotor1_position - rotor1_start;

		if (plug_board_location < 0) {
			plug_board_location = alphabet_normal_length + plug_board_location;
		}
		// if the increment puts it over, then increment rotor 2
		if (plug_board_location > alphabet_length) {
			plug_board_location = ((plug_board_location % alphabet_length) - modulous_shift);
		}

		character = Enc_Dec_plug_board[plug_board_location];

		for (int base_iterator = 0; base_iterator < alphabet_normal_length; base_iterator++) {
			if (character == Enc_Dec_plug_board[base_iterator]) {
				base_location = base_iterator;
				break;
			}
		}
		character = base[base_location];
		//************************End Plug board and encyption and decryption of a character***************\\|
		//append a new character to the output string
		output = output + character;
		//Reset Variables for good measure
		base_location = 0;
		rotor1_position = 0;
		rotor2_position = 0;
		rotor3_position = 0;
	}
	//Format the output in the German 5 character block Style, define a return string
	std::string return_string = "";
	for (int i = 0; i < output.length(); i++) {
		if (i % 5 == 0 && i != 0) {
			return_string = return_string + " ";
		}
		return_string = return_string + output[i];
	}
	//reset the output string and return the encryption
	output = "";
	return return_string;// return_string;
}

std::string format_to_enigma_standard(std::string input_string) {
	std::string return_string = "";
	for (int i = 0; i < output.length(); i++) {
		if (i % 5 == 0 && i != 0) {
			return_string = return_string + " ";
		}
		return_string = return_string + input_string[i];
	}
	//reset the output string and return the encryption
	output = "";
	return return_string;
}

//Place Holder when an implementation can be adapted for multi-line input files    
std::string enigma_getline(std::string string_in) {
	return 0;
}

//************************************************ Bombe ************************************************\|
/*
Precondition: Pass in a string that is alpha only- run the sanatize_string function on the desired string first
Postcondition: A string will be returned

Function: Bombe was a cluster of Enigma machines that iterated through every possible combination the Enigma could
be set to. This program simulates that.
The function sets the Enigma settings with all possible configurations by use of nested loop permutations

Firstly, all possible rotor combinations
Next, the function will permute all possible plugboard combinations
Lastly, the function permutes all possible starting locations for each rotor- 1-25

after we reach the deepest level of the permutation- the third rotor setting, we then have the Enigma
settings fully configured for 1 possibility.

The encryption/decryption function is then called..
with the possible decryption returned from Enigma(string), the string is then searched for a keyword.

For test purposes, the test words will be "DERFUHRER" or "WETTERBERICHT"

*/
std::string Bombe(std::string sanatized_input) {
	std::string decryption_attempt = "";
	//sanatize just to make sure
	sanatized_input = sanatize(sanatized_input);

	//******ADD a loop to take user inputs for "keywords", add them to an array?

	std::cout << "Welcome to Bombe, I will begin my Attempt to Break your Message.\nWorking....." << std::endl;
													//Rotor 1\\
	//***************************************************************************************************************************************\|
	for (int R1 = 0; R1 <= Number_of_Rotors; R1++) {
		rotor1 = I; //&Rotors[R1];
		std::cout << "The Current Rotor in Position 1 is: " << rotor1 << std::endl;
													//Rotor 2\\
	//***************************************************************************************************************************************\|
	for (int R2 = 0; R2 <= Number_of_Rotors; R2++) {
		if (R2 != R1) {
			rotor2 = II;//&Rotors[R2];
			std::cout << "The Current Rotor in Position 2 is: " << rotor2 << std::endl;
		}
		else {
			continue;
		}
													//Rotor 3\\
	//***************************************************************************************************************************************\|
	for (int R3 = 0; R3 <= Number_of_Rotors; R3++) {
		if (R3 != R1 && R3 != R2) {
			rotor3 = III; // &Rotors[R3];
			std::cout << "The Current Rotor in Position 3 is: " << rotor3 << std::endl;
		}
		else {
			continue;
		}
				//********* End rotor Permutations, Continue with Plugboard and starting position *********\|

		//Begin Pair Permutations
	//***************************************************************************************************************************************\|
														//Pair 1\\
		//***************************************************************************************************************************************\|
		for (int pair11 = 0; pair11 <= 26; pair11) {
			for (int pair12 = 0; pair12 <= 26; pair12) {
				if (pair12 == pair11) {
					continue;
				}
				else {
					add_to_plugboard(pair11, pair12);

														//Pair 2\\
		//***************************************************************************************************************************************\|
		for (int pair21 = 0; pair21 <= 26; pair21) {
			//make sure the selected value is not already used
			if (pair21 == pair11 || pair21 == pair12) {
				continue;
			}
			else {
			//now to select the corresponding letter for pair 2
				for (int pair22 = 0; pair22 <= 26; pair22) {
					if (pair22 == pair11 || pair22 == pair12 || pair22 == pair21) {
						continue;
					}
					else {
						add_to_plugboard(pair21, pair22);
						
														//Pair 3\\
		//***************************************************************************************************************************************\|
		for (int pair31 = 0; pair31 <= 26; pair31) {
			//make sure the selected value is not already used
			if (pair31 == pair11 || pair31 == pair12 || pair31 == pair21 || pair31 == pair22) {
				continue;
			}
			else {
			//now to select the corresponding letter for pair 3
				for (int pair32 = 0; pair32 <= 26; pair32) {
					if (pair32 == pair11 || pair32 == pair12 || pair32 == pair21 || pair32 == pair22 || pair32 == pair31) {
						continue;
					}
					else {
						add_to_plugboard(pair31, pair32);

														//Pair 4\\
		//***************************************************************************************************************************************\|
		for (int pair41 = 0; pair41 <= 26; pair41) {
			//make sure the selected value is not already used
			if (pair41 == pair11 || pair41 == pair12 || pair41 == pair21 ||
				pair41 == pair22 || pair41 == pair31 || pair41 == pair32){
					continue;
			}
			else {
				//now to select the corresponding letter for pair 4
				for (int pair42 = 0; pair42 <= 26; pair42) {
					if (pair42 == pair11 || pair42 == pair12 || pair42 == pair21 || pair42 == pair22 ||
						pair42 == pair31 || pair42 == pair32 || pair42 == pair41){
						continue;
					}
					else {
						add_to_plugboard(pair41, pair42);

														//Pair 5\\
		//***************************************************************************************************************************************\|
		for (int pair51 = 0; pair51 <= 26; pair51) {
			//make sure the selected value is not already used
			if (pair51 == pair11 || pair51 == pair12 || pair51 == pair21 || pair51 == pair22 ||
				pair51 == pair31 || pair51 == pair32 || pair51 == pair41 || pair51 == pair42){
				continue;
			}
			else {
				//now to select the corresponding letter for pair 5
				for (int pair52 = 0; pair52 <= 26; pair52) {
					if (pair52 == pair11 || pair52 == pair12 || pair52 == pair21 || pair52 == pair22 ||
						pair52 == pair31 || pair52 == pair32 || pair52 == pair41 || pair52 == pair42 || pair52 == pair51){ 																					
						continue;
					}
					else {
						add_to_plugboard(pair51, pair52);

														//Pair 6\\
		//***************************************************************************************************************************************\|
		for (int pair61 = 0; pair61 <= 26; pair61) {
			//make sure the selected value is not already used
			if (pair61 == pair11 || pair61 == pair12 || pair61 == pair21 || pair61 == pair22 ||
				pair61 == pair31 || pair61 == pair32 || pair61 == pair41 || pair61 == pair42 ||
				pair61 == pair51 || pair61 == pair52){
				continue;
			}
			else {
				//now to select the corresponding letter for pair 6
				for (int pair62 = 0; pair62 <= 26; pair62) {
					if (pair62 == pair11 || pair62 == pair12 || pair62 == pair21 || pair62 == pair22 ||
						pair62 == pair31 || pair62 == pair32 || pair62 == pair41 || pair62 == pair42 ||
						pair62 == pair51 || pair62 == pair52 || pair62 == pair61){
						continue;
					}
					else {
						add_to_plugboard(pair61, pair62);
							
														//Pair 7\\
		//***************************************************************************************************************************************\|                            
		for (int pair71 = 0; pair71 <= 26; pair71) {
			//make sure the selected value is not already used
			if (pair71 == pair11 || pair71 == pair12 || pair71 == pair21 || pair71 == pair22 ||
				pair71 == pair31 || pair71 == pair32 || pair71 == pair41 || pair71 == pair42 ||
				pair71 == pair51 || pair71 == pair52 || pair71 == pair61 || pair71 == pair62){
				continue;
			}
			else {
				//now to select the corresponding letter for pair 7
				for (int pair72 = 0; pair72 <= 26; pair72) {
					if (pair72 == pair11 || pair72 == pair12 || pair72 == pair21 || pair72 == pair22 ||
						pair72 == pair31 || pair72 == pair32 || pair72 == pair41 || pair72 == pair42 ||
						pair72 == pair51 || pair72 == pair52 || pair72 == pair61 || pair72 == pair62 ||
						pair72 == pair71){
						continue;
					}
					else {
						add_to_plugboard(pair71, pair72);

														//Pair 8\\
		//***************************************************************************************************************************************\|
		for (int pair81 = 0; pair81 <= 26; pair81) {
			//make sure the selected value is not already used
			if (pair81 == pair11 || pair81 == pair12 || pair81 == pair21 || pair81 == pair22 ||
				pair81 == pair31 || pair81 == pair32 || pair81 == pair41 || pair81 == pair42 ||
				pair81 == pair51 || pair81 == pair52 || pair81 == pair61 || pair81 == pair62 ||
				pair81 == pair71 || pair81 == pair72){
				continue;
			}
			else {
				//now to select the corresponding letter for pair 8
				for (int pair82 = 0; pair82 <= 26; pair82) {
					if (pair82 == pair11 || pair82 == pair12 || pair82 == pair21 || pair82 == pair22 ||
						pair82 == pair31 || pair82 == pair32 || pair82 == pair41 || pair82 == pair42 ||
						pair82 == pair51 || pair82 == pair52 || pair82 == pair61 || pair82 == pair62 ||
						pair82 == pair71 || pair82 == pair72 || pair82 == pair81){
						continue;
					}
					else {
						add_to_plugboard(pair81, pair82);

														//Pair 9\\
		//***************************************************************************************************************************************\|
		for (int pair91 = 0; pair91 <= 26; pair91) {
			//make sure the selected value is not already used
			if (pair91 == pair11 || pair91 == pair12 || pair91 == pair21 || pair91 == pair22 ||
				pair91 == pair31 || pair91 == pair32 || pair91 == pair41 || pair91 == pair42 ||
				pair91 == pair51 || pair91 == pair52 || pair91 == pair61 || pair91 == pair62 ||
				pair91 == pair71 || pair91 == pair72 || pair91 == pair81 || pair91 == pair82){
				continue;
			}
			else {
				//now to select the corresponding letter for pair 9
				for (int pair92 = 0; pair92 <= 26; pair92) {
					if (pair92 == pair11 || pair92 == pair12 || pair92 == pair21 || pair92 == pair22 ||
						pair92 == pair31 || pair92 == pair32 || pair92 == pair41 || pair92 == pair42 ||
						pair92 == pair51 || pair92 == pair52 || pair92 == pair61 || pair92 == pair62 ||
						pair92 == pair71 || pair92 == pair72 || pair92 == pair81 || pair92 == pair82 ||
						pair92 == pair91){
						continue;
					}
					else {
						add_to_plugboard(pair91, pair92);

														//Pair 10\\
		//***************************************************************************************************************************************\|
		for (int pair101 = 0; pair101 <= 26; pair101) {
			//make sure the selected value is not already used
			if (pair101 == pair11 || pair101 == pair12 || pair101 == pair21 || pair101 == pair22 ||
				pair101 == pair31 || pair101 == pair32 || pair101 == pair41 || pair101 == pair42 ||
				pair101 == pair51 || pair101 == pair52 || pair101 == pair61 || pair101 == pair62 ||
				pair101 == pair71 || pair101 == pair72 || pair101 == pair81 || pair101 == pair82 ||
				pair101 == pair91 || pair101 == pair92){
				continue;
			}
			else {
				//now to select the corresponding letter for pair 10
				for (int pair102 = 0; pair102 <= 26; pair102) {
					if (pair102 == pair11 || pair102 == pair12 || pair102 == pair21 || pair102 == pair22 ||
						pair102 == pair31 || pair102 == pair32 || pair102 == pair41 || pair102 == pair42 ||
						pair102 == pair51 || pair102 == pair52 || pair102 == pair61 || pair102 == pair62 ||
						pair102 == pair71 || pair102 == pair72 || pair102 == pair81 || pair102 == pair82 ||
						pair102 == pair91 || pair102 == pair92 || pair102 == pair101){
						continue;
					}
					else {
						add_to_plugboard(pair101, pair102);
			//***************** End of Plugboard Permutations, continue with Rotor Start Locations *****************\|

												//Rotor 1 Start Location\\
		//***************************************************************************************************************************************\|
		for (int r1start = 0; r1start <= 26; r1start++) {
			rotor1_position = r1start;
				
												//Rotor 2 Start Location\\
		//***************************************************************************************************************************************\| 
		for (int r2start = 0; r2start <= 26; r2start++) {
			rotor2_position = r2start;

												//Rotor 3 Start Location\\
		//***************************************************************************************************************************************\|
		for (int r3start = 0; r3start <= 26; r3start++) {
			rotor3_position = r3start;				
			//***************** End of Rotor Start Location Permutations, continue with Mirror *****************\|                


		//Mirror Permutation\\
	//***************************************************************************************************************************************\|   
		for (int selected_mirror = 0; selected_mirror <= 2; selected_mirror) { //Mirrors.length() = 3, therefore 2
		   //assign a mirror from the mirrors array
			mirror = &Mirrors[selected_mirror];
			//***************** End of Mirror Permutation, continue with Final Configuration and decryption *****************\|

					//fill the plugboard "holes" with the normal letters- there should be 6 '*' replaced
			fill_plugboard();
				//Now we can finally attempt a decryption
			decryption_attempt = enigma(sanatized_input);
			std::cout << decryption_attempt << std::endl;
			//Now, using the possible words given in the keyword_array, we can check to see if any of
			//them are substrings of the decryption_attempt
			for (int key_word = 0; key_word <= number_of_keywords - array_index_shift; key_word++) {
				//If the keyword is a substring...
				if (decryption_attempt.find(keyword_array[key_word]) != std::string::npos) {
					//Display the possible decryption to the user,
					std::cout << "A Possible Decryption of the Message is: \n" << format_to_enigma_standard(decryption_attempt) << std::endl;
					//If the user deems it makes sense
					std::cout << "\n Is this an Acceptable Decryption, or Shall I Keep Trying?[y/n] ";
					char userin_pause_or_continue;
					std::cin >> userin_pause_or_continue;
					if (userin_pause_or_continue == 'y' || userin_pause_or_continue == 'Y') {
						//return this decryption to the main()
						return decryption_attempt;
					}
					else {
						//If the user deemed this match to be wrong, keep trying from where we left off
						//***** THIS RESET MAY NOT BE NEEDED******
						//Reset the values of the plugboard for the next decryption attempt
						reset_plugboard();
						continue;
					}
					//else {
						//continue to the next combination}
				}
			}
			//Reset the values of the plugboard for the next decryption attempt
			reset_plugboard();
		}
		// Close the Rotor Start Location Permutations                                        
		}}}
	// Close all of the Character Pair Permutations                           
	}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
	//Close the Rotor Combination Permutations
	}}}
	//Close the Function
	std::cout << "I am sorry, Dave. I am Unable to Crack this Message" << std::endl;
	std::string failed = "failed to find a possible decryption";
	return failed;
}
//*************************************************** End of Bombe ***********************************************************************\|

//************************************************** Bome Short **************************************************************************\|
//Essentially, bombe withought the plugboard

std::string BombeShort(std::string sanatized_input) {
	//set the rotors to the beginning
	std::string decryption_attempt = "";
	//sanatize just to make sure
	sanatized_input = sanatize(sanatized_input);

	//******ADD a loop to take user inputs for "keywords", add them to an array?

	std::cout << "Welcome to Bombe, I will begin my Attempt to Break your Message.\nWorking....." << std::endl;
													//Rotor 1\\
	//***************************************************************************************************************************************\|
	for (int R1 = 0; R1 <= Number_of_Rotors; R1++) {
		rotor1 = I;//Rotors[R1];
													//Rotor 2\\
		//***************************************************************************************************************************************\|
		for (int R2 = 0; R2 <= Number_of_Rotors; R2++) {
			if (R2 != R1) {
				rotor2 = II;//Rotors[R2];
			}
			else {
				continue;
			}
													//Rotor 3\\
		//***************************************************************************************************************************************\|
		for (int R3 = 0; R3 <= Number_of_Rotors; R3++) {
			if (R3 != R1 && R3 != R2) {
				rotor3 = III;//Rotors[R3];
			}
			else {
				continue;
			}
			//********* End rotor Permutations, Continue with Plugboard and starting position *********\|

	//***************** No Plugboard Permutation in the Short Bombe to Make it Easier *****************\|

												//Rotor 1 Start Location\\
	//***************************************************************************************************************************************\|
		for (int r1start = 0; r1start <= 26; r1start++) {
			rotor1_position = r1start;

												//Rotor 2 Start Location\\
	//***************************************************************************************************************************************\| 
		for (int r2start = 0; r2start <= 26; r2start++) {
			rotor2_position = r2start;

												//Rotor 3 Start Location\\
	//***************************************************************************************************************************************\|
		for (int r3start = 0; r3start <= 26; r3start++) {
			rotor3_position = r3start;
			
			//***************** End of Rotor Start Location Permutations, continue with Mirror *****************\|                

		//Mirror Permutation\\
	//***************************************************************************************************************************************\|   
		for (int selected_mirror = 0; selected_mirror <= 25; selected_mirror++) {//Mirrors.length()
			//assign a mirror from the mirrors array
			mirror = mirrorA; //Mirrors[selected_mirror];

		//***************** End of Mirror Permutation, continue with Final Configuration and decryption *****************\|

			//fill the plugboard "holes" with the normal letters- there should be 6 '*'s replaced
			//fill_plugboard(); Dont need this if the plugboard is given
			//Now we can finally attempt a decryption
			decryption_attempt = enigma(sanatized_input);
								
			if (hold == 0) {
				std::cout << decryption_attempt << std::endl;
				hold = 1;
			}
				
			//Now, using the possible words given in the keyword_array, we can check to see if any of
			//them are substrings of the decryption_attempt
			for (int key_word = 0; key_word <= number_of_keywords - array_index_shift; key_word++) {
				//If the keyword is a substring...
				if (decryption_attempt.find(keyword_array[key_word]) != std::string::npos) {
					//Display the possible decryption to the user,
					std::cout << "A Possible Decryption of the Message is: \n" << decryption_attempt << std::endl;
					//If the user deems it makes sense
					std::cout << "\n Is this an Acceptable Decryption, or Shall I Keep Trying?[y/n] ";
					char userin_pause_or_continue = ' ';
					std::cin >> userin_pause_or_continue;
					if (userin_pause_or_continue == 'y' || userin_pause_or_continue == 'Y') {
						//return this decryption to the main()
						return decryption_attempt;
					}
					else {
						//If the user deemed this match to be wrong, keep trying from where we left off
						//***** THIS RESET MAY NOT BE NEEDED******
						//Reset the values of the plugboard for the next decryption attempt
						//reset_plugboard(); dont touch the plugboard
						continue;
					}
					//else {
						//continue to the next combination}
				}
			}
			//Reset the values of the plugboard for the next decryption attempt
			reset_plugboard();//dont touch the plugboard
		}
		// Close the Rotor Start Location Permutations                                        
		}}}
		// Close all of the Character Pair Permutations                           
	}}}
	//Close the Function
	std::cout << "I am sorry, Dave. I am Unable to Crack this Message" << std::endl;
	std::string failed = "failed to find a possible decryption";
	return failed;
}

											//Bombe Helper Functions\\
//***************************************************************************************************************************************\|           
//add the plugboard letter combo from Bombe to the array            
char add_to_plugboard(char char_base, char char_plugboard) {
	//Search for the first char in the base

	for (int i = 0; i <= 25; i++) { //base.size() = 25, or 26 elements
									//when found, replace the appropriate char in the plugboard with the second char passed
		if (char_base == base[i]) {
			Enc_Dec_plug_board[i] = char_plugboard;
		}
	}
	return 0;
}

//add the *kosher* letters into the gaps, these are unused plugboard characters
void fill_plugboard() {

	for (int i = 0; i <= 25; i++) { //plug_board.size() = 25, or 26 elements
									//there should be 6 every time, as only 10 pairs are encrypted
		if (Enc_Dec_plug_board[i] == '*') {
			Enc_Dec_plug_board[i] = base[i];
		}
	}
}

// Reset the Plugboard after a decryption attempt was made
void reset_plugboard() {
	for (int i = 0; i <= 25; i++) { //plug_board.size() = 25, or 26 elements
		plug_board[i] = '*';
	}
}
//**************************End Bombe Helper Functions******************************\\
    
//*****************************End String Functions*********************************\\

int main() {
	std::cout << "       _________________________________________________________" << std::endl;
	std::cout << "      / ------------------------------------------------------- \\" << std::endl;
	std::cout << "     / /  ____    _____    _   ________   _______        ____  \\ \\" << std::endl;
	std::cout << "    / /  /  __|  / ___ \\  | | /  ____  |  |      \\      / _  \\  \\ \\" << std::endl;
	std::cout << "   / /  /  /    | |   \\ | | | | |    \\_|  | |\\ \\  \\    / / | |   \\ \\" << std::endl;
	std::cout << "  / /  |  /     | |   | | | | | |         | | | \\  \\  / / /  |    \\ \\" << std::endl;
	std::cout << " / /   | |___   | |   | | | | | |  _____  | | | || | / / /   |     \\ \\" << std::endl;
	std::cout << " |(    |  ___|  | |   | | | | | | |___  | | | | || | | |/ /| |      )|" << std::endl;
	std::cout << " \\ \\   | |      | |   | | | | | |     | | | | | || | |   / | |     / /" << std::endl;
	std::cout << "  \\ \\  |  \\     | |   | | | | | |     | | | | | || | |  /  | |    / /" << std::endl;
	std::cout << "   \\ \\  \\  \\__  | |   | | | | | |_____| | | | | || | | |   | |   / /" << std::endl;
	std::cout << "    \\ \\  \\____| |_|   |_| |_| \\_________/ |_| |_||_| |_|   |_|  / /" << std::endl;
	std::cout << "     \\ \\                                                       / /" << std::endl;
	std::cout << "      \\ ------------------------------------------------------/ /" << std::endl;
	std::cout << "       \\_______________________________________________________/" << std::endl;

	//list options
	std::cout << std::endl << "[1] Verschlusselung oder Entschlusselung(Encrypt/Decrypt from input)"
		<< std::endl << "[2] Verschlusselung von text(Encrypt/Decrypt From a File)"
		<< std::endl << "[3] Bombe, Crack an Enigma Message From Text"
		<< std::endl << "[4] Bombe, Crack an Enigma Message From a File"
		<< std::endl << "[5] Einstellungen(Settings)"
		<< std::endl << "[6] Ausgang" << std::endl;
	do {
		//get a user selected option
		std::cout << "Select an option:" << std::endl;
		std::cin >> option;
		switch (option) { //Detects the option
		case 1: {
			//simple in-terminal encryption decryption, take a string without spaces
			std::cout << "Enter a string without punctuation, numbers or special characters: \n";
			std::cin >> input;
			std::string sanatized_input = "";
			sanatized_input = sanatize(input);
			std::cout << "The Message reads: " << enigma(sanatized_input) << std::endl;
			break;
		}

		case 2: {
			//Give input output file names
			std::string input_file_name = "";
			std::string output_file_name = "";
			//take the user given .txt file name
			std::cout << "Wie heisst der Nam des Textes mit .txt: \n";
			std::cin >> input_file_name;
			//output .txt file
			std::cout << "Wo ist es Gehen mit .txt?: \n";
			std::cin >> output_file_name;

			//open the files
			std::ifstream inputFile(input_file_name.c_str());
			std::ofstream outputFile(output_file_name.c_str());

			//error check to ensure a file can be opened
			if (inputFile.is_open()) {
				//do case 1 for each line
				std::string sanatized_input = "";
				while (getline(inputFile, input)) {
					sanatized_input = sanatize(input);
					std::string out_string = enigma(sanatized_input);
					std::cout << "The message Reads: " << out_string << std::endl;
					outputFile << out_string;
					output.clear();
					input.clear();
					inputFile.close();
					outputFile.close();
				}
			}
			//if the file cannot be opened, tell me so!
			else {
				std::cout << "Keiner Text heisst dass" << std::endl;
				break;
			}

			break;

		}

				// Bombe From Input
		case 3: {
			//simple in-terminal Bombe, take a string without spaces, 
			//If the string contains the key words it will find it
			std::cout << "Enter the encrypted Enigma Message without punctuation, numbers or special characters: \n";
			std::cin >> input;
			std::string sanatized_input = "";
			sanatized_input = sanatize(input);
			std::cout << "The Chosen Message from Bombe Reads: " << BombeShort(sanatized_input) << std::endl;
			//Output the settings that worked? the should still be set
			break;

		}

				// Bombe From a File
		case 4: {
			//Give input output file names
			std::string input_file_name = "";
			std::string output_file_name = "";
			//take the user given .txt file name
			std::cout << "What is the File Name With .txt extentsion: \n";
			std::cin >> input_file_name;
			//output .txt file
			std::cout << "Where is the Decryption going with .txt?: \n";
			std::cin >> output_file_name;

			//open the files
			std::ifstream inputFile(input_file_name.c_str());
			std::ofstream outputFile(output_file_name.c_str());

			//error check to ensure a file can be opened
			if (inputFile.is_open()) {
				//do case 1 for each line
				std::string sanatized_input = "";
				while (getline(inputFile, input)) {
					sanatized_input = sanatize(input);
					std::string out_string = Bombe(sanatized_input);
					std::cout << "The Chosen Decryption from Bombe Reads: " << out_string << std::endl;
					outputFile << out_string;
					output.clear();
					input.clear();
					inputFile.close();
					outputFile.close();

					//Output the settings that worked? the should still be set
				}
			}

			//if the file cannot be opened, tell me so!
			else {
				std::cout << "No File Named That" << std::endl;
				break;
			}

			break;
		}

				//Settings Case DOES NOT WORK, under CONSTRUCTION :-) 
		case 5: {
			std::cout << "Welcome to the settings!" << std::endl;
			std::cout << "Would you like to:" << std::endl <<
				"[1] Set the Rotors" << std::endl <<
				"[2] Set the Rotor Starting Position?" << std::endl <<
				"[3] Set the Rotor Kick Values?" << std::endl <<
				"[4] Set mirror?" << std::endl <<
				"[2] Set the Switch Board Settings?" << std::endl <<

				"[7] Go Back" << std::endl;
			do {
				std::cin >> settings_option;

				switch (settings_option) { //Detects the option
				case 1: {
					std::cout << "Set Rotor 1's Start: " << std::endl;
					std::cin >> rotor1_start;

					std::cout << "Set Rotor 2's Start: " << std::endl;
					std::cin >> rotor2_start;

					std::cout << "Set Rotor 3's Start: " << std::endl;
					std::cin >> rotor3_start;
					break;
				}
				case 2: {
					std::cout << "Please enter 26, non-repeating, capital letters: " << std::endl;
					std::string plug_board_chars;
					std::cin >> plug_board_chars;
					for (int i = 0; i <= 25; i++) {
						Enc_Dec_plug_board[i] = plug_board_chars[i];
					}
					break;
				}
				case 3: {
					std::cout << "When will Rotor 1 increment?" << std::endl;
					std::cin >> rotor1_kick_value;
					std::cout << "When will Rotor 2 increment?" << std::endl;
					std::cin >> rotor2_kick_value;
					std::cout << "When will Rotor 3 increment?" << std::endl;
					std::cin >> rotor3_kick_value;
					break;
				}
				default: {
					std::cout << "Invalid Option!";
					system("PAUSE");
					break;
				}
				}
			} while (settings_option != 7);
		}

		case 6: {
			return 0;
		}

		default: {//If user chooses anything else besides options given
			std::cout << "Invalid Option!";
			system("PAUSE");
			break;
		}
		}

	} while (option != 4);

	system("PAUSE");
	return 0;
}


