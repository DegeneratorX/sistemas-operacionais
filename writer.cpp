#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//semáforos
#include <semaphore.h>
#include <fcntl.h>

int main()
{

	int in = 0, out = 0;

    // Abre semáforo criado pelo printer
	sem_t* sem = sem_open("/sem_print", O_RDWR);
	if(sem == SEM_FAILED) exit(1);

	while(1)
	{
		// Espera usuário digitar texto
		char texto[100];
		scanf("%s", texto);
	
		// Espera o semáforo ficar verde para continuar. Antes de entrar na 
		// região crítica, coloca vermelho (0)
    	sem_wait(sem);
	

    	FILE* spool;


        // ***Início da região crítica***, pois manipula arquivo compartilhado 
		// com a printer

        // Abre arquivo com informações do spooler, lendo variáveis in (índice 
		// do arquivo a ser gravado) e out (índice do arquivo a ser impresso)
		spool = fopen("spool.txt","r+");		
		fscanf(spool, "%d %d\n", &in, &out);
		

        // Cria arquivo de impressão com a string digitada pelo usuário.
        // O nome do arquivo é igual ao índice de entrada do spooler
		char filename[20];
		sprintf(filename, "%d.prt", in);
		
		
		FILE* fout = fopen(filename, "w");
		
		fprintf(fout, "%s", texto);
		
		fclose(fout);

		// Atualiza spooler com o índice da próxima posição livre
		rewind(spool);
    	fprintf(spool, "%d %d\n", (in+1), out);
		fclose(spool);
		sem_post(sem);
	
	    // Acorda processo printer lendo o seu número no arquivo
		pid_t pid;
		FILE* spid = fopen("spid.txt", "r");
		fscanf(spid, "%d", &pid);
		fclose(spid);

        // *** Fim da região crítica ***
        // passa semáforo para verde (1) para que outro processo que também 
		// utiliza esse sinal possa entrar na sua região crítica
		//kill(pid, SIGCONT);
	}		

	return 0;
}