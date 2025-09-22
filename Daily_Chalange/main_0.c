#include<stdio.h>

int sumofnumbers(int n);

int main(){
  int long num;
  printf("Enter a number: ");
  scanf("%d",&num);

  printf("The result is: %d\n",sumofnumbers(num));
  return 0;
}


int sumofnumbers(int n){
  int long sum = 0;
  while(n!=0){
    sum += n%10;
    n/=10;
  }
  return sum;
}
