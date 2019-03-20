#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int GetPaswordHash (const char *EnteredPassword);
int GetLoginHash(const char *EnteredLogin);
char* GeneratePassword (int Passwordsize, int LoginHash);


int main ()
{
	srand(time(NULL));
	printf ("Welcome to KeyGen Otus Reverse Engineering, Task 1-3, part 2\n");
	printf ("Enter Login:");
	char strvar[100];
	fgets (strvar, 100, stdin);
	
	printf ("\n");
	GeneratePassword (20, GetLoginHash (strvar)& 0xff);
	system("pause");
	
	return 0;
}


int GetPaswordHash (const char *EnteredPassword)
{
	int i;
	int PasswordHashResult = 0;
	size_t PasswordLength = strlen (EnteredPassword);

	for (i = 0; i < PasswordLength; ++i)
		PasswordHashResult += EnteredPassword[i] ^ 0x99;
	return PasswordHashResult ;
}


int GetLoginHash(const char *EnteredLogin)
{
	unsigned int SecondPart; 
	unsigned int Result; 
	int LoginSymbol; 
	int i;
	int y;
	int count; 
	size_t LoginLength = strlen (EnteredLogin)-1;

	count = 0;
	Result = -1;
 
	for (y = 0; y < LoginLength; ++y)
	{
		LoginSymbol = EnteredLogin[y];
		
		Result ^= LoginSymbol;
		for ( i = 7; i >= 0; --i )
		{
			SecondPart = -(Result & 1);
			Result = SecondPart & 0xEDB88320 ^ (Result >> 1);
		}
	}
	return ~Result ;
}


char* GeneratePassword (int Passwordsize, int LoginHash)
{
	
	char Symbols [63] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *ach;
		
	size_t length = Passwordsize;
	char *Result = (char*)malloc(length+1 * sizeof(char));
	char *LastHash = (char*)malloc(length+1 * sizeof(char));
	int i;
		
	int min = 0; 
	int max = 20;
	int difference;
	int XorS;

	for(i = 0; i < length; i++) 
	{
				
		char randomletter = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"[rand () % 62];
		Result[i] = randomletter;
		if (i > 2)
		{
			difference = LoginHash - (GetPaswordHash (Result) & 0xff);
			XorS = ((Result[i]^0x99)+difference) & 0xff ^0x99;
				
			ach=strchr (Symbols,XorS);
				
			if (ach!=NULL)
			{
				Result[i] = XorS;
				printf ("Password: %s \n", Result);
				return Result;
			}
		}
		strcpy(LastHash,Result);
	}
	return Result;
}