# ifndef HEADER_H
	#define HEADER_H
	#include "header.h"
#endif

/* In this file, we put all the auxiliary functions that we need to work on the books. */
int get_nb_books()
/* returns the number of books in the library and the list of them in the variable "books" 
param books : an array of struct Book where the list of books will be stored
return : the number of books in the list */
{
	int i = 0;
	FILE *f=fopen(BOOKS_FILE, "r");
	if (f!=NULL)
	{
		int size = 3*MAX_STRING+MAX_DIGITS_ID+5;
		char str_id[MAX_DIGITS_ID], str_is_free[1];
		char inter[size];
		while(fgets(inter, size, f)!=NULL) // reads every line of the file
			i++;
	}
	else
	{
		printf("Erreur de lecture de la liste des livres");
		exit(-1);
	}
	fclose(f);
	return i;
}

void get_list_of_books(struct Book books[])
/* returns the number of books in the library and the list of them in the variable "books" 
param books : an array of struct Book where the list of books will be stored
return : the number of books in the list */
{
	int i = 0, size = 3*MAX_STRING+MAX_DIGITS_ID+5;
	FILE *f=fopen(BOOKS_FILE, "r");
	if (f!=NULL)
	{
		char str_id[MAX_DIGITS_ID], str_is_free[1];
		char inter[size];
		while(fgets(inter, size, f)!=NULL) // reads every line of the file
		{
			get_book_infos(&books[i],inter); // gets the information of the lines in a struct Book
			i++;
		}
	}
	else
	{
		printf("Erreur de lecture de la liste des livres");
		exit(-1);
	}
	fclose(f);
}

void get_book_infos(struct Book* book,char* string)
/* Puts an objet Book in the book variable based on its line in the file books.txt 
param book : a pointer to a struct Book where the book will be stored 
param string : the string of the file "books.txt" */
{
	char str_id[MAX_DIGITS_ID], str_time[NB_DIGITS_TIME];
	int i, i_beg;
	/* we retrieve the id */
	for(i=0; string[i]!=';'; i++)
    	str_id[i] = string[i];
    str_id[i] = '\0';
	book->id=atoi(str_id);
	/* we retrieve the title */
	i++;
    i_beg=i;
    for(i; string[i]!=';'; i++)
		book->title[i-i_beg] = string[i];
	book->title[i-i_beg]='\0';
	/* we retrieve the author */
	i++;
    i_beg=i;
    for(i; string[i]!=';'; i++)
		book->author[i-i_beg] = string[i];
	book->author[i-i_beg] = '\0';
	/* we retrieve the genre */
	i++;
    i_beg=i;
    for(i; string[i]!=';'; i++)
		book->genre[i-i_beg] = string[i];
	book->genre[i-i_beg] = '\0';
	/* we retrieve the time it has been borrowed */
	i++;
    i_beg=i;
	for(i; string[i]!=';'; i++)
		str_time[i-i_beg]=string[i];
	str_time[i-i_beg]='\0';
	book->time_borrowed = atoi(str_time);
	book->is_free=string[i+1]-'0';
}

int get_books_borrowed(int* books_borrowed, char* str)
/* From the string "str" of the books id borrowed by the user, return in "books_borrowed" the list of books id borrowed
param books_borrowed : an array of integer where the ids of books borrowed will be
param str : the string from the file "books.txt" where the ids of the books borrowed are written 
return : the number of books borrowed */
{
	char str_tmp[MAX_DIGITS_ID];
	int i=0, i_beg=0, pos=0, len=strlen(str);
	while(i<len){
		for(i; str[i]!=';'&&i<len; i++)
		{ //as long as we don't meet the caracter ";"
			str_tmp[i-i_beg] = str[i];
		}
//				printf("aaa");
	    str_tmp[i-i_beg] = '\0';
	    books_borrowed[pos]=atoi(str_tmp);
	    i++;
	    i_beg=i;
	    pos++;
	}
	return pos;
}

void get_book_from_id(struct Book *book, struct Book books[], int id)
/* put the book of id "id" in the variable "book"
param book : a pointer of struct Book where the book will be stored
param books : the list of all books
param id : an int representing the id of the book we are looking for */
{
	int i=0;
	while(i<MAX_BOOKS)
	{
		if(books[i].id==id)
		{
			*book=books[i];
			break;
		}
		i++;
	}
}

int get_last_id()
/* this function gives the last id of the book in the "books.txt" file 
return : an int, the id found */
{
	int id;
	FILE *f=fopen(BOOKS_FILE, "r");
	int size = 3*MAX_STRING+MAX_DIGITS_ID+NB_DIGITS_TIME+6;
	if (f!=NULL)
	{
		
		char inter[size];
		while(fgets(inter, size, f)!=NULL)
		{
			sscanf(inter, "%d;", &id);
		}
	}
	else
	{
		printf("Erreur de lecture des livres de la bibliotheque");
		exit(-1);
	}
	fclose(f);
	return id;
}

