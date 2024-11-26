#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

//semÃ¡foros
#include <semaphore.h>
#include <fcntl.h>

sem_t* sem;

void *imprime(void* args)
{
    int in = 0, out = 0;
	while(1)
	{
		sem_wait(sem);
		FILE* spool;

		spool = fopen("spool.txt","r+");
	    fscanf(spool, "%d %d\n", &in, &out);

		out++;
		
	    if(out < in && out >= 0)
	    {
			char filename[20];
	
			sprintf(filename, "%d.prt", out);
	
            FILE *fout;
            fout = fopen(filename, "r");
		
			char content[100];
			fscanf(fout, "%s", content);
			printf("%s\n", content);

			fclose(fout);
			remove(filename);
		}else{
			if(in == out)
			    in = 0;
			out = -1;
			
		}

		rewind(spool);
    	fprintf(spool, "%d %d\n", in, out);		

		fclose(spool);
		sem_post(sem);
		
		//if(out == -1) raise(SIGSTOP);
	}
	return NULL;
}

void* menu(void* arg)
{
	while(getchar() != 'q');
	sem_close(sem);
	sem_unlink("/sem_print");
	exit(0);
	return NULL;
}

int main()
{

	
	pid_t pid = getpid();
	
	FILE* spid = fopen("spid.txt", "w");
	fprintf(spid, "%d", pid);
	fclose(spid);
	
	sem = sem_open("/sem_print", O_CREAT | O_EXCL, 0644, 1);
	if(sem == SEM_FAILED){
	 	sem_unlink("/sem_print");
	 	sem = sem_open("/sem_print", O_CREAT | O_EXCL, 0644, 1);
	}

	pthread_t thread, thread2;
	pthread_create(&thread, NULL, imprime, NULL);
	pthread_create(&thread2, NULL, menu, NULL);
	
	pthread_join(thread, NULL);
	pthread_join(thread2, NULL);

	return 0;
}