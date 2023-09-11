#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"
#include <stdlib.h>

#define ONE 	0b00110000
#define TWO 	0b01011011
#define THREE 	0b01001111
#define FOUR 	0b01100110
#define FIVE 	0b01101101
#define SIX 	0b01111101
#define SEVEN 	0b00000111
#define EIGHT 	0b01111111
#define NINE 	0b01100111
#define ZERO	0b00111111

#define SEGS_LO 4
#define SEGS_HI 2

#define ARR_SZ 8541
#define SLOWDOWN 500000

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

int convert_to_7seg(int input);
int display_7seg(int lo, int number, volatile int* hex_ptr);
void display_primes(volatile int* hex3_0_ptr, volatile int* hex_5_4_ptr);

/* This program increments the contents of the red LED parallel port */
int main(void)
{
   volatile int * LEDR_ptr;   // virtual address pointer to red LEDs

   int fd = -1;               // used to open /dev/mem for access to physical addresses
   void *LW_virtual;          // used to map physical addresses for the light-weight bridge
	volatile int* HEX3_HEX0_ptr;
	volatile int* HEX5_HEX4_ptr;
    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   LEDR_ptr = (unsigned int *) (LW_virtual + LEDR_BASE);

	//set va ptr to IO
	HEX3_HEX0_ptr = (unsigned int*) (LW_virtual + HEX3_HEX0_BASE);
	HEX5_HEX4_ptr = (unsigned int*) (LW_virtual + HEX5_HEX4_BASE);

	//*HEX3_HEX0_ptr = 0b111000000;

	//display 318
	//display_7seg(1, 12, HEX3_HEX0_ptr);
	//display_7seg(1, 52, HEX5_HEX4_ptr);
	
	display_primes(HEX3_HEX0_ptr, HEX5_HEX4_ptr);
	
    
   // Add 1 to the I/O register
   //*LEDR_ptr = *LEDR_ptr + 1;
    
   unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
   close_physical (fd);   // close /dev/mem
   return 0;
}

void display_primes(volatile int* hex3_0_ptr, volatile int* hex_5_4_ptr){
	int curr = 3;
	int* prime_list = malloc(sizeof(int) * ARR_SZ);

	if(prime_list == NULL) return;

	prime_list[0] = 2;
	int top = 1;

	while(1){
		int prime = 1;

		for(int i = 0; prime_list[i] * prime_list[i] <= curr && i < top; i++){
			if(curr % prime_list[i] == 0){
				curr++;
				prime = 0;
				break;
			}
		}
		if(prime)
		{
			volatile int slowdown = 0;

			for(int i = 0; i < SLOWDOWN; i++)
			{
				//just to slow down so we can actually see stuff
				//on the 7-seg
				slowdown++;
			}
 
			display_7seg(1, (curr % 10000), hex3_0_ptr);
			display_7seg(0, (curr / 10000), hex_5_4_ptr);	

			if(top < ARR_SZ){
				prime_list[top] = curr;
			}

			top++;
			curr+=2;
		}

		if (curr < 0)
		{
			curr = 3;

			free(prime_list);
			prime_list = malloc(sizeof(int) * ARR_SZ);

			if(prime_list == NULL) return;

			prime_list[0] = 2;
			top = 1;

		}

	}
}

int display_7seg(int lo, int number, volatile int* hex_ptr)
{
	if (lo)
	{
		int digit[SEGS_LO];

      for(int i = 0; i < SEGS_LO; i++)
      {
         digit[i] = ZERO;
      }

		int ctr = 0;
		int seg_num = 0;

		while(number > 0)
		{
			digit[ctr] = convert_to_7seg(number % 10);
			number /= 10;
			ctr++;
		}

		for (int i = 0; i < SEGS_LO; i++){
			seg_num += digit[i] << (8 * i);
		}

      *hex_ptr = seg_num;

		return 1;
	} else
	{

      int digit[SEGS_HI];

      for(int i = 0; i < SEGS_LO; i++)
      {
         digit[i] = ZERO;
      }

		int ctr = 0;
		int seg_num = 0;

		while(number > 0)
		{
			digit[ctr] = convert_to_7seg(number % 10);
			number /= 10;
			ctr++;
		}

		for (int i = 0; i < SEGS_HI; i++){
			seg_num += digit[i] << (8 * i);
		}

      *hex_ptr = seg_num;

		return 1;
	}
}

int convert_to_7seg(int input)
{
	switch(input)
	{
	case 0:
		return ZERO;
	case 1:
		return ONE;
	case 2:
		return TWO;
	case 3:
		return THREE;
	case 4:
		return FOUR;
	case 5:
		return FIVE;
	case 6:
		return SIX;
	case 7:
		return SEVEN;
	case 8:
		return EIGHT;
	case 9:
		return NINE;
	default:
		return -1;
	}
}
// Open /dev/mem, if not already done, to give access to physical addresses
int open_physical (int fd)
{
   if (fd == -1)
      if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1)
      {
         printf ("ERROR: could not open \"/dev/mem\"...\n");
         return (-1);
      }
   return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical (int fd)
{
   close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
   void *virtual_base;

   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
      printf ("ERROR: mmap() failed...\n");
      close (fd);
      return (NULL);
   }
   return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;
}


