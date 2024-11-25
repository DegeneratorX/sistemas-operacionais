#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <semaphore.h>
#include <fcntl.h>

int main()
{

    int in = 0, out = 0;
    FILE* spool;

    // Abre semáforo criado pelo printer
    sem_t *sem = sem_open("/sem_printer", O_RDWR);

    // laço infinito para pegar texto e mandar para impressão
    while(1)
    {
        // Espera usuário digitar texto
        char texto[100];
        scanf("%s", texto);

        // Espera o semáforo ficar verde para continuar. Antes de entrar na região crítica, coloca vermelho (0)
        sem_wait(sem);

        // ***Início da região crítica***, pois manipula arquivo compartilhado com a printer

        // abre arquivo com informações do spooler, lendo variáveis in (índice do arquivo a ser gravado)
        // e out (índice do arquivo a ser impresso)
        while(!(spool = fopen("spool.txt","r+")));
        fscanf(spool, "%d %d\n", &in, &out);

        // cria arquivo de impressão com a string digitada pelo usuário.
        // O nome do arquivo é igual ao índice de entrada do spooler
        char filename[20];
        sprintf(filename, "%d.txt", in);

        FILE* fout = fopen(filename, "w");

        fprintf(fout, "%s", texto);

        fclose(fout);

        // atualiza spooler com o índice da próxima posição livre
        rewind(spool);
        fprintf(spool, "%d %d\n", (in+1), out);
        fclose(spool);

        // acorda processo printer lendo o seu número no arquivo
        int pid;
        FILE* pidf = fopen("pidprinter.txt", "r");
        fscanf(pidf, "%d", &pid);
        kill(pid, SIGCONT);
        fclose(pidf);

        // *** Fim da região crítica ***
        // passa semáforo para verde (1) para que outro processo que também utiliza esse sinal
        // possa entrar na sua regiÃ£o crítica
        sem_post(sem);
    }

    return 0;
}