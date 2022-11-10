# ifndef HEADER_H
	#define HEADER_H
	#include "header.h"
#endif

/* In this file, we put all the auxiliary functions that we need in the program. */

void clear_entries(void)
/* when a scanf is done, there is a \n left in the input screen, so if we follow by a fgets, it stops directly because it reads directly the \n.
So we read here everything on the screen until we find a \n or an EOF */
{
	int c;
	do
		c = getchar();
	while(c != EOF && c != '\n');
}

int read_string(char* string, int size_limit)
/* reads a string in stdin by leaving the last \n caracter in the string and clearing the \n left in the input if the string read has a size bigger than size_limit
param string : the string to put the caracters read
param size_limit : an int, limit size of the string to read 
return -1 if there was an error in reading, 1 if not */
{
	char *position_newline = NULL;
		if (fgets(string, size_limit, stdin) != NULL)
		{
	    	position_newline = strchr(string, '\n');
	    	if (position_newline != NULL)
	            *position_newline = '\0';
	        else
		        clear_entries();
	    	return 1;
		}
	    else
	        return -1;
}

void print_hour(unsigned long t)
/* display the hour, minutes and seconds of the timestamp t. For this, we start by a modulo 86400 (the amount of seconds in a day), and then we look for the seconds, the minutes then the hours.
param t : the timestamp we want to know the hours, minutes and seconds */
{
	int time_now = t+3600*TIME_ZONE;
	int sec_day = time_now%86400;
	int sec = sec_day%60;
	int min = ((sec_day-sec)/60)%60;
	int h = (sec_day-sec-min*60)/3600;
	printf("%dh%02dm%ds",h, min, sec );
}


void rewrite_user(struct User* user, char* str)
/* rewrites the line of "user" in users.txt given the string of the books he borrowed.
For this, a second file is created, every line not about this user is copy-pasted, and the line of the user is completely rewritten with its informations and the good string.
At the end, we delete the first file, and rename the second one "users.txt".
param user : pointer to struct User representing the user connected
param str : a string representing the books the user borrowed, separated by a semi-colon */
{
	FILE *f=fopen(USERS_FILE, "r");
	char* f2_name = "users_tmp.txt";
	FILE *f2=fopen(f2_name, "w");
	int size = 2*MAX_STRING+MAX_BOOKS_TEACHER*(MAX_DIGITS_ID+1)+3;
	if (f!=NULL)
	{
		if (f2!=NULL){
			char login[MAX_STRING];
			char inter[size];
			while(fgets(inter, size, f)!=NULL) //for each line of the file
			{
				sscanf(inter, "%s ", login);
				if(strcmp(login, user->login)!=0) // if it is not the line of the user, we rewrite it
					fputs(inter, f2);
				else // if it is, we write login, password, is_teacher a,d the string of books borrowed, all separated with a space
				{
					char s[size];
					sprintf(s, "%s %s %d %s\n", user->login, user->password, user->is_teacher, str);
					fputs(s, f2);
				}
			}			
		}
		else
		{
			printf("Erreur d'ouverture du fichier temporaire %s", f2_name);
			exit(-1);
		}
	}
	else
	{
		printf("Erreur de lecture des utilisateurs de la bibliotheque");
		exit(-1);
	}
	fclose(f);
	fclose(f2);
	remove(USERS_FILE);	
	rename(f2_name, USERS_FILE);
}


