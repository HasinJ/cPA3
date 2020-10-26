#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

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

unsigned long int* DecimalToBinary(unsigned long int* result, int decimal,int bits){

  result = calloc(bits,sizeof(int*));
  //printf("decimal: %d\n", decimal);
  unsigned long int quotient=decimal;
  unsigned long int remainder=0;
  result[bits-1]=2;
  for (size_t i = bits-2; i!=-1; i--) {
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
  //printf("left: ");
  //read(result,bits);
  return result;
}

unsigned long int* DecimalToFraction(unsigned long int* result, double number,int bits){

  result = calloc(bits,sizeof(int*));

  for (size_t i = 0; i<bits; i++) {
    double product;
    int integer_part;
    product = number*2.0;
    integer_part = (int) product;
    result[i] = integer_part;
    if(integer_part==1) number=product-1.0;
    else number=product;
    //printf("%ld\n", i);
    if(number==1.0) break;
  }
  //printf("right: ");
  //read(result,bits);
  return result;
}

void movePoint(unsigned long int* mLeft, int leftsize, unsigned long int* mRight, int rightsize,int show){
  int e=0,temp=0,period=-1,j=0,k=0,stop=0;
  unsigned long int* m = calloc(53,sizeof(int*));
  for (size_t i = 0; i < 53; i++) {
    if(j!=leftsize){
      m[i]=mLeft[j];
      j++;
    }else if(k!=rightsize){
      m[i]=mRight[k];
      k++;
    }
  }
  //read(m,53);
  //printf("leftsize%d\n", leftsize);
  //printf("rightsize%d\n", rightsize);

  while(temp!=2){
    temp=m[++period];
  }

  //leftside
  for (size_t i = 0; i < leftsize; i++) {
    temp=m[i];
    if(temp==1){
      stop=i;
      k=1;
      break;
    }
  }

  //if 1 isnt on the leftside
  if(temp!=1){
    j=period;
    for (size_t i = 0; i < rightsize; i++) {
      temp=m[++j];
      if(temp==1){
        stop=j;
        k=-1;
        break;
      }
    }
  }
  //printf("period%d\n", period);
  //printf("stop%d\n", stop);
  //printf("k%d\n", k);

  if (k==-1) {
    for (size_t i = period; i != stop; i++) {
      temp=m[i+1];
      m[i+1]=m[i];
      m[i]=temp;
      e--;
    }
  }else if(k==1){
    for (size_t i = period; i != stop+1; i--) {
      temp=m[i-1];
      m[i-1]=m[i];
      m[i]=temp;
      e++;
    }
  }

  for (size_t i = 0; i!=stop; i++) {
    if(m[i]==0) {
      m[i]=3;
    }
  }

  //printf("answer: ");
  for (size_t i = 0; i<53; i++) {
    if(m[i]==2) {
      printf(".");
      k=0;
      for (j = i+1; k < show; j++) {
        printf("%lu", m[j]);
        k++;
      }
      break;
    }if(m[i]==3) continue;
    printf("%lu", m[i]);
  }
  printf(" %d\n", e);
  free(m);
  return;
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
    //printf("\nnum: %f\n", number);
    int integer_part = (int) number;
    double decimal_part = number - (double) integer_part;

    if(decimal_part<0) {
      //printf("just do left side\n");
      unsigned long int* mLeft;
      int max=0, leftsize=1;
      while(max<number){
        max=(1<<(++leftsize-1))-1;
      }
      mLeft = DecimalToBinary(mLeft,integer_part,leftsize);
      movePoint(mLeft,leftsize,0,0,bits);
      free(mLeft);
      continue;
    }

    unsigned long int* mRight;
    int rightsize = 52;
    //printf("decimal_part:%f\n", decimal_part);
    //printf("rightsize:%d\n", rightsize);
    mRight = DecimalToFraction(mRight,decimal_part,rightsize);

    unsigned long int* mLeft;
    int max=0, leftsize=1;
    while(max<number){
      max=(1<<(++leftsize-1))-1;
    }
    mLeft = DecimalToBinary(mLeft,integer_part,leftsize);

    movePoint(mLeft,leftsize,mRight,rightsize,bits);

    free(mLeft);
    free(mRight);
  }
}
