#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void cria_instancia(char nomearq[], int n, int k, int p){

  FILE *arquivo;
  int j, l, i, x;

  arquivo = fopen(nomearq,"w");
  if (!arquivo) {
    printf("O Arquivo %s nao pode ser aberto.\n", nomearq);
    getchar();
    exit(1);
  }

  fprintf(arquivo, "%d\n%d\n", n, k);

  for(l=0; l<n; l++){
    for(j=0; j<p; j++){
      x=rand()%10;
      fprintf(arquivo, "%d", x);
    }
    fprintf(arquivo, "\n");
  }

  fclose(arquivo);
}

int main(int argc, char *argv[]){

 	//srand(time(NULL));

 	int N=5; //sample of instance with the same kind
	int i=1;
	char nome_arquivo[25]; // file name
	int n[]={100, 200, 400, 800}; //sequence size
	//int k[]={3,4,5,6}; // number of parts
  int k=atof(argv[1]); // For obtain the same sequence for each element, I replaced k=3,4,5,6 
	int p=12; //maximum number of digits of each element


	for(int i0=0; i0<4; i0++){
     	for(int i1=0; i1<N; i1++){
        	sprintf(nome_arquivo, "inst%i/inst%i.txt", k-2, i);
          //sprintf(nome_arquivo, "inst%i.txt", i);
        	cria_instancia(nome_arquivo, n[i0], k, p);
        	i++;
    	}
  	}

	return 0;
}