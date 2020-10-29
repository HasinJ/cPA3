#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int length=0;
int e=0;
unsigned long int mantissa = 0b0;

void read(unsigned long int* result, int bits){
  for (size_t i = 0; i<bits; i++) {
    if(result[i]==2) {
      printf(".");
      continue;
    }
    printf("%lu", result[i]);
  }
  printf("\n");
}

void Mantissa(unsigned long int number) {
  if(number==1) mantissa = (mantissa<<1)|(1<<0); //adds 1
  if(number==0) mantissa = (mantissa<<1); //adds 0
}

void readBinary(unsigned long int decimal, unsigned long bits, int integer){
  unsigned long int* result = calloc(64,sizeof(unsigned long int*));
  unsigned long int quotient=decimal;
  unsigned long int remainder=0;
  for (size_t i = length-1; i!=-1; i--) {
    remainder=quotient%2;
    quotient=quotient/2;
    if(quotient==0 && remainder==0) {
      break;}
    result[i]=remainder;
  }
  //read(result,bits);

  int found=0,j=1;
  for (size_t i = 0; j<bits; i++) {
    if(result[i]==1 && found==0){
      if(i==0 && e==0 && integer==0) e=i-1;
      if(e==0 && integer==0) e=-(i+1);
      printf("%lu", result[i]);
      found=1;
      printf(".");
      continue;
    }else if(found==0) continue;
    Mantissa(result[i]);
    printf("%lu", result[i]);
    j++;
  }
  printf(" %d\n",e);
  free(result);
}

unsigned long int DecimalToBinary(unsigned long int decimal, unsigned long bits){
  unsigned long int key = 0b0;
  unsigned long int quotient=decimal;
  unsigned long int remainder=0;
  for (size_t i = bits-1; i!=-1; i--) {
    remainder=quotient%2;
    quotient=quotient/2;
    if(quotient==0 && remainder==0) {
      break;}
    if(remainder==1) key = (key<<1)|(1<<0); //adds 1
    if(remainder==0) key = (key<<1); //adds 0
    length++;
  }
  return key;

}

unsigned long int DecimalPart(unsigned long int key, double number){
  while (number!=0.0) {
    double product;
    int integer_part;
    product = number*2.0;
    integer_part = (int) product;
    if(integer_part==1) {
      key = (key<<1)|(1<<0); //adds 1
      number = product-1.0;
    }else {
      key = (key<<1); //adds 0
      number = product;
    }
    length++;
  }

  return key;
}

int main(int argc, char const *argv[argc+1]) {
  FILE* f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  double number = 6.25;
  int frac_bits = 4;
  int exp_bits = 3;
  int sign=0;
  //while(fscanf(f,"%lf %d",&number,&bits)!=EOF) {
  e=0;
  if (number<0) sign=1;
  while(number>2){
    number = number/2.0;
    e++;
  }
  number-=1;
  unsigned long int bias = (1lu << (exp_bits-1))-1;
  unsigned long int exponent = e + bias;
  printf("number: %f e: %d exponent: %lu sign: %d\n", number, e, exponent,sign);

  unsigned long int mantissa = 0b0;
  while(number!=1.0){
    number = number*2.0;
    if(number > 1) {
      number-=1.0;
      mantissa = (mantissa<<1)|(1<<0); //adds 1
      continue;
    }
    mantissa = (mantissa<<1); //adds 0
  }
  while(frac_bits!=0){
    
    frac_bits--;
  }

    //printf("%lu\n", mantissa);
  //}
  return EXIT_SUCCESS;
}
