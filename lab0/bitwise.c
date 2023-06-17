#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
	return binstr;
}

void print_usage(char *name) {
	printf("Usage: %s <action> <byte> <bit no>\n", name);
	printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
		"\t\t <byte> 8-bit integer in hexadecimal\n"
		"\t\t <bit no> of bit to operate on\n");
}

int main(int argc, char *argv[])
{
	char a;	// action: 'h', 'l', 'r', 's', 't'
	unsigned long n; // value to convert must be smaller than 256
	int bit; // bit to operate on: must be between 0 an 7
    //validate if there's 4 arguments
    if (argc!=4){ 
        print_usage(argv[0]);
        return 1;
    }
    //validate first argument
    a=argv[1][0]; //a string is a null terminated character array
    if (a!='h' && a!='l' && a!='r' && a!='s' && a!='t'){
        print_usage(argv[0]);
        printf("first argument must be h or l or r or s or t");
        return 1;
    }
    //validate second argument
    n=strtoul(argv[2],NULL,16);
    if (n>(1<<8)-1){
        print_usage(argv[0]);
        printf("second argument must be between 0 and 256");
        return 1;
    }
    bit=atoi(argv[3]);
    if (bit<0 || bit>7){
        print_usage(argv[0]);
        printf("third argument must be between 0 and 7");
        return 1;
    }


    char binary[9]; // array for binary representation of n,
					//  remember that in C strings are terminated with a 0
    byte2bin(n,binary);
    printf("Initial value: %s\n", binary); 
    printf("Action: %c\n", a);
    printf("Bit: %d\n\n", bit);
    switch (a)
    {
    case 'h':
        if (n & BIT(bit)){
            printf("Bit %d is 1\n", bit);
        }
        else{
             printf("Bit %d is not 1\n", bit);
        }
        return 0;
    case 'l':
        if (!(n & BIT(bit))){
            printf("Bit %d is 0\n", bit);
        }
        else{
             printf("Bit %d is not 0\n", bit);
        }
        return 0;
    case 's':
        n|=BIT(n);
        break;
    case 'r':
        n&=~BIT(bit);
        break;
    case 't':
        n^=BIT(bit);
    }
    byte2bin(n,binary);
    printf("Final result: %s",binary);
    return 0;
    
	// Validate command line arguments
	
	// Print to stdout the binary representation of n
	
	// Do what the user asked and print the result
	
    return 0;
}

