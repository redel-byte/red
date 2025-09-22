#include<stdio.h>


int main(){

  int arr[] = {1,2,3,4,5};
  int lenght = sizeof(arr)/sizeof(arr[0]);
  int last = arr[lenght - 1];
  printf("lenght :%d\nlast : %d\n",lenght,last);
  for (int i = lenght - 1; i > 0; i-=1){
    arr[i] = arr[i-1];
  }
  arr[0] = last;
  printf("[");
  for (int i =0; i < lenght; i++){
    printf("%d",arr[i]);
  }
  printf("]");
  return 0;
}
