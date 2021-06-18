#include <stdio.h>
#include <string.h>

int main(void){
  char buff[15];
  int pass = 0;

  printf("\n Enter the password : \n");
  gets(buff);

  if(strcmp(buff, "mysecretpassword")){
    printf("\n Wrong password\n");
  }else{
    printf("\n Correct Passowrd\n");
    pass = 1;
  }

  if(pass){
    printf("Root privilages given to the user");
  }
  
  return 0;
}
