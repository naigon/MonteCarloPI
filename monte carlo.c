//NAIGON MEDEIROS MARTINS
//CALCULO DE PI PELO METODO DE MONTE CARLO USANDO THREADS(OpenMP)

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N_PONTOS 50000000 //numero de pontos utilizados no calculo da aproximação
#define PI 3.141592653589793 //valor de pi até a 15a casa depois da virgula, apenas para fins de comparação com o valor obtido

double gera_coord(); //função que gera numeros aleatorios
void calcula_pi(); //função para o calculo do valor de pi

int main(void){
printf("################### MONTE CARLO PI ###################\n\n");

srand(time(NULL)); //inicia gerador de numeros com a semente
clock_t start_time; //variavel para calculo do tempo de execução
start_time = clock();

calcula_pi(); //chamada da função para o calculo de PI

double tempo = (clock() - start_time) / (double)CLOCKS_PER_SEC; //obtem o tempo de execução depois do retorno de calcula_pi()
printf("[TEMPO TOTAL DE EXECUCAO] = %.2f segundos \n\n",tempo); // imprime tempo na tela

return 0;
}

/*	- corpo da função geradora de numeros aleatorios
função geral um double aletório, e retorna só a parte
após a virgula, pois o objetivo é obter um valor real 
entre 0 e 1 nas coordenadas do ponto */

double gera_coord(){
	double aux=100.0*((double)(rand())/RAND_MAX);
	aux=aux - (int)(aux);
	return aux;
}

//corpo da função que calcula o valor de PI
void calcula_pi(){

	int i=0;
	double x=0,y=0;
	int pdentro=0,pfora=0;
	double valor_pi=0,erro=0;
	
	/*	- instruções do OpenMP que paralelizam o laço, 
	com numero de threads desejado e redução das variaveis 
	locais pdentro e pfora...
		- escalonamento dynamic se mostrou melhor que static */
	
	#pragma omp parallel for num_threads(4) schedule(dynamic) private(i) reduction(+:pdentro) reduction(+:pfora) 
	for(i=0;i<N_PONTOS;i++){
		x=gera_coord(); //gera coordenada x do ponto
		y=gera_coord(); //gera coordenada y do ponto
		//printf("P(%d) x=%.15f  y=%.15f \n",i,x,y); //print auxiliar para verificar geração das coordenadas
		if((x*x + y*y) <= 1) //soma os quadrados das coordenadas
			pdentro++; //se a soma dos quadrados for menor ou igual a 1, ponto caiu dentro
		else
			pfora++;//se a soma dos quadrados for maior que 1, ponto caiu fora				
	}		

valor_pi = 4.0*(((double)pdentro)/((double)N_PONTOS)); //calcula aproximado de PI
erro = valor_pi - PI; //calcula o erro(diferença em relação ao valor ideal de PI)

//imprime informações na tela
printf("\n");
printf("[NUMERO TOTAL DE PONTOS] = %d \n\n",N_PONTOS);
printf("[PONTOS DENTRO DO CIRCULO] = %d \n\n",pdentro);
printf("[PONTOS FORA DO CIRCULO] = %d \n\n",pfora);
printf("[VALOR APROXIMADO DE PI] = %.15f \n\n",valor_pi);
printf("[VALOR IDEAL DE PI] = %.15f \n\n",PI);
printf("[DIFERENÇA EM RELACAO AO PI IDEAL] = %.15f \n\n",erro);

}
