// PELoader.cpp: определяет точку входа для консольного приложения.
//


#define _UNICODE

#include "stdafx.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <DbgHelp.h>

using namespace std;


typedef struct
{
	string DllName;
	string FunctionName;
	DWORD FunctionAddress;

} FUNC_ITEM;

typedef struct _RELOCATION
{
	WORD Offset : 12;
	WORD Type : 4;
} *PRELOCATION;

typedef vector<FUNC_ITEM> FUNCS_ARRAY;


HMODULE getThisModuleHandle()
{
	//Returns module handle where this function is running in: EXE or DLL
	HMODULE hModule = NULL;
	::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)getThisModuleHandle, &hModule); 
	return hModule;
}

LPVOID readPe(LPCWSTR Filename, DWORD &Filesize)
{

	DWORD rd = 0;
	LPVOID Image = NULL;

	HANDLE Hwnd = CreateFile((LPCWSTR)Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//createfilemapping

	if (Hwnd != INVALID_HANDLE_VALUE)
	{
		Filesize = GetFileSize(Hwnd, NULL);
		Image = VirtualAlloc(NULL, Filesize, MEM_COMMIT, PAGE_READWRITE);
		if (Image)
		{
			if (!ReadFile(Hwnd, Image, Filesize, &rd, NULL))
			{
				VirtualFree(Image, Filesize, MEM_RELEASE);
				Filesize = 0;
				Image = NULL;

			}
			CloseHandle(Hwnd);
		}
		else
		{
			printf("\nERROR : Can`t VirtualAlloc %s\n------------------\n", Hwnd);
			return NULL;
		}
	}
	else
	{
		printf("\nERROR : Could not open the file specified: %ls\n", Filename);
		//printf("\Can`t CreateFile %s\n------------------\n", Filename);
	}
	return Image;
}

DWORD RVA2RAW(LPVOID FileImage, DWORD dwRVA)
{
	DWORD dwRawRVAAdr(0);
	IMAGE_DOS_HEADER* DOSHeader = PIMAGE_DOS_HEADER(FileImage);
	IMAGE_NT_HEADERS* NTHeader = PIMAGE_NT_HEADERS(DWORD(FileImage) + DOSHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSections = IMAGE_FIRST_SECTION(NTHeader);

	if (!pSections)
	{
		return dwRawRVAAdr;
	}

	while (pSections->VirtualAddress != 0)
	{
		if (dwRVA >= pSections->VirtualAddress && dwRVA < pSections->VirtualAddress + pSections->SizeOfRawData)
		{
			dwRawRVAAdr = (dwRVA - pSections->VirtualAddress) + pSections->PointerToRawData;
			break;
		}
		pSections++;
	}


	return dwRawRVAAdr;
}

char* getDllName(LPVOID FileImage, PIMAGE_IMPORT_DESCRIPTOR p)
{
	DWORD DllNameVA = p->Name;
	DWORD DllNameRAW = RVA2RAW(FileImage, DllNameVA);
	return (char*)((DWORD)FileImage + DllNameRAW);
}

void fixTHUNK(LPVOID PeImage, int imageDescrIndex, int thunkIndex, DWORD FunctionAddress)
{
	printf("Fix Thunk\n");

	printf("\tFunction name\tOld Address\tNew Address\n");


	IMAGE_DOS_HEADER* DOSHeader = PIMAGE_DOS_HEADER(PeImage);
	IMAGE_NT_HEADERS* NTHeader = PIMAGE_NT_HEADERS(DWORD(PeImage) + DOSHeader->e_lfanew);

	DWORD imageImportDescrVA = NTHeader->OptionalHeader.DataDirectory[1].VirtualAddress;
	DWORD imageImportDescrRAW = RVA2RAW(PeImage, imageImportDescrVA);

	PIMAGE_IMPORT_DESCRIPTOR fileImageImportDescr = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)PeImage + imageImportDescrVA);
	PIMAGE_IMPORT_BY_NAME importByNameImage;
	DWORD OriginalAddress;
	
	while (fileImageImportDescr->OriginalFirstThunk)
	{
		auto importedModuleName = (char*)(DWORD_PTR)(fileImageImportDescr->Name + (DWORD)PeImage);
		auto importedModule = GetModuleHandleA(importedModuleName);
		
		auto nameAddressPtr = (PIMAGE_THUNK_DATA)(DWORD_PTR)(fileImageImportDescr->OriginalFirstThunk + (DWORD)PeImage); 
		auto functionAddressPtr = (PIMAGE_THUNK_DATA)(DWORD_PTR)(fileImageImportDescr->FirstThunk + (DWORD)PeImage); 

		while (nameAddressPtr->u1.Function)
		{
			FARPROC importedFunctionPtr = NULL;
			if (nameAddressPtr->u1.Ordinal & IMAGE_ORDINAL_FLAG)
			{
				importedFunctionPtr = GetProcAddress(importedModule, MAKEINTRESOURCEA(nameAddressPtr->u1.Ordinal));
				importByNameImage = 0x0;
			}
			else
			{
				importByNameImage = (PIMAGE_IMPORT_BY_NAME)(DWORD_PTR)(nameAddressPtr->u1.AddressOfData + (DWORD)PeImage);
				importedFunctionPtr = GetProcAddress(importedModule, (char*)importByNameImage->Name);
			}

			if (importedFunctionPtr)
			{

				OriginalAddress = functionAddressPtr->u1.Function;
				auto oldProt = 0ul;
				VirtualProtect(functionAddressPtr, sizeof(IMAGE_THUNK_DATA), PAGE_EXECUTE_READWRITE, &oldProt);
				functionAddressPtr->u1.Function = (DWORD_PTR)importedFunctionPtr;
				printf("\t%s\t0x%08x\t0x%08x\r\n", importByNameImage->Name, OriginalAddress, functionAddressPtr->u1.Function);
			}
			nameAddressPtr++;
			functionAddressPtr++;
		}
		imageDescrIndex++;
		fileImageImportDescr++;
	}
	printf("\n------------------\n");
}


