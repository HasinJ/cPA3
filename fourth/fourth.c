#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void read(unsigned long int* result, int bits){
  for (size_t i = 0; i<bits; i++) {
    printf("%lu", result[i]);
  }
  printf("\n");
}

unsigned long int* DecimalToBinary(unsigned long int* result, int decimal,int bits){

  result = calloc(bits,sizeof(unsigned long int*));
  //printf("decimal: %d\n", decimal);
  unsigned long int quotient=decimal;
  unsigned long int remainder=0;
  for (size_t i = bits-1; i!=-1; i--) {
    //printf("i%ld\n", i);
    remainder=quotient%2;
    //printf("remainder: %ld\n", remainder);
    quotient=quotient/2;
    //printf("quotient: %ld\n", quotient);
    if(quotient==0 && remainder==0) {
      break;}
    result[i]=remainder;
    //printf("i%ld\n", i);
  }
  //printf("remainder: %ld\n", remainder);
  //printf("quotient: %ld\n", quotient);
  printf("left: ");
  read(result,bits);
  return result;
}

unsigned long int* DecimalToFraction(unsigned long int* result, double number,int bits){

  result = calloc(bits,sizeof(unsigned long int*));

  for (size_t i = bits-1; i!=-1; i--) {
    double product;
    int integer_part;
    product = number*2.0;
    integer_part = (int) product;
    result[i] = integer_part;
    if(integer_part==1) number=product-1.0;
    else number=product;
    //printf("%ld\n", i);
    if(product==1.0) break;
  }
  printf("right: ");
  read(result,bits);
  return result;
}

int main(int argc, char const *argv[argc+1]) {
  FILE* f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  double number=0;
  int bits=0;
  while (fscanf(f,"%lf %d",&number,&bits)!=EOF) {
    unsigned long int* mRight;
    int integer_part = (int) number;
    double decimal_part = number - (double) integer_part;
    mRight = DecimalToFraction(mRight,decimal_part,log(1/decimal_part)/log(2));

    unsigned long int* mLeft;
    int max=0, temp=1;
    while(max<number){
      max=(1<<(++temp-1))-1;
    }
    mLeft = DecimalToBinary(mLeft,integer_part,temp-1);
    free(mLeft);
    free(mRight);
  }
}
