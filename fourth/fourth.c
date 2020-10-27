#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int length=0;
int e=0;

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

void readBinary(unsigned long int decimal, unsigned long bits, int integer){
  unsigned long int* result = calloc(64,sizeof(unsigned long int*));
  //printf("decimal: %lu\n", decimal);
  unsigned long int quotient=decimal;
  unsigned long int remainder=0;
  for (size_t i = length-1; i!=-1; i--) {
    //printf("i%ld\n", i);
    remainder=quotient%2;
    //printf("remainder: %lu\n", remainder);
    quotient=quotient/2;
    //printf("quotient: %lu\n", quotient);
    if(quotient==0 && remainder==0) {
      break;}
    result[i]=remainder;
    //printf("i%ld\n", i);
  }
  //read(result,bits);
  //printf("remainder: %d\n", remainder);
  //printf("quotient: %d\n", quotient);

  int found=0,j=1;
  for (size_t i = 0; j<bits; i++) {
    //printf(" i:%ld ", i);
    if(result[i]==1 && found==0){
      if(i==0 && e==0 && integer==0) e=i-1;
      if(e==0 && integer==0) e=-(i+1);
      printf("%lu", result[i]);
      found=1;
      printf(".");
      continue;
    }else if(found==0) continue;
    printf("%lu", result[i]);
    j++;
  }
  printf(" %d\n",e);
  free(result);
}

unsigned long int DecimalToBinary(unsigned long int decimal, unsigned long bits){
  unsigned long int key = 0b0;
  //printf("decimal: %lu\n", decimal);
  unsigned long int quotient=decimal;
  unsigned long int remainder=0;
  for (size_t i = bits-1; i!=-1; i--) {
    //printf("i%ld\n", i);
    remainder=quotient%2;
    //printf("remainder: %lu\n", remainder);
    quotient=quotient/2;
    //printf("quotient: %lu\n", quotient);
    if(quotient==0 && remainder==0) {
      break;}
    if(remainder==1) key = (key<<1)|(1<<0); //adds 1
    if(remainder==0) key = (key<<1); //adds 0
    length++;
    //printf("i%ld\n", i);
  }
  return key;
  //printf("remainder: %d\n", remainder);
  //printf("quotient: %d\n", quotient);

}

unsigned long int Mantissa(unsigned long int key, double number){
  //printf("\n");

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
    //printf("integer_part%d\n", integer_part);
    //printf("product%f\n", product);
    //printf("number%f\n", number);
  }

  //printf("\nkey %lu\n", key);
  return key;
}

int main(int argc, char const *argv[argc+1]) {
  FILE* f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  double number = 0;
  int bits = 0;
  while(fscanf(f,"%lf %d",&number,&bits)!=EOF) {
    e=0;
    while(number>2){
      number = number/2.0;
      e++;
    }
    int integer_part = (int) number;
    double decimal_part = number - (double) integer_part;

    length=0;
    unsigned long int integer_binary = DecimalToBinary(integer_part,bits);
    unsigned long int result = Mantissa(integer_binary,decimal_part);
    readBinary(result,bits+1,integer_part); //adding one because of .

  }
  return EXIT_SUCCESS;
}
