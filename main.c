#include "connexion.c"
#include "connected.c"
#include "connected_functions.c"
#include "util_functions.c"
#include "password.c"
#include "alph_order.c"
#include "book_functions.c"

int main()
{
	int choice=0;
	CLEAR_CONSOLE;
	printf("************************************************\n");
	printf("*       Application Emprunt Bibliotheque       *\n");
	printf("*                                              *\n");
	printf("************************************************\n");
	do
	{
		do
		{
			printf("\n1. Se connecter\n2. Nouvel utilisateur\n3. Quitter l'application ?\n");
			scanf("%d", &choice);
		}
		while(choice != 1 && choice !=2 && choice !=3);
		if (choice==1)
			{
				struct User user;
				if(connexion(&user)!=-1)
					connected(&user);
			}
		else if (choice==2)
			create_new_user();
	}
	while(choice!=3);	
	printf("\nAu revoir !\n");
	return 0;
}