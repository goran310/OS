#include <stdio.h>
#include <signal.h>

#define N 6

int OZNAKA_CEKANJA[N] = {0};
int PRIORITET[N] = {0};
int TEKUCI_PRIORITET=0;

int sig[] = {SIGUSR1, SIGUSR2, SIGILL, SIGHUP, SIGINT};

void zabrani_prekidanje () {
	int i;
	
	for (i = 0; i < 5; i++) 
		sighold(sig[i]);
}

void dozvoli_prekidanje () {
	int i;
	
	for(i = 0; i < 5; i++) 
		sigrelse(sig[i]);
}

void prekidna_rutina (int sig);

void obrada_prekida (int n) {
	int i;
    
	switch(n) {
		case 1:
			printf("-  P  -  -  -  -\n");
			for (i = 0; i < 5; i++) {
				printf("-  %d  -  -  -  -\n", i+1);
				sleep(1);
			}
			printf("-  K  -  -  -  -\n");
			break;
			
		case 2:
			printf("-  -  P  -  -  -\n");
			for (i = 0; i < 5; i++) {
				printf("-  -  %d  -  -  -\n", i+1);
				sleep(1);
			}
			printf("-  -  K  -  -  -\n");
			break;
			
		case 3:
			printf("-  -  -  P  -  -\n");
			for (i = 0; i < 5; i++) {
				printf("-  -  -  %d  -  -\n", i+1);
				sleep(1);
			}
			printf("-  -  -  K  -  -\n");
			break;
			
		case 4:
			printf("-  -  -  -  P  -\n");
			for (i = 0; i < 5; i++) {
				printf("-  -  -  -  %d  -\n",i+1);
				sleep(1);
			}
			printf("-  -  -  -  K  -\n");
			break;
			
		case 5:
			printf("-  -  -  -  -  P\n");
			for (i = 0; i < 5; i++) {
				printf("-  -  -  -  -  %d\n", i+1);
				sleep(1);
			}
			printf("-  -  -  -  -  K\n");
			break;
	}
}

void prekidna_rutina (int sig) {
	int n, i = 0, x = 0;
	zabrani_prekidanje();
	
	//odredi uzrok prekida
	switch(sig) {
		case SIGUSR1:
		n = 1;
		printf("-  X  -  -  -  -\n");
		break;
		
		case SIGUSR2:
		n = 2;
		printf("-  -  X  -  -  -\n");
		break;
		
		case SIGILL:
		n = 3;
		printf("-  -  -  X  -  -\n");
		break;
		
		case SIGHUP:
		n = 4;
		printf("-  -  -  -  X  -\n");
		break;
		
		case SIGINT:
		n = 5;
		printf("-  -  -  -  -  X\n");
		break;
	}
	
	OZNAKA_CEKANJA[n]++;
	
	if (TEKUCI_PRIORITET < n) {
		do {
			x = 0;
			/*odredi prekid najvećeg prioriteta koji čeka na obradu*/
			for(i = TEKUCI_PRIORITET+1; i < N; i++) {
				if (OZNAKA_CEKANJA[i] != 0)
					x = i;
			}
		
			/* ako postoji prekid koji ceka i prioritetniji je od trenutnog posla, idi u obradu */
			if (x > 0) {
				OZNAKA_CEKANJA[x]--;
				PRIORITET[x] = TEKUCI_PRIORITET;
				TEKUCI_PRIORITET = x;
			
				dozvoli_prekidanje();
				obrada_prekida(x);
				zabrani_prekidanje();
			
				//obnova prioriteta prije obrade
				TEKUCI_PRIORITET = PRIORITET[x];
			
			}
		} while (x > 0);
	}
	dozvoli_prekidanje();
}

int main () {
	int i;
	
	//poveži signale sa prekidnom rutinom
	sigset(SIGUSR1, prekidna_rutina);
	sigset(SIGUSR2, prekidna_rutina);
	sigset(SIGILL, prekidna_rutina);
	sigset(SIGHUP, prekidna_rutina);
	sigset(SIGINT, prekidna_rutina);
	
	printf("Proces obrade prekida, PID = %d\n\n", getpid());
	printf("GP S1 S2 S3 S4 S5\n");
	printf("-----------------\n\n");
	
	sleep(10);
	
	for (i = 0; i < 30; i++) {
		printf("%d  -  -  -  -  -\n", i+1);
		sleep(1);
	}
	
	printf("Zavrsio osnovni program\n");
	
	return 0;
}
	
		
		
		
		
			
	
