# ifndef HEADER_H
	#define HEADER_H
	#include "header.h"
#endif

/* In this file, we put all the main functions that we need once connected. */

void add_book(struct Book books[], int n)
/* this function is used when a teacher wants to add a book in the library. It will ask for the title of the book, the author and the genre.
Then it will add a line in "books.txt" with an automatic id calculated by adding 1 to the last id, the title, the genre, the time the book has been borrowed (0 since it is not borrowed yet), 
and an int at 1 to say that the book is free, all separated by a semi-colon. So, no semi-colon can appear in the strings input. 
param books : an array of all books to check if the book added isn't already in it. In this case, we exit the function. 
param n : the size of the list of books*/
{
	char title[MAX_STRING];
	char author[MAX_STRING];
	char genre[MAX_STRING];
	clear_entries();
	do
	{
		if (strchr(title, ';')!=NULL) //if there is a semi-colon in the title
			printf("Pas de point-virgule s'il vous plait !\n\n");
		printf("Quel est le titre du livre a rajouter ?\n");

		read_string(title, MAX_STRING);
	}while (strchr(title, ';')!=NULL);
	do
	{
		if (strchr(author, ';')!=NULL) //if there is a semi-colon in the author
			printf("Pas de point-virgule s'il vous plait !\n\n");
		printf("Quel est l'auteur du livre a rajouter ?\n");
		read_string(author, MAX_STRING);
	}while (strchr(author, ';')!=NULL);
	if(book_already_in_library(title, author, books, n)==1)
	{
		printf("Ce livre est deja propose dans la bibliotheque, il n'est pas necessaire d'en avoir un deuxieme.\n\n");
		return;
	}
	do
	{
		if (strchr(genre, ';')!=NULL) //if there is a semi-colon in the genre
			printf("Pas de point-virgule s'il vous plait !\n\n");
		printf("Quel est le genre du livre a rajouter ?\n");
		read_string(genre, MAX_STRING);
	}while (strchr(genre, ';')!=NULL);
	FILE* f = fopen(BOOKS_FILE, "a"); // we open the file and put the cursor at the end
	int id=get_last_id();
	if (f!=NULL)
	{
		char book_string[3*MAX_STRING+MAX_DIGITS_ID+NB_DIGITS_TIME+6];
		fseek(f, -1, SEEK_CUR); // we put the cursor one step behind, so it is at the end of the last line
		sprintf(book_string, "%d;%s;%s;%s;0;1\n", id+1, title, author, genre); //we create the string to write
		fputs(book_string, f); // we write it
	}
	else
	{
		printf("Erreur de lecture des livres de la bibliotheque");
		exit(-1);
	}
	fclose(f);
	printf("\nLivre bien ajoute a la bibliotheque\n");
}

void give_book_back(struct User* user, int books[], int N)
/* "user" gives back a book
param user : pointer to struct User representing the user connected
param books : an array of books id representing the books borrowed by the user
param N : the amount of books borrowed */
{
	if(books[0]==0) // if there are no book
	{
		printf("Vous n'avez pas de livres a rendre !\n");
		return;
	}
	int book_number, book_id;
	if(N!=1) // if there are more than one book
	{
		printf("Lequel des %d livres voulez-vous rendre ? (1-%d)\n", N, N);
		scanf("%d", &book_number);
		clear_entries();
		book_id = books[book_number-1];
	}
	else // if there is only one book, we ask for confirmation
	{
		book_id = books[0];
		char answer;
		do
		{
			clear_entries();
			printf("Etes-vous sur de vouloir le rendre ? (O/N)\n");
			scanf("%c", &answer);
			
		}while(answer!='o'&&answer!='O'&&answer!='n'&&answer!='N');
		if (answer=='n' || answer=='N')
			return;
	}
	char books_borrowed_str[MAX_BOOKS_TEACHER*(MAX_DIGITS_ID+1)]=""; // will be a string with the books borrowed
	create_book_string_remove(books_borrowed_str, books, book_id, N); // creates the new string of the books borrowed after giving back one
	rewrite_user(user, books_borrowed_str); // rewrites in the file "users.txt" the part where the books borrowed are
	strcpy(user->books_borrowed,books_borrowed_str); // updates the books borrowed by the user in the struct User
	change_book_status(book_id); // changes in the file "books.txt" the fact that the book is now free
	printf("Vous avez rendu votre livre. Retour au menu\n\n");
}

