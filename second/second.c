#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void read(signed long int* result, int bits){
  for (size_t i = bits-1; i!=-1; i--) {
    printf("%lu", result[i]);
  }
  printf("\n");
}

signed long int* DecimalToBinary(signed long int decimal, int bits, signed long int* result){
  signed long int quotient=decimal;
  signed long int remainder=0;
  for (size_t i = 0; i<bits; i++) {
    remainder=quotient%2;
    quotient=quotient/2;
    if(quotient==0 && remainder==0) {
      break;}
    result[i]=remainder;
  }
  return result;
}

signed long int* InvertBinary(signed long int* binary, int bits){

  for (size_t i = 0; i < bits; i++) {
    if(binary[i]==1) {
      binary[i]=0;
      continue;
    }else if(binary[i]==0){
      binary[i]=1;
    }
  }
  return binary;

}

int BinaryToDecimal(signed long int* binary, int bits){
  signed long int result=0;
  for (size_t i = 0; i < bits; i++) {
    result+=binary[i]<<i;
  }
  return result;
}

void TwosComplement(signed long int* result, signed long int decimal, int bits){
  decimal = (decimal*-2)/2;
  signed long int max = (1 << (bits-1));
  if(decimal>max) decimal=max;
  //printf("decimal:%ld\n", decimal);
  result = DecimalToBinary(decimal,bits,result);
  //read(result,bits);

  result = InvertBinary(result, bits);
  //printf("inverted: ");
  //read(result,bits);
  decimal = BinaryToDecimal(result, bits);
  //printf("%lu\n", decimal);
  decimal+=1;
  //read(result,bits);
  //printf("new:%lu\n", decimal);
  result = DecimalToBinary(decimal,bits,result);
  //read(result,bits);
  decimal = BinaryToDecimal(result, bits);
  //printf("-%lu: ",decimal);
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

  signed long int decimal;
  int bits;
  while(fscanf(f,"%lu %d",&decimal,&bits)!=EOF){
    signed long int* result = calloc(bits,sizeof(signed long int*));

    if(decimal<0) {
      TwosComplement(result,decimal,bits);
      continue;
    }

    signed long int max = (1 << (bits-1))-1;
    if(decimal>max) decimal=max;
    result = DecimalToBinary(decimal,bits,result);
    read(result,bits);
    free(result);
  }
}
