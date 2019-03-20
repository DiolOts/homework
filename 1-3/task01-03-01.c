#include <math.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <ctype.h>

#define MAX_INPUT 100
int GetPassword(char *inputLogin);


int main(){
    
    printf ("Welcome to KeyGen Otus Reverse Engineering, Task 1-3, part 1\n");
    printf ("NOTE: Login must contain only alpha characters!\n");
    char value[MAX_INPUT] = ""; 
  
    printf("Enter Login:");
    fgets(value, 100, stdin); // Read input
    printf("\n");
    printf("Password: %d\n", GetPassword(value));
	
	system("pause");
    return EXIT_SUCCESS;
}




int GetPassword(char *inputLogin) 
{ 
    char *s = inputLogin;
    while (*s) 
	{
        *s = toupper((unsigned char) *s);
        s++;
    }
    int Result = 0; 
    
    size_t LoginLength = strlen (inputLogin)-1;

    int i;
    for (i = 0; i < LoginLength; ++i)
    {
        Result += (int) inputLogin[i];
    }

    Result = Result ^ 0x1234 ^ 0x5678;
    return Result;
}