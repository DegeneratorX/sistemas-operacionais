#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *sem_result,*sem_log,*sem_sal;

int main(){
	sem_result = sem_open("/sem_result",O_CREAT,0644,1);
	sem_log = sem_open("/sem_log",O_CREAT,0644,1);
	sem_sal = sem_open("/sem_sal",O_CREAT,0644,1);
	if (sem_result == SEM_FAILED || sem_log == SEM_FAILED){
		printf("ERRO NA CRIAÇÃO DOS SEMÁFOROS.\n");
		exit(1);
	}
	sem_wait(sem_sal);
	printf("q2 pegou o saleiro.\n");
	sleep(1);
	sem_wait(sem_log);
	sem_wait(sem_result);

	sem_post(sem_sal);
	sem_close(sem_sal);

	printf("Escrevendo no arquivo e log.\n");
	sleep(1);
	printf("Escrevendo no arquivo de resultado.\n");
	//wait 2 sec
	sleep(1);


	sem_post(sem_log);
	sem_post(sem_result);

	sem_close(sem_log);
	sem_close(sem_result);
	return 0;
}