int get_list_of_available_books(struct Book books[])
/*  returns the number of available books and the list of them in the variable "books" 
	param books : the array of books to fill 
	return : an int corresponding to the number of books stored in the array */
{
	int counter = 0, size = 3*MAX_STRING+MAX_DIGITS_ID+NB_DIGITS_TIME+6;
	FILE *f=fopen(BOOKS_FILE, "r");
	if (f!=NULL)
	{
		struct Book book;
		char inter[size];
		while(fgets(inter, size, f)!=NULL) //we read each line of the file
		{
			get_book_infos(&book,inter); //we extract the infos in the variable book
			if(book.is_free==1) // if the book is free, we add it to the list
			{
				get_book_infos(&books[counter],inter);
				counter++;
			}
		}
	}
	else
	{
		printf("Erreur de lecture de la liste des livres");
		exit(-1);
	}
	fclose(f);
	return counter;
}

int books_late(int *books_borrowed_id, struct Book books[], int time_allowed)
/*  returns the amount of books where the user is in late 
	param books_borrowed_id : an array of ints, the ids of the books borrowed 
	param books : the list of all books
	param time_allowed : the amount of time the user is allowed to keep a book
	return : an int, the number of books in late */
{
	unsigned int time_now = time(NULL);
	int nb_late = 0, i;
	for (i=0;i<MAX_BOOKS_TEACHER;i++)
	{
		if(books_borrowed_id[i]!=0)
		{
			struct Book book;
			get_book_from_id(&book, books, books_borrowed_id[i]);
			if(book.time_borrowed+time_allowed<time_now)
				nb_late++;
		}
	}
	return nb_late;	
}

char* create_book_string_remove(char* books_borrowed_str, int books[], int book_id, int N)
/* creates a new string with the books borrowed now by the user
   param books_borrowed_str : the final string
   param books : an array of int, the books' id borrowed now by the user
   param book_id : an int, the book we want to remove from the list
   param N : the max size of books_borrowed_string */ 
{
	int i;
	char str_tmp[MAX_DIGITS_ID+1];
	for (i=0; i<N; i++)
	{
		if(books[i]!=book_id)
		{
			sprintf(str_tmp, "%d;", books[i]);
			strcat(books_borrowed_str, str_tmp);
		}		
	}
	if(strlen(books_borrowed_str)!=0) // if the string is not empty, we add the final caracter \0 at the end
		books_borrowed_str[strlen(books_borrowed_str)-1]='\0';
}

char* create_book_string_add(char* books_borrowed_str, int book_id)
/* books is an array of ints, the books borrowed actually by the user
   book_id is a book we want to add to the list
   books_borrowed_string is the final string 
   N is the max size of books_borrowed_string */ 
{
	if(strcmp(books_borrowed_str, "")!=0)
		strcat(books_borrowed_str, ";");
	char str[MAX_DIGITS_ID];
	sprintf(str,"%d",book_id);
	//itoa(book_id, str, 10);
	strcat(books_borrowed_str, str);
}

void change_book_status(int book_id)
/* change the fact that a book is available or not in the file "books.txt" by modyfing the int is_free. If it is, sets the time to 0, if not, sets the time to the actual timestamp.
For doing this, we rewrite completely the file in a second one, copying the lines not about this book and rewrite the line of the book, changing the time and the int is_free.
Then we delete the first file, and rename the second one "Books.txt".
param book_id : the book id we want to change the status */
{
	FILE *f=fopen(BOOKS_FILE, "r");
	char* f2_name = "books_tmp.txt";
	FILE *f2=fopen(f2_name, "w");
	int size = 3*MAX_STRING+MAX_DIGITS_ID+NB_DIGITS_TIME+6;
	if (f!=NULL)
	{
		if (f2!=NULL){
			int id, is_free;
			char title[MAX_STRING], author[MAX_STRING], genre[MAX_STRING];
			time_t time_borrowed;
			char inter[size];
			while(fgets(inter, size, f)!=NULL) // we get here every line of the file
			{
				sscanf(inter, "%d;%[^;];%[^;];%[^;];%ld;%d", &id, title, author, genre, &time_borrowed, &is_free); //here we get each parameter that are placed between the semi-colons. 
				//%[^;] means that we take every caracter displayed, even the special ones like spaces, but not the semi-colons.
				if(id!=book_id)
					fputs(inter, f2);
				else
				{
					char s[size];
					time_t time_borrowed = time(NULL);
					if (is_free==1) // creation of the string to write in the file.
						sprintf(s, "%d;%s;%s;%s;%ld;%d\n", id, title, author, genre, time_borrowed, 0); 
					else
						sprintf(s, "%d;%s;%s;%s;%d;%d\n", id, title, author, genre, 0, 1);
					fputs(s, f2); // here it is written
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
		printf("Erreur de lecture des livres de la bibliotheque");
		exit(-1);
	}
	fclose(f);
	fclose(f2);
	remove(BOOKS_FILE);	
	rename(f2_name, BOOKS_FILE);
}

int book_already_in_library(char* title, char* author, struct Book books[], int n)
/* this function checks if the book the teacher wants to add already is in the library. It already is if the title and the author are the same than one of the books in the library
param title : a string, the title of the book the user wants to add
param author : a string, the author of the book the user wants to add
param books : a list of struct Book, all the books already in the library
param n : the size of the list of books
return : an int, 1 if the book already is in the library, 0 if not */
{
	int i, res=0;
	for(i=0;i<n;i++)
	{
		if(strcmp(title, books[i].title)==0 && strcmp(author, books[i].author)==0)
			res=1;
	}
	return res;	
};