FUNCS_ARRAY fixImportTable(LPVOID FileImage, LPVOID PeImage)
{
	printf("Fix Import Table\n");
	printf("\tDll name\tFunction Name\n");
	
	HMODULE curDllModule;
	FUNC_ITEM func;
	FUNCS_ARRAY result;
	size_t rva = 0;

	int imageDescrIndex = 0, thunkIndex = 0;

	IMAGE_DOS_HEADER* DOSHeader = PIMAGE_DOS_HEADER(FileImage);
	IMAGE_NT_HEADERS* NTHeader = PIMAGE_NT_HEADERS(DWORD(FileImage) + DOSHeader->e_lfanew);

	DWORD imageImportDescrVA = NTHeader->OptionalHeader.DataDirectory[1].VirtualAddress;
	DWORD imageImportDescrRAW = RVA2RAW(FileImage, imageImportDescrVA);

	PIMAGE_IMPORT_DESCRIPTOR fileImageImportDescr = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)FileImage + imageImportDescrRAW);
	vector<string> DllList;

	for (PIMAGE_IMPORT_DESCRIPTOR p = fileImageImportDescr; *(DWORD *)p; p++, imageDescrIndex++)
	{
		if (!p->Characteristics || !p->Name)
		{
			break;
		}

		char *DllName = getDllName(FileImage, p);
		func.DllName.assign(DllName);
		int num = MultiByteToWideChar(0, 0, DllName, -1, NULL, 0);
		wchar_t *WideDllName = new wchar_t[num];
		MultiByteToWideChar(0, 0, DllName, -1, WideDllName, num);
		curDllModule = LoadLibrary(WideDllName);

		DWORD dllInfoThunkVA = p->Characteristics != 0 ? p->Characteristics : p->FirstThunk;
		DWORD dllThunkOffset = RVA2RAW(FileImage, dllInfoThunkVA);
		PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((DWORD)FileImage + dllThunkOffset);

		while (*(DWORD *)pThunk)
		{
			if (pThunk->u1.AddressOfData & 0xFFFF == 0x8000)
			{
				continue;
			}

			DWORD functionsVA = pThunk->u1.AddressOfData;
			DWORD functionsOffset = RVA2RAW(FileImage, functionsVA);
			LPSTR pFunction = (LPSTR)((DWORD)FileImage + functionsOffset +2);
			func.FunctionName.assign(pFunction);
			func.FunctionAddress = (DWORD)GetProcAddress(curDllModule, pFunction);
			printf("\t%s\t%s\n", DllName, pFunction);
			result.push_back(func);
			rva = (size_t)pThunk;
			pThunk++;
			thunkIndex++;
		}

	}
	printf("\n------------------\n");
	fixTHUNK(PeImage, 0, 0, 0);

	thunkIndex = 0;
	return result;

}


