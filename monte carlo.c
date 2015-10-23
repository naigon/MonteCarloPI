//CALCULO DE PI PELO METODO DE MONTE CARLO

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N_PONTOS 100
#define PI 3.141592653589793

double gera_coord();
void calcula_pi();

void main(){
printf("################### N.D.E. - NELSON DEVELOPMENT ENTERPRISE ###################\n\n");

srand(time(NULL));
clock_t start_time;
start_time = clock();

calcula_pi();

double tempo = (clock() - start_time) / (double)CLOCKS_PER_SEC;
printf("[TEMPO TOTAL DE EXECUCAO] = %.2f segundos \n\n",tempo);

}

double gera_coord(){
	double aux=100.0*((double)(rand())/RAND_MAX);
	aux=aux - (int)(aux);
	return aux;
}

void calcula_pi(){

	int i;
	double x,y;
	int pdentro=0,pfora=0;
	double valor_pi,erro;
	
	#pragma omp parallel for num_threads(4) reduction(+:pdentro) reduction(+:pfora) 
	for(i=0;i<N_PONTOS;i++){
		x=gera_coord();
		y=gera_coord();
		//printf("P(%d) x=%.15f  y=%.15f \n",i,x,y);
		if((x*x + y*y) <= 1)
			pdentro++;
		else
			pfora++;				
	}
		
valor_pi=4.0*(((double)pdentro)/((double)N_PONTOS));
erro= valor_pi - PI;

printf("\n");
printf("[NUMERO TOTAL DE PONTOS] = %d \n\n",N_PONTOS);
printf("[PONTOS DENTRO DO CIRCULO] = %d \n\n",pdentro);
printf("[PONTOS FORA DO CIRCULO] = %d \n\n",pfora);
printf("[VALOR APROXIMADO DE PI] = %.15f \n\n",valor_pi);
printf("[VALOR IDEAL DE PI] = %.15f \n\n",PI);
printf("[DIFERENÇA EM RELACAO AO PI IDEAL] = %.15f \n\n",erro);

}

