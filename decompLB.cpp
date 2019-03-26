#include <ilcplex/ilocplex.h>
#include <iostream>
ILOSTLBEGIN

int min(int i, int k){// encontra o menor de dois índices
	if(i<k){return i;}
	return k;
}

int index(int j, int i, int n){// mapeia (j,i)--> l permitindo elininação de um triângulo de variáveis
    i = (int) j*n+i-j*(j+1)/2;
    //i = (int) j*(n-(j+1)/2)+i;
    return i;
}
//início do mergesort
void Merge( double *A,  double *L, int leftCount, double *R, int rightCount) {
	int i,j,k;

	// i - to mark the index of left aubarray (L)
	// j - to mark the index of right sub-raay (R)
	// k - to mark the index of merged subarray (A)
	i = 0; j = 0; k =0;

	while(i<leftCount && j< rightCount) {
		if(L[i]  > R[j]) A[k++] = L[i++];
		else A[k++] = R[j++];
	}
	while(i < leftCount) A[k++] = L[i++];
	while(j < rightCount) A[k++] = R[j++];
}

// Recursive function to sort an array of lllegers. 
void MergeSort( double *A, int n) {
	int mid,i;
	 double *L, *R;
	if(n < 2) return; // base condition. If the array has less than two element, do nothing. 

	mid = n/2;  // find the mid index. 

	// create left and right subarrays
	// mid elements (from index 0 till mid-1) should be part of left sub-array 
	// and (n-mid) elements (from mid to n-1) will be part of right sub-array
	L = ( double*)malloc(mid*sizeof( double)); 
	R = ( double*)malloc((n- mid)*sizeof( double)); 
	
	for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
	for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray

	MergeSort(L,mid);  // sorting the left subarray
	MergeSort(R,n-mid);  // sorting the right subarray
	Merge(A,L,mid,R,n-mid);  // Merging L and R lllo A as sorted list.
    free(L);
    free(R);
}
//fim do mergesort

// algoritmos de máximo e mínimo retornam o índice do menor/maior elemento
int menor( double *L, int k){
	int aux=0;
	for(int i=1; i<k; i++){
		if(L[aux]>L[i]){
			aux=i;
		}
	}
	return aux;
}

int maior( double *L, int k){
	int aux=0;
	for(int i=1; i<k; i++){
		if(L[aux] < L[i]){
			aux=i;
		}
	}
	return aux;
}

typedef struct par{
	int max;
	int min;
}par;

