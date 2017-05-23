#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int pid = 0;
int sig[] = {SIGUSR1, SIGUSR2, SIGILL, SIGHUP, SIGINT};

void prekidna_rutina(int sig) {
	kill(pid, SIGKILL);
	exit(0);
}

int main (int argc, char **argv) {
	int j;
	
	//dohvati pid drugog procesa
	pid = atoi(argv[1]);
	
	sigset(SIGINT, prekidna_rutina);
	srand(time(NULL));
	
	while(1) {
		j = (rand() % 4);
		sleep(rand()%3 + 3);
		kill(pid, sig[j]);
		printf("signal koji se šalje: %d\n", j+1);
	}
	
	return 0;
	
}
		
