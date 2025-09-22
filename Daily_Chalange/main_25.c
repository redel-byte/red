#include<stdio.h>

int main(){

  int arr1[4] = {1,2,3,4};
  int arr2[4] = {3,4,5,6};
  int lenght = sizeof(arr1)/sizeof(arr1[1]);
  int arr3[lenght];

  for(int i = 0; i < 2; i++){
    arr3[i] = arr1[i];
    if (i == 1){
      for (int i = 2; i < lenght; i++){
        arr3[i] = arr2[i];
      }
    }
    
  }
  for(int i =0;i<lenght;i++){
    printf("%d\n",arr3[i]);
  }
  return 0;
}