par maiormenor( double *L, int k){
	par ind;
	ind.min=0; // menor
	ind.max=0; // maior
	for(int i=1; i<k; i++){
		if(L[ind.min]>L[i]){
			ind.min=i;
		}
		else{
			if(L[ind.max]<L[i]){
				ind.max=i;
			}
		}
	}
	return ind;
}
// fim das auxiliares
void LPT(double *y, double &obj, int n, int k, double *v) {
	int i; // índice do conjunto
	int j=0; // índice da parte

	double *L;
	L = ( double*)malloc(k*sizeof( double));

	for(i=0; i<k; i++){
		L[i]=v[i];
		y[index(i,i,n)]=1;
	}

	L[k-1]+=v[k];
	y[index(k-1,k,n)]=1;

	for(i=k+1; i<n; i++){
		j=menor(L, k);
		L[j]+=v[i];
		y[index(j,i,n)]=1;
	}

	par a=maiormenor(L, k);
	obj=L[a.max]-L[a.min];
	free(L);

}
//==========================================================
// modelo com menos variáveis
//==========================================================
double VNDB(int NumViz, int IterMax, char inst[]) {
    
    try {
    	IloEnv env;

    	//==================================================
    	// carrega a instância
    	//==================================================

    	int n,k;
		
		ifstream entrada(inst);
		entrada  >> n;
		entrada  >> k;

		double v[n];
		
		for(int i=0;i<n;++i){
			entrada >> v[i];
		}
		
		int N = (int) (n*k - k*(k-1)/2);
		//=======================================================
		// guarda solução inicial para o vndb
		//=======================================================

		int j,i,l;

		double y[N];
		for(i=0; i<N; i++){
			y[index(j,i,n)]=0;
		}
		double obj=0;

		MergeSort( v, n);
		LPT(y, obj, n, k, v);

		//=======================================================
		// define variáveis
		//=======================================================

		IloNumVarArray x(env, N, 0, 1, ILOBOOL);
		IloNumVarArray t(env, 2, 0, IloInfinity, ILOFLOAT);

		//=========================================================
		// cria o modelo
		//=========================================================
		IloModel modelo(env);

		modelo.add(IloMinimize(env, t[1] - t[0]));
		
		for(j=0;j<k;j++){
			IloExpr Expr1(env);
			for(i=j;i<n;i++){
				Expr1 = Expr1 + v[i]*x[index(j,i,n)];
			}
			modelo.add(Expr1 - t[1] <= 0);
			modelo.add(Expr1 - t[0] >= 0);
			Expr1.end();	
		}

		for(i=0;i<n;i++){
			IloExpr Expr2(env);
			for(j=0;j<min(i+1,k);j++){
				Expr2 = Expr2 + x[index(j,i,n)];
			}
			modelo.add(Expr2 == 1);
			Expr2.end();
		}

		for(j=2;j<k;j++){
			for(l=2;l<n-k;l++){
				IloExpr Expr3(env);
				for(i=j-1; i<l; i++){
					Expr3 = Expr3 + x[index(j-1,i,n)];
				}
				Expr3=(l-j+1)*Expr3;
				for(i=j; i<l; i++){
					Expr3 = Expr3 - x[index(j,i,n)];
				}
				modelo.add(Expr3 >= 0);
				Expr3.end();
			}
		}
		//================================================================
		// parametros do solver
		//================================================================

		IloCplex cplex(modelo);
		//std::ofstream log(log2);
		//cplex.setOut(log);
		cplex.setOut(env.getNullStream());
		cplex.setParam(IloCplex::Param::TimeLimit,60);
    	cplex.setParam(IloCplex::Param::MIP::Limits::TreeMemory,4096);

		//================================================================
		// vizinhança vndb
		//================================================================

		FILE *fp2;
		fp2 = fopen("result.txt","aw+");
		fprintf(fp2,"%lf\t", obj);
		bool criterioparada = false;
		int r = 1;
		int iter=1;
		while(criterioparada == false){
			IloExpr branch(env);
			for(j = 0; j < N; j++){
				branch += x[j]*(1 - y[j]) + y[j]*(1 - x[j]);
			}
			IloExtractable mid_branching = modelo.add(branch == 2*r);
			cplex.solve();
			if(cplex.getStatus() == IloAlgorithm::Optimal or cplex.getStatus() == IloAlgorithm::Feasible){
				if(cplex.getObjValue() < obj){
					obj = cplex.getObjValue();
					for (j = 0; j < N; j++){
        				y[j]=cplex.getValue(x[j]);
        			}
        			r++;
        			modelo.add(branch >= 2*r);
        			modelo.remove(mid_branching);
					r=1;
        		}
        		else{
        			modelo.remove(mid_branching);
					r++;
        		}
			}
			else if(cplex.getStatus() == IloAlgorithm::Infeasible){
				criterioparada = true;
			}

			if(r > NumViz or iter == IterMax){
				criterioparada = true;
			}
			branch.end();
        	fprintf(fp2,"%lf\t", obj);
			iter++;	
    	}
    	fprintf(fp2,"\n");
    	fclose(fp2);
		return obj;
        cplex.end(); //método de solução
		modelo.end(); //modelo matemático
		env.end(); // ambiente
		
    } catch (IloException& ex){
        cerr << "Erro Cplex: " << ex << endl;
    } catch (...) {
        cerr << "Erro Cpp:" << endl;
    }
}

int main(){

	char inst[20];
	clock_t t;
	FILE *arquivo1;

	int NumViz=10; 
	int IterMax=60;
	double f;

	for(int i=1; i<21; i++){

		sprintf(inst, "inst1/inst%i.txt", i);
		t = clock();
		f=VNDB(NumViz, IterMax, inst);
		t = clock() - t;
		arquivo1 = fopen("tempoobj_vndb.txt","aw+");
		fprintf(arquivo1,"%lf\t%lf\n", ((double)t)/((CLOCKS_PER_SEC)/1000), f);
		fclose(arquivo1);
	}

	// for(int i=1; i<21; i++){ //1 até 7 ou 0 até 1

	// 	sprintf(inst, "inst2/inst%i.txt", i);
	// 	arquivo1 = fopen("tempoobj_vndb.txt","aw+");

	// 	t = clock();
	// 	f=VNDB(NumViz, IterMax, inst);
	// 	t = clock() - t;

	// 	fprintf(arquivo1,"%lf\t%lf\n", ((double)t)/(CLOCKS_PER_SEC), f);
	// 	fclose(arquivo1);
	// }
	return 0;
}