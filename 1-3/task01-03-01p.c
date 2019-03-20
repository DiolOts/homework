#include <math.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <ctype.h>

int main()
{

	FILE * pFile;
	pFile = fopen("CRACKME-31158-5470e7.EXE", "r+b");

	if (pFile != NULL) {
		fseek(pFile, 0x841, SEEK_SET);
		unsigned char newByte1 = 0x39;
		fwrite( &newByte1, sizeof( newByte1 ), 1, pFile );
		fseek(pFile, 0x842, SEEK_SET);
		unsigned char newByte2 = 0xC0;
		fwrite( &newByte2, sizeof( newByte2 ), 1, pFile );
		fclose(pFile);
	}
	else
	{
		printf("Cant Find File: CRACKME-31158-5470e7.EXE");
	}
	system("pause");
	return EXIT_SUCCESS;
}