void borrow_book(struct User* user, struct Book books[])
/* With this function, a user will borrow a book 
param user : pointer to struct User representing the user connected
param books : the list of all books */
{
	int books_borrowed_id[MAX_BOOKS_TEACHER]={0};
	int nb_books_owned = get_books_borrowed(books_borrowed_id, user->books_borrowed);
	if((user->is_teacher==1 && nb_books_owned == MAX_BOOKS_TEACHER) || (user->is_teacher==0 && nb_books_owned == MAX_BOOKS_STUDENT)) //if the user has the maximum books he has right to own, he cannot borrow one more
	{
		printf("\nVous avez deja emprunte le maximum de livres possibles, veuillez en rendre d'abord.\n\n");
		return;
	}
	int time_allowed;
	if (user->is_teacher==1)
		time_allowed = TEACHER_TIME;
	else
		time_allowed = STUDENT_TIME;
	int nb_books_late = books_late(books_borrowed_id, books, time_allowed); //gets the number of books in late for return, and forbids to book another one.
	if(nb_books_late>1)
	{
		printf("Vous avez %d livres en retard. Veuillez d'abord les rendre avant d'en emprunter de nouveaux !\n\n", nb_books_late);
		return;
	}
	else if (nb_books_late==1)
	{
		printf("Vous avez 1 livre en retard. Veuillez d'abord le rendre avant d'en emprunter de nouveaux !\n\n");
		return;
	}
	/* asks for the books the user wants to see */
	int book_choice, n, i, sort_choice, nb_book_null=0;
	do
	{
		printf("Que voulez-vous faire ?\n1. Afficher tous les livres disponibles\n2. Recherche par titre ou auteur\n3. Recherche par genre\n4. Retour\n");
		scanf("%d", &book_choice);
		clear_entries();
	}while(book_choice !=1 && book_choice !=2 && book_choice!=3 &&book_choice!=4);
	struct Book list_books[MAX_BOOKS]={0};
	n = get_list_of_available_books(list_books);
	// restriction of the list of books to the ones wanted
	if (book_choice==2)
	{
		char search[MAX_STRING];
		printf("Quelle est votre recherche ?\n");
		read_string(search, MAX_STRING);
		for(i=0;i<n;i++)
		{
			if(strstr(list_books[i].title, search) == NULL && strstr(list_books[i].author, search) == NULL ) //if the search is not in the title or author, we delete the title of the book in the array of books
				{
					strcpy(list_books[i].title, "");
					nb_book_null++;
				}
		}		
	}
	else if (book_choice==3)
	{
		/* displays all the available genres now in library */
		
		char genres[MAX_STRING][MAX_GENRE];
		char genre[MAX_STRING];
		int counter =0;
		printf("Voici les genres disponibles : \n");
		int j;
		/* displays the genres existing in the database */
		for(i=0;i<n;i++)
		{
			strcpy(genre, list_books[i].genre);
			j=0;
			while(j<MAX_GENRE)
			{
				if (strcmp(genre, genres[j])==0)
					break;
				j++;
			}
			if(j==MAX_GENRE)
			{
				strcpy(genres[counter], genre);
				counter++;
			}
		}
		strcpy(genres[counter], "");
		for(j=0;j<MAX_GENRE;j++){
			if (strcmp(genres[j],"")==0)
				break;
			printf("%s\n", genres[j]);
		}
		/* asks for the genre user is interested in */
		
		char search[MAX_STRING];
		printf("\nQuelle est votre recherche ?\n");
		read_string(search, MAX_STRING);
		for(i=0;i<n;i++)
		{
			if(strcmp(list_books[i].genre, search) !=0) //if the search is different of the genre looked for, we delete the title of the book in the array of books
				{
					strcpy(list_books[i].title, "");
					nb_book_null++;
				}
		}			
	}
	else if (book_choice==4)
		return;
	if(nb_book_null==n)
		printf("\nIl n'y a pas de livre disponible correspondant a votre recherche.\n\n");
	else
	{
		/* create a new list, where all the books not in the search and the ones not available won't be listed */
		struct Book list_books_blanks_end[MAX_BOOKS]={0};
		int j, cpt=0, nb_books, nb_book_chosen;
		for(j=0;j<MAX_BOOKS;j++)
		{
			if(strcmp(list_books[j].title, "")!=0 && list_books[j].is_free==1)
			{
				list_books_blanks_end[cpt]=list_books[j];
				cpt++;
			}
			nb_books = cpt;
		}
		/*asks for the method to sort the books, by title or by author */
		if(nb_books>=2)
		{
			do{
				printf("\nComment voulez-vous trier la liste ?\n1. Par titre\n2. Par auteur\n");
				scanf("%d", &sort_choice);
				clear_entries();
			}while(sort_choice !=1 && sort_choice !=2);
		}
		else
			sort_choice=1;
			
		sort_alphabetical_order(list_books_blanks_end, sort_choice);
		printf("\n***Voici la liste des livres disponibles a l'emprunt : ***\n\n");
		for(i=0;i<nb_books;i++)
			printf("%d) %s de %s (%s)\n", i+1, list_books_blanks_end[i].title,list_books_blanks_end[i].author, list_books_blanks_end[i].genre);
			
		/* asks for the book to choose */
		do{
			printf("\nEntrez le numero du livre choisi, 0 pour en emprunter aucun.\n");
			scanf("%d", &nb_book_chosen);
			clear_entries();
		}while(nb_book_chosen <0 || nb_book_chosen >nb_books);
		if (nb_book_chosen == 0)
		{
			printf("\nAucun livre emprunte. Retour au menu\n\n");
			return;
		}
		else
		{
			struct Book book_chosen = list_books_blanks_end[nb_book_chosen-1];
			create_book_string_add(user->books_borrowed, book_chosen.id);
			rewrite_user(user, user->books_borrowed);
			change_book_status(book_chosen.id);
			printf("\nVous venez d'emprunter le livre %s de %s (%s)\n\n", book_chosen.title, book_chosen.author, book_chosen.genre);
		}
	}
}













