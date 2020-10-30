#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int length=0;

void printBinary(signed long int decimal, signed long int bits, int count){
  if(count!=0){
    for (signed long int i = count-1; bits!=0; i--) {
      if(i<0) {
        bits--;
        printf("0");
        continue;}
      signed long int mask = 1lu<<i;
      mask = mask & decimal;
      mask = mask>>(i);
      printf("%ld", mask);
      bits--;
    }

  }

  if(count==0){
    for (signed long int i = bits-1; i!=-1; i--) {
      signed long int mask = 1lu<<i;
      mask = mask & decimal;
      mask = mask>>(i);
      printf("%ld", mask);
    }

  }
}


signed long int roundMantissa(signed long int mantissa, signed long int bits, int count){
  unsigned long int last = 0b0;
  unsigned long int plus1 = 0b0;
  unsigned long int sticky = 1;
  if(count>bits){
    for (count-=1; bits!=0; count--) {
      //printf("countfirst: %d\n", count);

      last = ((1lu<<(count)) & mantissa) >> (count);
      plus1 = ((1lu<<(count-1)) & mantissa) >> (count-1);
      bits--;
    }
    unsigned long int temp = 0;
    //printf("count: %d\n", count);
    for (; count!=0; count--) {
      if((((1lu<<(count)) & mantissa) >> (count)) == 1) temp = (temp<<1)|(1<<0); //adds 1
      if((((1lu<<(count)) & mantissa) >> (count)) == 0) temp = (temp<<1); //adds 0
    }
    if(temp==0) sticky = 0;

  }

  //printf("last: %lu\n", last);
  //printf("plus1: %lu\n", plus1);
  //printf("sticky: %lu\n", sticky);
  if((last==1 && plus1==1) || (plus1==1 && sticky==1)){
    mantissa+=1;
  }

  if(count==0){
    return mantissa;
  }

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

    while(number>2){
      number = number/2.0;
      e++;
    }
    if(number>1) number-=1;
    decimal_part = number;
    signed long int exponent = e + bias;

    //printf("e:%d\n", e);

    //rightside
    signed long int mantissa = 0;
    int count = 0;
     while(decimal_part!=decimal_part-decimal_part){ //needs changing around
       if(count==33) break;
       //printf("%f\n", decimal_part);
       decimal_part = decimal_part*2.0;
       if(decimal_part >= 1) {
         decimal_part-=1.0;
         mantissa = (mantissa<<1)|(1<<0); //adds 1
         count++;
         continue;
       }
       count++;
       mantissa = (mantissa<<1); //adds 0
     }
     //printf("count: %d\n", count);
     //printf("mantissa: ");
     //printBinary(mantissa,count,0);

     //printf("sign: ");
     printBinary(sign,bits-(exp_bits+frac_bits),0);

     //printf("exponent: ");
     printBinary(exponent,exp_bits,0);

     mantissa = roundMantissa(mantissa,frac_bits,count);
     //printf("mantissa rounded: ");
     //printBinary(mantissa,count,0);

     //printf("mantissa actual: ");
     printBinary(mantissa,frac_bits,count);

     printf("\n");

  }

}
