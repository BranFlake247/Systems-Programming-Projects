#include <stdio.h>

int main(){
    printf("Please enter an integer: ");
    int iInteger = 0;
    scanf("%d", &iInteger);

    if(iInteger >= 0){

        for(int i = 0; i <= iInteger; i++){
            
                if(((i % 3) == 0)&((i % 5) == 0)){
                    printf("FizzBuzz");
                }

                else if((i % 3) == 0){
                    printf("Fizz");
                }

                else if((i % 5) == 0){
                    printf("Buzz");
                }

                else {
                    printf("%d", i);
                }
                printf("\n");
            }
        }

    else{

        for(int i = 0; i >= iInteger; i--){
        
            if(((i % 3) == 0)&((i % 5) == 0)){
                printf("FizzBuzz");
            }

            else if((i % 3) == 0){
                printf("Fizz");
            }

            else if((i % 5) == 0){
                printf("Buzz");
            }

            else {
                printf("%d", i);
            }
            printf("\n");
        }
    }
    return 0;
}