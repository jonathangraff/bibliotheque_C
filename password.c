# ifndef HEADER_H
	#define HEADER_H
	#include "header.h"
#endif

/* In this file, we put the auxiliary functions that we need for the password : to code and decode using the Vigenere Cipher and to check if it is strong enough. */

void code(char* coded, char word[], char key[])
/* this function will code the password entered by a new user so it cannot be read directly in the file "users.txt".
It uses the principle of the Vigenere Cipher. The key is in the variable "key" and needs to be only letters and numbers, no special caracters.
For the password, it won't code the special caracters, not to have caracters we don't want in the file, like spaces by example, 
but it will code letters normally, and digits too on the same principle as letters.
param coded : the output string
param word[] : a string to code
param key[] : the key of the Vigenere Cipher : needs not to have special caracters  */ 
{
	int i, n=strlen(word), n_k = strlen(key);
	for(i=0;i<n;i++)
	{
		int first_car_w, first_car_k, nb_car_w, nb_car_k;
		int res_w = get_first_nb_car(&first_car_w, &nb_car_w, word[i]);
		get_first_nb_car(&first_car_k, &nb_car_k, key[i%n_k]);
		if (res_w==1)
		{
			int shift = key[i%n_k]-first_car_k; //the shift in the alphabetical (or numerical) order
			coded[i]=(word[i]+shift-first_car_w)%nb_car_w+first_car_w; //shifts the letter (or digit) of the word
		}
		else
			coded[i] = word[i];
	}
	coded[i]='\0'; //to end the string
}

void decode(char* decoded, char word[], char key[])
/* this function will decode the password retrieved from the file using the Vigenere Cipher too. 
param decoded : the output string
param word[] : the string to decode
param key[] : the key of the Vigenere Cipher : needs not to have special caracters  */ 
{
	int i, n=strlen(word), n_k = strlen(key);
	for(i=0;i<n;i++)
	{
		int first_car_w, first_car_k, nb_car_w, nb_car_k;
		int res_w = get_first_nb_car(&first_car_w, &nb_car_w, word[i]);
		get_first_nb_car(&first_car_k, &nb_car_k, key[i%n_k]);
		if (res_w==1)
		{
			int shift = key[i%n_k]-first_car_k;
			decoded[i]=(word[i]-shift+3*nb_car_w-first_car_w)%nb_car_w+first_car_w;
		}
		else
			decoded[i] = word[i];
	}	
	decoded[i]='\0';
}

int get_first_nb_car(int* first_car, int* nb_car, char car)
/* this function checks the caracter "car", if it is a lower case letter, it puts 'a' in first_car and 26 in nb_car, 
if it is a upper case one, it puts 'A' in first_car and 26 in nb_car, 
if it is a number, it puts '0' in first_car and 10 i nb_car. It returns 1 in these cases, and -1 in all the other cases. */
{
	if(car>='a' && car<='z')
	{
		*nb_car = 26;
		*first_car = 'a';
		
		return 1;
	}
	else if(car>='A' && car<='Z')
	{
		*first_car = 'A'; 
		*nb_car = 26;
		return 1;
	}
	else if(car>='0' && car<='9')
	{
		*first_car = '0'; 
		*nb_car = 10;
		return 1;
	}
	else
		return -1;
}

int password_strong(char* password)
/* this function checks if the password is secure enough : one lower case letter, one upper case letter, one digit and 8 caracters at least 
param password : a string, the password to check
return : an int, 1 if it is strong enough, 0 if not */
{
	int lower=0, upper=0, digit=0, i;
	for(i=0;i<strlen(password);i++)
	{
		char c = password[i];
		if(c>='a' && c<='z')
			lower++;
		else if(c>='A' && c<='Z')
			upper++;
		else if(c>='0' && c<='9')
			digit++;
	}
	if (lower>=1 && upper>=1 && digit>=1 && strlen(password)>7)
		return 1;
	else
	{
		printf("\nVotre mot de passe n'est pas assez securise !\n\n");
		return 0;
	}
}
