#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void printBinary(signed long int decimal, signed long int bits){
  for (unsigned long int i = bits-1; i!=-1; i--) {
    unsigned long int mask = 1lu<<i;
    mask = mask & decimal;
    mask = mask>>(i);
    printf("%ld", mask);
  }
  printf("\n");
}

signed long int roundMantissa(signed long int mantissa){
  signed long int mask = 0b1111 & mantissa; //last four
  signed long int g = mask & 0b0100;
  signed long int s = mask & 0b0010;
  signed long int r = mask & 0b0001;

  if(g==0) mask = (0b1000 & mantissa) | 0b001;
  if(g==1 && (r==1 || s==1)) mask = (0b1000 & mantissa) | 0b111;

  return mask;
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
    if (number<0) {
      number=-(number);
      sign=1;
    }
    while(number>2){
      number = number/2.0;
      e++;
    }
    if(number>1 && number<2) number-=1;
    printf("e:%d\n", e);

    signed long int bias = (1lu << (exp_bits-1))-1;
    signed long int exponent = e + bias;

    //mantissa
    signed long int mantissa = 0b0;
    int i=0;
     while(number!=number-number){ //needs changing around
       number = number*2.0;
       if(number >= 1) {
         number-=1.0;
         mantissa = (mantissa<<1)|(1<<0); //adds 1
         i++;
         continue;
       }
       i++;
       mantissa = (mantissa<<1); //adds 0
     }

    //padding with zeros
    if (i!=frac_bits) {
      i=frac_bits-i;
      mantissa=mantissa<<i;
    }
    printf("mantissa: ");
    printBinary(mantissa,frac_bits+2);

    //rounding mantissa
    //mantissa = roundMantissa(mantissa) | (mantissa & 0b0000);
    //printf("rounded mantissa: ");
    //printBinary(mantissa,frac_bits);

    //answer = (sign<<(bits-1)) | mantissa;
    printf("sign: ");
    printBinary(sign,bits-(exp_bits+frac_bits));

    printf("exp: ");
    printBinary(exponent,exp_bits);

    //printBinary(sign,bits-(exp_bits+frac_bits));
    //printBinary(mantissa,frac_bits);
    //printBinary(exp,exp_bits);
    //printf("\n");
  }
  return EXIT_SUCCESS;
}
