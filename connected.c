# ifndef HEADER_H
	#define HEADER_H
	#include "header.h"
#endif

/* In this file, we have the menu once the user is connected. */

void connected(struct User* user)
/* this function is the core of the program. Once the user is connected, he can navigate through the application
param user : a struct User pointer representing the user connected */
{
	if (user->is_teacher==1) // welcoming message
		printf("\nBienvenue Professeur %s \n\n", user->login);
	else
		printf("\nBienvenue Etudiant %s \n\n", user->login);
	
	while(1){ //will continue as long as user didn't choose to disconnect
		int n = get_nb_books();
		struct Book * books = (struct Book*)calloc(n,sizeof(struct Book));
		get_list_of_books(books);
		int books_borrowed_id[MAX_BOOKS_TEACHER]={0};
		get_books_borrowed(books_borrowed_id, user->books_borrowed);
		int i;
		/* displays the books borrowed at that moment and the time to return */
		if(books_borrowed_id[0]==0)
			printf("*********************************************************************************************\n\nVous n'avez emprunte aucun livre en ce moment\n\n");
		else
		{
			printf("*********************************************************************************************\n\nVous avez actuellement emprunte : \n");
			for (i=0;i<MAX_BOOKS_TEACHER && books_borrowed_id[i]!=0;i++){
				struct Book book;
				get_book_from_id(&book, books, books_borrowed_id[i]);
				unsigned long time_now = time(NULL);
				unsigned long time_borrowed = (unsigned long) difftime( time_now, book.time_borrowed );
				printf("%d) %s de %s (%s) ", i+1, book.title, book.author, book.genre);
				unsigned long time_allowed;
				if(user->is_teacher==1)
					time_allowed = TEACHER_TIME;
				else
					time_allowed = STUDENT_TIME;
				unsigned long time_left;
				unsigned long time_limit=book.time_borrowed + time_allowed;
				
/* these two following lines work, but not as soon as the book is borrowed, only once disconnected and reconnected, the problem seems to be the creation of the struct tm*. 
So I used a hand made function "print_hour" to get the hour, minutes and seconds from the timestamp. */
					
//				struct tm* time_limit_infos = localtime(& time_limit);
//				printf("a rendre pour %02ldh%02ldm%02lds ", time_limit_infos->tm_hour, time_limit_infos->tm_min, time_limit_infos->tm_sec);
				printf("a rendre pour ");
				print_hour(time_limit);
				if(time_borrowed>time_allowed)
					printf(" (EN RETARD !!)\n");
				else
				{
					time_left = time_allowed-time_borrowed;
					unsigned long secondes = time_left%60;
					unsigned long minutes = (time_left-secondes)/60 ;
					printf(" (dans %ld minutes et %ld secondes)\n", minutes, secondes);
				}			     
			}
			printf("\n");
		}
		/* asks for next action to do, teacher can add a book too */
		int choice;
		if (user->is_teacher==1)
		{
			do{
				printf("Que souhaitez-vous faire ?\n1. Emprunter un nouveau livre\n2. Rendre un livre\n3. Ajouter un livre a la bibliotheque\n4. Vous deconnecter\n");
				scanf("%d", &choice);
			}while(choice !=1 && choice !=2 && choice!=3 &&choice!=4);
			if(choice==1)
				borrow_book(user, books);		
			else if(choice==2)
				give_book_back(user, books_borrowed_id, i);
			else if(choice==3)
				add_book(books, n);
			else
				return;
		}
		else
		{
			do{
				printf("Que souhaitez-vous faire ?\n1. Emprunter un nouveau livre\n2. Rendre un livre\n3. Vous deconnecter\n");
				scanf("%d", &choice);
			}while(choice !=1 && choice !=2 && choice!=3);
			if(choice==1)
				borrow_book(user, books);		
			else if(choice==2)
				give_book_back(user, books_borrowed_id, i);
			else if(choice==3)
				return;
		}
	}
}







