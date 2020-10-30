#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int length=0;

void printBinary(signed long int decimal, signed long int bits){
  for (signed long int i = bits-1; i!=-1; i--) {
    signed long int mask = 1lu<<i;
    mask = mask & decimal;
    mask = mask>>(i);
    printf("%ld", mask);
  }
  printf("\n");
}

void printFrac(signed long int decimal, signed long int bits){
  if(length<bits) length = bits;
  for (signed long int i = length-1; i!=-1; i--) {
    if(bits==-1) break;
    signed long int mask = 1lu<<i;
    mask = mask & decimal;
    mask = mask>>(i);
    printf("%ld", mask);
    bits--;
  }
  printf("\n");
}

signed long int without1(signed long int decimal, signed long int bits){
  signed long int j;
  for (j = 32; j!=-1; j--) {
    signed long int mask = 1lu<<j;
    mask = mask & decimal;
    mask = mask>>(j);
    if(mask == 1) break;
  }
  j--;
  signed long int frac = 0b0;
  for (signed long int i = j; i!=-1; i--) {
    signed long int mask = 1lu<<i;
    mask = mask & decimal;
    mask = mask>>(i);
    if(mask==1) frac = (frac<<1)|(1<<0); //adds 1
    if(mask==0) frac = (frac<<1); //adds 0
    length++;
  }
  //printf("%d\n", length);
  return frac;
}

signed long int roundMantissa(signed long int mantissa,signed long int frac_bits){
  if(length==frac_bits) return mantissa;
  signed long int g = (mantissa & 0b0100)>>2;
  signed long int s = (mantissa & 0b0010)>>1;
  signed long int r = mantissa & 0b0001;

  if(g==1 && (r==1 || s==1)) mantissa += 1;

  return mantissa;
}

int main(int argc, char const *argv[argc+1]) {
  FILE* f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  double number = 0;
  signed long int bits = 0;
  signed long int frac_bits = 0;
  signed long int exp_bits = 0;
  signed long int sign=0;

  while(fscanf(f,"%lf %ld %ld %ld",&number,&bits,&exp_bits,&frac_bits)!=EOF) {
    int e=0;
    sign=0;
    length=0;
    if (number<0) {
      number=-(number);
      sign=1;
    }
    //printf("number: %f\n", number);
    signed long int bias = (1lu << (exp_bits-1))-1;
    signed long int integer_part = (int) number;
    double decimal_part = number - integer_part;
    //printf("%f\n", decimal_part);

    while(number>2){
      number = number/2.0;
      e++;
    }

    //printf("e:%d\n", e);

    //rightside
    signed long int frac = integer_part;
    signed long int count = 0;
     while(decimal_part!=decimal_part-decimal_part){ //needs changing around
       if(count==33) break;
       //printf("%f\n", decimal_part);
       decimal_part = decimal_part*2.0;
       if(decimal_part >= 1) {
         decimal_part-=1.0;
         frac = (frac<<1)|(1<<0); //adds 1
         count++;
         continue;
       }
       count++;
       frac = (frac<<1); //adds 0
     }

     //where it finds 1 on the rightside
     if (integer_part==0) {
       signed long int i = frac_bits;
       signed long int temp = 0;
       while(temp!=1){
         i--;
         if(i==frac_bits) break;
         temp = 1lu<<i & frac;
         temp=temp>>i;
       }
       e = i - frac_bits;
       //printf("new e:%d\n", e);
     }
     //printBinary(frac,32);
     //printf("\n");

     printf("sign: ");
     printBinary(sign,bits-(exp_bits+frac_bits));

     signed long int exponent = e + bias;
     printf("exponent: ");
     printBinary(exponent,exp_bits);

     printf("frac old: ");
     printBinary(frac,20);

     frac = without1(frac,frac_bits);
     printf("frac without 1: ");
     printBinary(frac,32);


     frac = roundMantissa(frac,frac_bits);
     printf("frac rounded: ");
     printBinary(frac,20);

     printf("frac new:");
     printFrac(frac,frac_bits);

     printf("\n");

  }

}
