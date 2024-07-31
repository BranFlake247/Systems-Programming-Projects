#include <stdio.h>
#include <time.h>
#include <unistd.h>     
#include <fcntl.h>      


struct login {
    char user_name[100];
    char terminal[100]; 
    time_t time_stamp;
};

#define LOGIN_FILE "login_file.txt"

int main(){
    struct login currentLogin;
    int intLoginFile;
    int intLoginLength = sizeof(currentLogin);

    intLoginFile = creat(LOGIN_FILE, S_IRUSR | S_IWUSR);
    for(int i=0; i<3; i++){
        sprintf(currentLogin.user_name, "user_%d", i);
        sprintf(currentLogin.terminal, "terminal_%d", 10 + i);
        time(&currentLogin.time_stamp);
        printf("Writing a record for %s on %s...\n", currentLogin.user_name, currentLogin.terminal);
        write(intLoginFile, &currentLogin, intLoginLength);
        sleep(1);
    }
    close(intLoginFile);

    intLoginFile = open(LOGIN_FILE, O_RDONLY);
    if(intLoginFile == -1){
        perror(LOGIN_FILE);
        return 1;
    }
    while(read(intLoginFile, &currentLogin, intLoginLength)==intLoginLength){
        printf("%s last logged in on %s on %.24s\n", currentLogin.user_name, currentLogin.terminal, 
            asctime(localtime(&currentLogin.time_stamp)));
    }
    close(intLoginFile);

    return 0;
}