#include <stdio.h>
#include <stdlib.h>

int BITS;  // size of character



//function print in binary format
void PrintInBinary(unsigned n)
{
	short int iPos;
	
	for (iPos = (BITS -1) ; iPos >= 0 ; iPos--)
	{
	  (n & (1 << iPos))? printf("1"): printf("0");	
	}
	
}

//bit reversal function
unsigned int ReverseTheBits(unsigned int num)
{
    unsigned int count = (BITS -1); 
    unsigned int tmp = num;         //  Assign num to the tmp 
	     
    num >>= 1; // shift num because LSB already assigned to tmp
    
    while(num)
    {
       tmp <<= 1;  //shift the tmp because alread have the LSB of num  
	      
       tmp |= num & 1; // putting the set bits of num
       
       num >>= 1; 
       
       count--;
    }
    
    tmp <<= count; //when num become zero shift tmp from the remaining counts
    
    return tmp & ((1 << BITS) - 1);
}
 
int main()
{
    printf("Enter the log2 of n : ");
    scanf("%u",&BITS);
    
    for (int i = 0; i < (1 << BITS); i++) {
	    int j = ReverseTheBits(i);
    	if (i < j) printf("swap(%d, %d);\n", i, j);
    }
    return 0;
}
