#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define KEY "VIGENEREKEY" //the Vigenere key for coding the password in the file
#define TIME_ZONE 2 //we need to add two hours to GMT time
#define NB_DIGITS_TIME 11 // the maximum of digits allowed to write a time (in seconds since 1st of january 1970 (stamptime)
#define MAX_BOOKS_STUDENT 3 // the maximum of books allowed for a student to borrow
#define MAX_BOOKS_TEACHER 5 // the maximum of books allowed for a teacher to borrow
#define MAX_DIGITS_ID 10 // the maximum of digits allowed for a book id
#define STUDENT_TIME 120 //the amount in seconds allowed to a teacher to keep a book
#define TEACHER_TIME 180 //the amount in seconds allowed to a teacher to keep a book
#define MAX_STRING 80 //the maximum of caracters allowed in a string (for title, author and genre of the book and for login and password of the user
#define MAX_GENRE 20 //the max amount possible of genres
#define MAX_USERS 100 // the maximum amount of users of the application
#define MAX_BOOKS 200 //the maximum amount of books the library can hold
#define USERS_FILE "users.txt" //the file where the users are stored
#define BOOKS_FILE "books.txt" //the file where the books are stored
#define MAX_TRIES_PASSWORD 3 // the max amount authorized to try the password
#if defined(_WIN32) //macro to clear the console
    #define CLEAR_CONSOLE system("cls") // for Windows
#elif defined(__linux__)
	#define CLEAR_CONSOLE system("clear") // for Linux
#endif
#if defined(_WIN32) //macro to add a new line at the end of files
    #define NEWLINE fputs("\n", f); // for Windows, it is necessary
#elif defined(__linux__) // for Linux, it is not
#endif
struct User{
	char login[MAX_STRING];
	char password[MAX_STRING];
	int is_teacher;
	char books_borrowed[MAX_BOOKS_TEACHER*MAX_DIGITS_ID];
};
struct Book {
	int id;
	char title[MAX_STRING];
	char author[MAX_STRING];
	char genre[MAX_STRING];
	unsigned long time_borrowed;
	int is_free;	
};
void create_new_user(void);
int connexion(struct User*);
void get_list_of_users(struct User[]);
int is_login_in_users(char*, struct User*);
void clear_entries(void);
int read_string(char* , int );
void connected(struct User*);
void get_book_infos(struct Book*, char*);
void get_list_of_books(struct Book[]);
int get_nb_books(void);
int get_list_of_available_books(struct Book[]);
int get_books_borrowed(int*, char*);
void get_book_from_id(struct Book*, struct Book[], int);
void give_book_back(struct User*, int[], int);
void borrow_book(struct User*, struct Book[]);
void add_book(struct Book[], int);
char* create_book_string_remove(char*, int[], int, int);
char* create_book_string_add(char*, int);
void rewrite_user(struct User*, char*);
void change_book_status(int);
int get_last_id(void);
int alphabetical_order(char[], char[]);
void sort_alphabetical_order(struct Book*, int);
void print_hour(unsigned long);
int books_late(int *books_borrowed_id, struct Book[], int);
void code(char*, char[], char[]);
void decode(char*, char[], char[]);
int get_first_nb_car(int* , int* , char);
int book_already_in_library(char*, char*, struct Book[], int);
int get_nb_users(void);
int password_strong(char*);