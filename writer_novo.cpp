#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//semÃ¡foros
#include <semaphore.h>
#include <fcntl.h>

int main()
{

	int in = 0, out = 0;

	sem_t* sem = sem_open("/sem_print", O_RDWR);
	if(sem == SEM_FAILED) exit(1);

	while(1)
	{
		char texto[100];
		scanf("%s", texto);
	

    	sem_wait(sem);
	
    	FILE* spool;
		spool = fopen("spool.txt","r+");		
		fscanf(spool, "%d %d\n", &in, &out);
		
		char filename[20];
		sprintf(filename, "%d.prt", in);
		
		
		FILE* fout = fopen(filename, "w");
		
		fprintf(fout, "%s", texto);
		
		fclose(fout);
	
		rewind(spool);
    	fprintf(spool, "%d %d\n", (in+1), out);
		
		fclose(spool);
		sem_post(sem);
	
		pid_t pid;
		FILE* spid = fopen("spid.txt", "r");
		fscanf(spid, "%d", &pid);
		fclose(spid);

		//kill(pid, SIGCONT);
	}		

	return 0;
}