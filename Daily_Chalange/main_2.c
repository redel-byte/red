#include<stdio.h>
int main(){
  int numbers[] = {1,2,3,4,5};
  int lenght = sizeof(numbers)/sizeof(numbers[0]);
  int temp[lenght];

  int j = 0 ; 
  for(int i = lenght-1; i >= 0; i--){
    temp[j] = numbers[i];
    j++;
  }
  printf("[");
  for(int i = 0; i < lenght ; i++){
    printf("%d",temp[i]);
    if (i<lenght-1){
      printf(",");
    }
  }
  printf("]");
  return 0;
}
