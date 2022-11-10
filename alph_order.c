# ifndef HEADER_H
	#define HEADER_H
	#include "header.h"
#endif

/* In this file, we put the two auxiliary functions related to the sorting in alphabetical order. */

int alphabetical_order(char* strA, char*strB)
/*  tests which of two given strings is first in alphabetical order
	returns -1 if first one, 1 if second one 
	param strA : first string
	param strB : second string  */
{
	if(strcmp(strA, "")==0)
		return 1;
	else if (strcmp(strB, "")==0)
		return -1;
	int i=0, A_value, B_value;
	do
	{
		/* we transform everything in upper case to compare */
		if(strA[i]>='a' && strA[i]<='z')
			A_value=strA[i]-'a'+'A';
		else
			A_value=strA[i];
		if(strB[i]>='a' && strB[i]<='z')
			B_value=strB[i]-'a'+'A';
		else
			B_value=strB[i];
		i++;
	}while(i<MAX_STRING && (A_value==B_value) && A_value!='\0'&& B_value!='\0');
	if(A_value<=B_value )
		return -1;
	else 
		return 1;
		
	/*tests for the function  : 
		printf("ordres 1 : %d %d %d \n", alphabetical_order("aaab", "aab"), alphabetical_order("aaaa", "aaaab"), alphabetical_order("aaa", "aaa"));
	printf("ordres 2 : %d %d %d \n", alphabetical_order("aab", "bb"), alphabetical_order("Aaab", "aab"), alphabetical_order("Aaaa", "aaaab"));
	printf("ordres 3 : %d %d %d \n", alphabetical_order("Aaa", "aaa"), alphabetical_order("Aab", "bb"), alphabetical_order("aaab", "Aab"));
	printf("ordres 4 : %d %d %d \n", alphabetical_order("aaaa", "Aaaab"), alphabetical_order("aaa", "Aaaa"), alphabetical_order("aab", "Bb"));
	printf("ordres 5 : %d %d %d \n", alphabetical_order("Aaab", "Aab"), alphabetical_order("Aaaa", "Aaaab"), alphabetical_order("Aaa", "Aaa"));
	printf("ordres 6 : %d \n", alphabetical_order("Aab", "Bb"));
	printf("ordres 1b: %d %d %d \n", alphabetical_order("aab", "aaab"), alphabetical_order("ab", "a"), alphabetical_order("aaa", "aaa"));
	printf("ordres 2b : %d %d %d \n", alphabetical_order("bb", "aab"), alphabetical_order("aab", "Aaab"), alphabetical_order("aaaab", "Aaaa"));
	printf("ordres 3b : %d %d %d \n", alphabetical_order("aaa", "Aaa"), alphabetical_order("bb", "Aab"), alphabetical_order("Aab", "aaab"));
	printf("ordres 4b : %d %d %d \n", alphabetical_order("Aaaab", "aaaa"), alphabetical_order("Aaaa", "aaa"), alphabetical_order("Bb", "aab"));
	printf("ordres 5b : %d %d %d \n", alphabetical_order("Aab", "Aaab"), alphabetical_order("Aaaab", "Aaaa"), alphabetical_order("Aaa", "Aaa"));
	printf("ordres 6b : %d \n", alphabetical_order("Bb", "Aab"));
	*/
}

void sort_alphabetical_order(struct Book *book, int sort_choice)
/* sorts in an alphabetical order a the list of books given in parameter. For this, we browse the list, and for each book, compare the title or author with all the other
	books to find the first one in alphabetical order. We place it in first position and we continue
	param book : a struct Book array where the books will be stored in the good order
	param sort_choice : an int, if equals at 1, we sort by title, if not, by author. */
{
	int i;
	for(i=0;i<MAX_BOOKS;i++)
	{
		int j, min=i;
		/* in this loop, we are looking for the first ranked string starting from ith position */
		for(j=i+1;j<MAX_BOOKS; j++)
		{
			if (sort_choice==1)
			{
				if(alphabetical_order(book[min].title, book[j].title)==1)
					min=j;
			}
			else
			{
				if(alphabetical_order(book[min].author, book[j].author)==1)
					min=j;
			}
		}
		//we swap this minimum with the one in ith position  
		struct Book book_tmp = book[i];
		book[i]=book[min];
		book[min]=book_tmp;	
	}
}
