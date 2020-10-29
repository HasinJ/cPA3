#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void printBinary(signed long int decimal, signed long int bits){
  for (unsigned long int i = bits-1; i!=-1; i--) {
    unsigned long int temp = 1lu<<i;
    temp = temp & decimal;
    temp = temp>>(i);
    printf("%ld", temp);
  }
  printf("\n");
}

int main(int argc, char const *argv[argc+1]) {
  FILE* f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  double number = 6.5;
  signed long int bits = 8;
  signed long int frac_bits = 4;
  signed long int exp_bits = 3;
  signed long int sign=0;
  signed long int answer=0b0;

  while(fscanf(f,"%lf %ld %ld %ld",&number,&bits,&frac_bits,&exp_bits)!=EOF) {
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

    signed long int bias = (1lu << (exp_bits-1))-1;
    signed long int exponent = e + bias;

    //mantissa
    signed long int mantissa = 0b0;
    int i=0;
    printf("%f\n", number);
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
    printBinary(mantissa,frac_bits);

    //tester
    /*
    int i=0;
    while(i!=5){ //needs changing around
      number = number*2.0;
      printf("%f\n", number);
      if(number >= 1) {
        number-=1.0;
        mantissa = (mantissa<<1)|(1<<0); //adds 1
        i++;
        continue;
      }
      mantissa = (mantissa<<1); //adds 0
      i++;
    }
    */


    //exponent
    mantissa = mantissa<<exp_bits;
    for (signed long int i = exp_bits-1; i!=-1; i--) {
      unsigned int temp = 1lu<<i;
      temp = (temp & exponent);
      if(temp<1) continue;
      mantissa = mantissa | temp;
    }


  /*
    for (signed long int i = bits-2; i!=(bits-2)-frac_bits; i--) {
      signed long int temp = 1lu<<i;
      temp = temp & mantissa;
      answer = answer | temp;
    }
  */

    answer = (sign<<(bits-1)) | mantissa;
    printf("answer: ");
    printBinary(answer,bits);

  /*
    for (signed long int i = frac_bits-1; i!=-1; i--) {
      signed long int temp = 1lu<<i;
      temp = temp & mantissa;
      temp = temp>>(i); //separates the grabbed binary
      printf("temp%ld\n", temp);
    }
  */

      //printf("%lu\n", mantissa);
  }
  return EXIT_SUCCESS;
}
