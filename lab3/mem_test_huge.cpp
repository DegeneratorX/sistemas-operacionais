#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <sys/mman.h>
#include <cstring>

#define NUM_ALLOCS 100
#define ALLOC_SIZE (2*1024*1024)

void* allocmem(size_t size) {
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("Erro ao alocar paginas.");
        exit(1);
    }
    return ptr;
}

void fillmem(void* ptr, size_t size) {
    memset(ptr, 0, size);
}

int main(){
    class timeval start, end;
    long seconds, useconds;

    void* allocs[NUM_ALLOCS];

    gettimeofday(&start, nullptr);

    for (int i = 0; i < NUM_ALLOCS; i++) {
        allocs[i] = allocmem(ALLOC_SIZE);
        if (allocs[i] == nullptr) {
            perror("Erro ao alocar. \n");
            exit(1);
        }
        fillmem(allocs[i], ALLOC_SIZE);
    }
    gettimeofday(&end, nullptr);
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    double total = seconds+useconds/1000000.0;
    printf("Tempo de execucao: %f.6f segundos\n", total);

    for (int i = 0; i < NUM_ALLOCS; i++) {
        munmap(allocs[i], ALLOC_SIZE);
    }
}