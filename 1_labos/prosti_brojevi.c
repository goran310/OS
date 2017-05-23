#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

int pauza = 0;
unsigned long broj = 1000000001;
unsigned long zadnji = 1000000001;

void periodicki_ispis (int sig) {
	printf("zadnji prosti broj: %lu\n", zadnji);
}

void postavi_pauzu (int sig) {
	pauza = 1 - pauza;
}

void prekini (int sig) {
	printf("Kraj programa, zadnji broj: %lu\n", zadnji);
	exit(0);
}

int prost (unsigned long n) {
	unsigned long i;
	unsigned long max;
	
	//nije prost
	if ((n&1) == 0)
		return 0;
	
	max = sqrt(n);
	
	for (i = 3; i <= max; i += 2) {
		if (n%i == 0)
			return 0;
	}
	
	//prost je
	return 1;
}

int main (int argc, char** argv) {
	
	
	//povezivanje signala sa funkcijama
	sigset(SIGINT, postavi_pauzu);
	sigset(SIGTERM, prekini);

	//postavljanje periodičkog alarma
	struct itimerval t;
	sigset (SIGALRM, periodicki_ispis);
	
	//slanje signala svakih 5 sekundi
	t.it_value.tv_sec = 1;
	t.it_value.tv_usec = 0;
	t.it_interval.tv_sec = 5;
	t.it_interval.tv_usec = 0;
	
	//pokretanje timera
	setitimer (ITIMER_REAL, &t, NULL);

	while (1) {
		if (prost(broj) == 1) 
			zadnji = broj;
		broj++;
		while (pauza == 1)
			sleep(1);
	}
	
	return 0;
}


	
