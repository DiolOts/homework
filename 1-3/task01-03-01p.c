#include <math.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <ctype.h>

int main(int argc, char* argv[])
{
	FILE * pFile;
	if(argc == 1) {
    	printf("task01-03-01p.exe file_to_patch\n");
		system("pause");
		return EXIT_SUCCESS;
	}
	pFile = fopen(argv[1], "r+b");

	if (pFile != NULL) {
		fseek(pFile, 0x841, SEEK_SET);
		unsigned char newByte1 = 0x39;
		fwrite( &newByte1, sizeof( newByte1 ), 1, pFile );
		fseek(pFile, 0x842, SEEK_SET);
		unsigned char newByte2 = 0xC0;
		fwrite( &newByte2, sizeof( newByte2 ), 1, pFile );
		fclose(pFile);
		printf("File Patched Successfully.\n");
	}
	else
	{
		printf("File not Found.\n");
	}
	system("pause");
	return EXIT_SUCCESS;
}