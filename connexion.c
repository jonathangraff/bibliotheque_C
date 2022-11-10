# ifndef HEADER_H
	#define HEADER_H
	#include "header.h"
#endif

/* In this file, we have the first menu to connect to the application or to create a new user and some funtions related to this menu. */

int connexion(struct User* user)
/* function used to connect to the library. The user will be stored in the variable user
param user : a struct User pointer where will be stored the user connected
return : an int which will tell if the connexion was successful (0) or not (-1) */
{
	char login[MAX_STRING], password_entered[MAX_STRING];
	printf("Login : ");
	scanf("%s", login); //asks for your login
	int n = get_nb_users();
	struct User * users = (struct User*)calloc(n,sizeof(struct User));
	get_list_of_users(users); // get the list of all users
	int login_index = is_login_in_users(login, users); //checks if the login is in the list of users or not
	if (login_index==-1)
	{
		printf("Votre login n'est pas connu, veuillez soit l'entrer à nouveau, soit créer un nouvel utilisateur\n");
		free(users);
		return -1;
	}
	printf("Mot de passe : ");
	scanf("%s", password_entered); //asks for the password
	char password_coded[MAX_STRING]; 
	strcpy(password_coded, users[login_index].password); //gets the password coded from the user
	char password_decoded[MAX_STRING];
	decode(password_decoded, password_coded, KEY);
	int nb_tries = 1;
	while(strcmp(password_entered,password_decoded)!=0 && nb_tries<MAX_TRIES_PASSWORD+1) //checks if the decoded password matches the entered password
	{
		nb_tries++; 
		printf("Erreur dans votre mot de passe, veuillez le rentrer a nouveau (%de essai sur %d): \n", nb_tries, MAX_TRIES_PASSWORD);
		printf("Login : %s\n", login);
		printf("Mot de passe : ");
		scanf("%s", password_entered); // another try if the previous didn't work
		decode(password_decoded, password_coded, KEY);
	}
	if(nb_tries<=MAX_TRIES_PASSWORD) //if it worked
	{
		*user = users[login_index];
		free(users);
		return 0; 
	}
	else
	{
		printf("Vous avez mal rentre %d fois votre mot de passe, retour au menu principal\n", MAX_TRIES_PASSWORD);
		free(users);
		return -1;
	}
}

void get_list_of_users(struct User users[])
/* returns the list of all the users in an array
param users : le tableau de Users a renvoyer */
{
	FILE *f=fopen(USERS_FILE, "r");
	if (f!=NULL)
	{
		int i = 0, size = 2*MAX_STRING+MAX_BOOKS_TEACHER*(MAX_DIGITS_ID+1)+3;
		char inter[size];
		while(fgets(inter, size, f)!=NULL)
		{
			if(sscanf(inter, "%s %s %d %s", users[i].login, users[i].password, &users[i].is_teacher, users[i].books_borrowed)<3)
				printf("Pb de lecture du fichier users - les données ne sont pas au bon format\n\n");
			i+=1;
		}
	}
	else
	{
		printf("Erreur de lecture des utilisateurs de la bibliotheque");
		exit(-1);
	}
	fclose(f);
}

int get_nb_users()
/* returns the list of all the users in an array
param users : le tableau de Users a renvoyer */
{
	int i = 0;
	FILE *f=fopen(USERS_FILE, "r");
	if (f!=NULL)
	{
		int size = 2*MAX_STRING+MAX_BOOKS_TEACHER*(MAX_DIGITS_ID+1)+3;
		char inter[size];
		while(fgets(inter, size, f)!=NULL)
			i+=1;
	}
	else
	{
		printf("Erreur de lecture des utilisateurs de la bibliotheque");
		exit(-1);
	}
	fclose(f);
	return i;
}

int is_login_in_users(char* login, struct User* users)
/* checks if the login already exists in the file
param login : a string to check
param users : the array of all users
return -2 if the login is empty, -1 if the login is not in the list of users, and the line (starting by 0) where the user already is if it is the case */
{
	if ((login[0])=='\0')
		return -2;
	int i;
	for (i =0;i<MAX_USERS && users[i].login!=NULL; i++)
	{
		if (strcmp(login,users[i].login)==0)
			return i;
	}
	return -1;
}


void create_new_user(void)
/* this function is used to create a new user in the database. It writes in the file "users.txt" a new line with the 
login, the password , the int is_teacher and the string of books borrowed, all separated with a space 
login is a string without spaces
password is a string without spaces
is_teacher is 0 if it is a student, 1 if it is a teacher
string is a string of the books' id separated by a semi-colon */
{
	clear_entries();
	char login[MAX_STRING]; //initialize a login string
	int login_index=-1; //this will remain at -1 as long as the login entered is not valid because with sapces or already used.
	do
	{		
		if(login_index>-1)	
			printf("Votre login est deja utilise, veuillez en choisir un nouveau\n");
		else if (strchr(login, ' ')!=NULL) //if there is a space in the login
			printf("Pas d'espace s'il vous plait !\n");
		printf("Entrez un login (pas d'espaces et maximum %d caracteres) : ", MAX_STRING);
		read_string(login, MAX_STRING); // asks for a login
		/* next three lines check if the login entered is already used or not */
		struct User users[MAX_USERS]={""}; 
		get_list_of_users(users);
		login_index = is_login_in_users(login, users);	
	}while (login_index!=-1 || strchr(login, ' ')!=NULL);
	printf("Votre login : %s\n", login);
	char password[MAX_STRING], password_confirm[MAX_STRING]; //initialize two strings where the password and the confirmation of password will be stored
	do
	{
		do
		{		
			do
			{		
				if (strchr(password, ' ')!=NULL) //if there is a space in the password
					printf("\nPas d'espace s'il vous plait !\n\n");				
				printf("Entrez un mot de passe (pas d'espace, au moins une majuscule, une minuscule et un chiffe, au minimum 8 caracteres et au maximum %d caracteres) : ", MAX_STRING);
				read_string(password, MAX_STRING); //asks for a password
			}while (strchr(password, ' ')!=NULL || strcmp(password, "")==0);
		}while(password_strong(password)!=1);
		printf("Merci de confirmer votre mot de passe : ");
		read_string(password_confirm, MAX_STRING); //asks for a confirmation to check if the password entered is correct
		if(strcmp(password, password_confirm)!=0)
		printf("\nVos deux mots de passe ne sont pas identiques.\n\n");
	}while(strcmp(password, password_confirm)!=0 );
	char answer; //asks if you are a teacher or not
	do{
		printf("Etes-vous un professeur ? (O/N)");
		scanf("%c", &answer);
		clear_entries();
	}while(answer!='o'&&answer!='O'&&answer!='n'&&answer!='N');
	/* stores  a line in the file "users.txt" with all the informations of the new user */
	FILE *f = fopen(USERS_FILE, "a");
	char password_coded[MAX_STRING]; //we code the password not to be seenable in the file
	code(password_coded, password, KEY);
	if (f!=NULL)
	{
		NEWLINE
		fputs(login, f);
		fputs(" ", f);
		fputs(password_coded, f);	
		fputs(" ", f);
		if (answer=='o' || answer=='O')
			fputs("1", f); // this is 1 if it is a teacher, 0 if a student
		else
			fputs("0", f);
		fputs(" ", f);
	}
	else
	{
		printf("Erreur de lecture des utilisateurs de la bibliotheque");
		exit(-1);
	}
	fclose(f);
}

