#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

//semáforos
#include <semaphore.h>
#include <fcntl.h>

sem_t* sem;

void *imprime(void* args)
{
    int in = 0, out = 0;
	while(1)
	{
		// Espera o semáforo ficar verde para continuar. Antes de entrar na 
		// região crítica, coloca vermelho (0)
		sem_wait(sem);

		// ***Início da região crítica***, pois manipula arquivo compartilhado 
		// com a printer

		// Abre arquivo com informações do spooler, lendo variáveis in (índice 
		// do arquivo a ser gravado) e out (índice do arquivo a ser impresso)
		FILE* spool;
		spool = fopen("spool.txt","r+");
	    fscanf(spool, "%d %d\n", &in, &out);

		// Incrementa out para ter índice do arquivo lido (o out sempre aponta 
		// para o índice "anterior", de tal forma que, se ele é negativo, a 
		// impressora sabe que já imprimiu tudo)
		out++;
		
		// Se o out for válido, entra na rotina de impressão
	    if(out < in && out >= 0)
	    {
			// Gera o nome do arquivo a imprimir
			char filename[20];
			sprintf(filename, "%d.prt", out);
	
			// Abre arquivo a imprimir
            FILE *fout;
            fout = fopen(filename, "r");
		
			// Imprime arquivo
			char content[100];
			fscanf(fout, "%s", content);
			printf("%s\n", content);

			// Fecha arquivo e remove
			fclose(fout);
			remove(filename);

		}else{

			if(in == out)
			    in = 0;
			out = -1;
			
		}

		// Atualiza spool com novos in e out (out incrementado para apontar 
		// para o próximo arquivo a ser impresso)
		rewind(spool);
    	fprintf(spool, "%d %d\n", in, out);		
		fclose(spool);

		// *** Fim da região crítica ***
		// Passa semáforo para verde (1) para que outro processo que também
		// utiliza esse sinal possa entrar na sua região crítica
		sem_post(sem);
		
		// Se imprimiu tudo (out = -1), durma
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

	
	// Pega número do processo e grava no arquivo para writer poder acordar o 
	// printer
	pid_t pid = getpid();
	FILE* spid = fopen("spid.txt", "w");
	fprintf(spid, "%d", pid);
	fclose(spid);
	

	// Cria semáforo, inicializando como verde (1)
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