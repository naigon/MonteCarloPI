//NAIGON MEDEIROS MARTINS
//CALCULO DE PI PELO METODO DE MONTE CARLO USANDO THREADS (OpenMP)
//OBS: NA COMPILACAO FOI UTILIZADDO O SINALIZADOR DO GNU "-fopenmp"

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N_THREADS 4 //numero de threads a serem usadas na divisao da tarefa
#define N_PONTOS 1000000 //numero de pontos aletorios que serao usados na aproximacao
#define PI 3.141592653589793 //valor de pi até a 15a casa depois da virgula, apenas para fins de comparacao com o valor obtido no fim

double gera_coord(); //funcao que gera numeros aleatorios
void calcula_pi(); //funcao para o calculo do valor de pi

int main(void){
printf("################### MONTE CARLO PI ###################\n\n");

srand(time(NULL)); //inicializa gerador de numeros com a semente
clock_t start_time; //variavel para calculo do tempo de execução
start_time = clock(); //inicia variavel de contagem de tempo

calcula_pi(); //chamada da função para o calculo de PI

double tempo = (clock() - start_time) / (double)CLOCKS_PER_SEC; //obtem o tempo de execucao depois do retorno de calcula_pi()
printf("[TEMPO TOTAL DE EXECUCAO] = %.2f segundos \n\n",tempo); // imprime tempo na tela em segundos

return 0;
}

/*	- Corpo da funcao geradora de numeros aleatorios,
ela gera um double aletorio, e retorna soh a parte
apos a virgula, pois o objetivo eh obter um valor real 
entre 0 e 1 nas coordenadas do ponto */

double gera_coord(){
	double aux=100.0*((double)(rand())/RAND_MAX);
	aux=aux - (int)(aux);
	return aux;
}

//corpo da função que calcula o valor de PI para N_PONTOS aleaorios desejados
void calcula_pi(){
		
	int i=0;
	double x=0,y=0;
	int pdentro=0,pfora=0;
	double valor_pi=0,erro=0;
	
	/*	- Instrucoes do OpenMP que paralelizam o laco e 
	dividem as iteracoes entre as n threads com as devidas
	operacoes de reducao nas variaveis de interesse pdentro e pfora...
	- OBS: escalonamento dynamic se mostrou melhor que static(em tempo) */
	
	#pragma omp parallel for num_threads(N_THREADS) schedule(dynamic) private(i) reduction(+:pdentro) reduction(+:pfora) 
	for(i=0;i<N_PONTOS;i++){
		x=gera_coord(); //gera coordenada x do ponto
		y=gera_coord(); //gera coordenada y do ponto
		//printf("P(%d) x=%.15f  y=%.15f \n",i,x,y); //print auxiliar para verificar geração das coordenadas
		if((x*x + y*y) <= 1) //soma os quadrados das coordenadas
			pdentro++; //se a soma dos quadrados for menor ou igual a 1, ponto caiu dentro
		else
			pfora++;//se a soma dos quadrados for maior que 1, ponto caiu fora				
	}		

valor_pi = 4.0*(((double)pdentro)/((double)N_PONTOS)); //calcula valor aproximado de PI
erro = valor_pi - PI; //calcula o erro(diferenca em relacao ao valor ideal de PI)

//imprime informações na tela
printf("\n");
printf("[NUMERO TOTAL DE PONTOS] = %d \n\n",N_PONTOS);
printf("[PONTOS DENTRO DO CIRCULO] = %d \n\n",pdentro);
printf("[PONTOS FORA DO CIRCULO] = %d \n\n",pfora);
printf("[VALOR APROXIMADO DE PI] = %.15f \n\n",valor_pi);
printf("[VALOR IDEAL DE PI] = %.15f \n\n",PI);
printf("[DIFERENÇA EM RELACAO AO PI IDEAL] = %.15f \n\n",erro);

}