void FixRelocTable(LPVOID PeImage, IMAGE_NT_HEADERS* NTHeader)
{
	DWORD relocTableVAddr = NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	DWORD relocTableSize = NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
	DWORD Step = (DWORD)PeImage - NTHeader->OptionalHeader.ImageBase;
	PIMAGE_BASE_RELOCATION pImageBaseRelocation = PIMAGE_BASE_RELOCATION(LPBYTE(PeImage) + relocTableVAddr);
	if (relocTableSize) {
		DWORD x;
		PRELOCATION pReloc;

		printf("Relocation Table\n", pImageBaseRelocation->VirtualAddress);
		printf("\tVirtual Adress\tSize\tElements\n");

		while (pImageBaseRelocation->SizeOfBlock && pImageBaseRelocation->VirtualAddress) {
			x = (DWORD)PeImage + pImageBaseRelocation->VirtualAddress;
			DWORD countOfRelocs = (pImageBaseRelocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
			DWORD OldVAddr;
			printf("\t0x%0x\t\t%d\t%d\n", pImageBaseRelocation->VirtualAddress, pImageBaseRelocation->SizeOfBlock, countOfRelocs);
			pReloc = (PRELOCATION)(((DWORD)pImageBaseRelocation) + sizeof(IMAGE_BASE_RELOCATION));

			printf("\t\tOld Address\tNew Address\n");
			for (int i = 0; i < countOfRelocs; i++) {

				OldVAddr = *((UINT_PTR*)(x + pReloc->Offset));
				switch (pReloc->Type) {
				case IMAGE_REL_BASED_DIR64:
					*((UINT_PTR*)(x + pReloc->Offset)) += (DWORD)Step;
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					*((DWORD*)(x + pReloc->Offset)) += (DWORD)Step;
					break;
				case IMAGE_REL_BASED_HIGH:
					*((WORD*)(x + pReloc->Offset)) += HIWORD(Step);
					break;
				case IMAGE_REL_BASED_LOW:
					*((WORD*)(x + pReloc->Offset)) += LOWORD(Step);
					break;
				default:
					printf("\t\t\tUnkown reloc type\n");
					break;
				}

				printf("\t0x%08x\t0x%08x\n", OldVAddr, *((UINT_PTR*)(x + pReloc->Offset)));
				pReloc += 1;
			}
			pImageBaseRelocation = (PIMAGE_BASE_RELOCATION)((DWORD)pImageBaseRelocation + pImageBaseRelocation->SizeOfBlock);
		}
	}

	printf("\n------------------\n");
}


int main(int argc, char* argv[])
{
    
	printf("Welcome to PELoader Otus Reverse Engineering, Task 14\n");
	printf("\n------------------\n\n");

	int i;
	DWORD Filesize = 0;
	
	
	if (argc < 2)
	{
		printf("Please add Run Filename as argument\nExample: PELoader.exe RunFilename\n");
		system("pause");
		return 0;
	}
			
	const char * intFilename = argv[1];
	
	printf("\nFileName %s\n------------------\n", intFilename);
	int num = MultiByteToWideChar(0, 0, intFilename, -1, NULL, 0);
	wchar_t *Filename = new wchar_t[num];
	MultiByteToWideChar(0, 0, intFilename, -1, Filename, num);

	LPVOID FileImage = readPe(Filename, Filesize);
	if (FileImage == NULL)
	{
		printf("Run Filename not found.\nExample: PELoader.exe RunFilename\n");
		system("pause");
		return 0;
	}
	
	
	HMODULE hModule = getThisModuleHandle();
	printf("Base Address: 0x%08x\n", hModule);
	
	//parse DOS header
	IMAGE_DOS_HEADER* DOSHeader = PIMAGE_DOS_HEADER(FileImage);

	//parse NT Header
	IMAGE_NT_HEADERS* NTHeader = PIMAGE_NT_HEADERS(DWORD(FileImage) + DOSHeader->e_lfanew);

	LPVOID PeImage = VirtualAlloc(NULL, NTHeader->OptionalHeader.SizeOfImage, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	PIMAGE_SECTION_HEADER pSecHeader;

	//copy image headers

	//copy DOS/NT Header to ImageBAse
	MoveMemory(PeImage, FileImage, NTHeader->OptionalHeader.SizeOfHeaders);
	
	//copy all sections by VirtualAddress
	for (i = 0, pSecHeader = IMAGE_FIRST_SECTION(NTHeader); i < NTHeader->FileHeader.NumberOfSections; i++, pSecHeader++)
	{
		MoveMemory((PBYTE)((DWORD)PeImage + pSecHeader->VirtualAddress), (PBYTE)((DWORD)FileImage + pSecHeader->PointerToRawData), pSecHeader->SizeOfRawData);
	}
			
	// fix import table
	fixImportTable(FileImage, PeImage);
	
	// fix relocation table
	FixRelocTable(PeImage, NTHeader);

	// fix sections rights
	DWORD	dwOld;
	DWORD	dwEP;
	if (!VirtualProtect((LPVOID)PeImage, NTHeader->OptionalHeader.SizeOfImage,
		PAGE_EXECUTE_READWRITE, &dwOld)) {
		printf("Failed to change mapping protection");
		return FALSE;
	}
			
	// run application from memory 
	printf("Run Module!\n");
	
	DWORD peEntryPoint = (DWORD)PeImage + NTHeader->OptionalHeader.AddressOfEntryPoint;
	printf("Executing Entry Point: 0x%08x", peEntryPoint);

	__asm
	{
		mov eax, [peEntryPoint]
		jmp eax
	}
	
	system("pause");
	return 0;
}

