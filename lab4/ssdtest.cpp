#include <cstdio>
#include <unistd.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(){
    const char* nome_do_disco = "/dev/sda2";
    int disco = open(nome_do_disco, O_RDONLY);

    int block_size, block_numbers = 1;
    ioctl(disco, BLKSSZGET, &block_size);

    lseek(disco, block_numbers * block_size, SEEK_SET);

    unsigned char buffer[block_size];
    read(disco, buffer, block_size);

    for(int i = 0; i < block_size; i++){
        printf("%02x", buffer[i]);
    }


}