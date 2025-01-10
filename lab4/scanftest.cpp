#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

int main(){
    char c;
    int tty = open("/dev/tty", O_RDONLY);
    while (1){
        read(tty, &c, 1);
        printf("%c", c);
    }
    close(tty);
}