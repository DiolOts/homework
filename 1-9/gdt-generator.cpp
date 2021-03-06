// Used for creating GDT segment descriptors in 64-bit integer form.

#include "stdafx.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <limits>
#include <string>
#include <conio.h>
#include <list>


struct GDT_para {
	uint32_t segment = 0; 
	uint32_t limit= 0; 
	uint16_t flag = 0;
	std::string title;
};

const int MAX = 10;



// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)

#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed

#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD

#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR

#define GDT_STACK_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWREXPD

#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD

#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR

#define GDT_STACK_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWREXPD

void create_descriptor_stroke(uint32_t base, uint32_t segment, uint32_t limit, uint16_t flag)
{
	uint64_t descriptor;

	// Create the high 32 bit segment
	descriptor = limit & 0x000F0000;         // set limit bits 19:16
	descriptor |= (flag << 8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
	descriptor |= (segment << 3) & 0x000000FF;         // set base bits 23:16
	descriptor |= (base >> 16) & 0x000000FF;
	descriptor |= base & 0xFF000000;         // set base bits 31:24

											 // Shift by 32 to allow for low part of segment
	descriptor <<= 32;

	// Create the low 32 bit segment
	descriptor |= base << 16;                       // set base bits 15:0
	descriptor |= limit & 0x0000FFFF;               // set limit bits 15:0

	printf("0x%.16llX\n", descriptor);
}

void create_descriptor(uint32_t base, uint32_t segment, uint32_t limit, uint16_t flag, std::string title = "")
{

	uint64_t descriptor;

	// Create the high 32 bit segment
	descriptor = limit & 0x000F0000;         // set limit bits 19:16
	descriptor |= (flag << 8) & 0x00F0FF00; // set type, p, dpl, s, g, d/b, l and avl fields
	if (flag == 0x4092)
	{
		descriptor |= 0x0b & 0x000000FF;
	}
	else
	{
		descriptor |= (segment << 3) & 0x000000FF;         // set base bits 23:16
	}
	
	descriptor |= (base >> 16) & 0x000000FF;
	descriptor |= base & 0xFF000000;         // set base bits 31:24

											 // Shift by 32 to allow for low part of segment
	descriptor <<= 32;

	// Create the low 32 bit segment
	descriptor |= base << 16;                       // set base bits 15:0
	descriptor |= limit & 0x0000FFFF;               // set limit bits 15:0

	if (flag == 0x4092)
	{
		descriptor |= 0x80000000;
	}

	/*
	printf("Full descriptor: 0x%.16llX\n", descriptor);
	
	printf("Acess:%d\n", flag);
	printf("Acess:0x%X\n", flag);

	printf("Segment:%d\n", segment);
	printf("Limit:%d\n", limit);
	printf("Flag:%d\n", flag);
	printf("Title:%s\n", title.c_str());
	
	*/

	printf("%s\n", title.c_str());
	printf("\tdw 0x%.4llX\n", descriptor & 0x000000000000FFFF);
	printf("\tdw 0x%.4llX\n", (descriptor & 0x00000000FFFF0000) >> 16);
	printf("\tdw 0x%.4llX\n", (descriptor & 0x0000FFFF00000000) >> 32);
	printf("\tdw 0x%.4llX\n", descriptor >> 48);


}


void wait()
{
	int c;
	printf("\nPress a key to continue...");
	c = _getch();
	if (c == 0 || c == 224) _getch();
}

int
main(void)
{
	GDT_para list[MAX];

	

	std::string climit;
	std::string ring;
	std::string type;
	int check = 0;

	char another = 'y';

	list[0].segment = 0;
	list[0].limit = 0;
	list[0].flag = 0;
	list[0].title = "GDT:";
	//
	
	int i = 1;


	std::cout << "Welcome to Global Descriptor Table Generator, Otus Reverse Engineering, Task 1 - 9\n\n";


	while (i < MAX)
	{
		std::cout << "Input Type Of Descriptor: 1 = code, 2 = data, 3 = stack: ";
		std::cin >> type;
		std::cout << "\nInput Ring: 0 = global, 3 = user: ";
		std::cin >> ring;
		std::cout << "\nInput Descriptor Size: ";
		std::cin >> climit;

		list[i].segment = i;
		list[i].limit = static_cast<unsigned int>(std::stoul(climit, 0 , 16));

		check = std::stoi(ring);
		if (check == 3)
		{
			check = std::stoi(type);
			switch (check)
			{
			case 2:
				list[i].flag = GDT_DATA_PL3;
				list[i].title = "DATA_USER_DESC:";
				break;
			case 3:
				list[i].flag = GDT_STACK_PL3;
				list[i].title = "STACK_USER_DESC:";
				break;
			default:
				list[i].flag = GDT_CODE_PL3;
				list[i].title = "CODE_USER_DESC:";
			}
			
		}
		else
		{
			check = std::stoi(type);
			switch (check)
			{
			case 2:
				list[i].flag = GDT_DATA_PL0;
				list[i].title = "DATA_DESC:";
				break;
			case 3:
				list[i].flag = GDT_STACK_PL0;
				list[i].title = "STACK_DESC:";
				break;
			default:
				list[i].flag = GDT_CODE_PL0;
				list[i].title = "CODE_DESC:";
			}
		}
		
		std::cout << "\nAdd another Descriptor? y/n ";
		std::cin >> another;
		if (another == 'n') break;
		std::cout << "\n";
		i++;
	}

	for (int y = 0; y <= i; y++) 
	{
		create_descriptor(0, list[y].segment, list[y].limit, list[y].flag, list[y].title);
	}
	create_descriptor(0, 0x0b, 0xFFFF, 0x4092, "VIDEO_DESC:");
	wait();
	return 0;
}

