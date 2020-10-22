#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void read(unsigned long int* result, unsigned long bits){
  for (size_t i = 0; i < bits; i++) {
    printf("%lu", result[i]);
  }
  printf("\n");
}

void DecimalToBinary(unsigned long int decimal, unsigned long bits){
  unsigned long int* result = calloc(bits,sizeof(unsigned long int*));
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
    result[i]=remainder;
    //printf("i%ld\n", i);
  }
  //printf("remainder: %d\n", remainder);
  //printf("quotient: %d\n", quotient);
  read(result,bits);
  free(result);
}

int main(int argc, char const *argv[argc+1]) {
  FILE* f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  unsigned long int decimal, bits;
  while(fscanf(f,"%lu %lu",&decimal,&bits)!=EOF){
    DecimalToBinary(decimal,bits);
  }

  return EXIT_SUCCESS;
}
