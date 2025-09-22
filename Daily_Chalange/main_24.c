#include<stdio.h>

int main(){
  int arr1[3] = {1,7,1};
  int arr2[3] = {5,5,6};
  int lenght = sizeof(arr1)/sizeof(arr1[0]) + sizeof(arr2)/sizeof(arr2[0]);
  int arr3[lenght];

  for(int i = 0; i < 3; i++){
    arr3[i] = arr1[i];
  }
  for(int i = 3; i<6; i++){
    arr3[i] = arr2[i - 3];
  }
  for(int i = 0; i < 6; i++){
    printf("%d\n",arr3[i]);
  }
  return 0;
}

