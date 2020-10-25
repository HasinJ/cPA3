#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void read(signed long int* result, int bits){
  for (size_t i = bits-1; i!=-1; i--) {
    printf("%lu", result[i]);
  }
  printf("\n");
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

int BinaryToDecimal(signed long int* binary, int bits){
  signed long int result=0;
  for (size_t i = 0; i < bits; i++) {
    result+=binary[i]<<i;
  }
  return result;
}

signed long int* TwosComplement(signed long int* result, signed long int decimal, int bits){
  decimal = (decimal*-2)/2;
  signed long int max = (1 << (bits-1));
  if(decimal>max) decimal=max;
  result = DecimalToBinary(decimal,bits,result);

  result = InvertBinary(result, bits);
  decimal = BinaryToDecimal(result, bits);
  decimal+=1;
  result = DecimalToBinary(decimal,bits,result);
  return result;
}

int toSigned(signed long int* result, signed long int decimal, int bits){
  result = DecimalToBinary(decimal,bits,result);
  signed long int msb = -(result[bits-1]<<(bits-1));
  signed long int positives = 0;
  for (size_t i = bits-2; i!=-1; i--) {
    positives += result[i]<<i;
  }
  //printf("msb: %ld\npositives: %ld\n", msb, positives);
  return positives+msb;
}

int toUnsigned(signed long int* result, signed long int decimal, int bits){
  if(decimal<0) {
    result = TwosComplement(result,decimal,bits);
  }else{
    signed long int max = (1 << (bits-1))-1;
    if(decimal>max) decimal=max;
    result = DecimalToBinary(decimal,bits,result);
  }

  signed long int positives = 0;
  for (size_t i = bits-1; i!=-1; i--) {
    positives += result[i]<<i;
  }
  //printf("positives: %ld\n", positives);
  return positives;
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
  char to[2];
  while(fscanf(f,"%ld %d %s %s\n",&decimal,&bits,to,to)!=EOF){
    signed long int* result = calloc(bits,sizeof(signed long int*));
    int answer=0;
    if(strcmp(to,"s")==0){
      answer = toSigned(result, decimal, bits);
    }
    if(strcmp(to,"u")==0){
      answer = toUnsigned(result,decimal,bits);
    }
    printf("%d\n", answer);
    //read(result,bits);
    free(result);
  }
}
