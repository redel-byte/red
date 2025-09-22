#include<stdio.h>

int main(){

  char arr[]="Hello El habib!";
  int lenght = sizeof(arr)/sizeof(arr[0]);
  char reverse[lenght];
  int j = 0;
  for(int i = lenght -1; i>=0; i--){
    reverse[j] = arr[i];
    j ++;
  }
  for(int i = 0; i < lenght; i++){
    printf("%c",reverse[i]);
  }
}
