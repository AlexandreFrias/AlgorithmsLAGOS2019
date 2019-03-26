#include <ilcplex/ilocplex.h>
#include <iostream>
ILOSTLBEGIN

int min(int i, int k){
	if(i<k){return i;}
	return k;
}

int index(int j, int i, int n){
    i = (int) j*n+i-j*(j+1)/2;
    return i;
}

FILE *fp2;

//==========================================================
// modelo com menos variáveis
//==========================================================
double mwnpp1(char inst[]) {
    
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
		int j,i,l;
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
			for(l=3;l<n;l++){
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

		cplex.setParam(IloCplex::Param::TimeLimit,3600);
    	cplex.setParam(IloCplex::Param::MIP::Limits::TreeMemory,30720);
    	cplex.setOut(env.getNullStream());
        
        //-1, 0, 1, 2, 3
        //cplex.setParam(IloCplex::LiftProjCuts, -1);
        //cplex.setParam(IloCplex::Cliques, -1);
		//cplex.setParam(IloCplex::Covers, -1);
		//cplex.setParam(IloCplex::FlowCovers, -1);
		//cplex.setParam(IloCplex::GUBCovers, -1);
		//cplex.setParam(IloCplex::FracCuts, -1);
		//cplex.setParam(IloCplex::MIRCuts, -1);
        //cplex.setParam(IloCplex::FlowPaths, -1);
		//cplex.setParam(IloCplex::ImplBd, -1);
		//cplex.setParam(IloCplex::DisjCuts, -1);

		//cplex.setParam(IloCplex::Param::MIP::Interval, 1); // 1 para o log mais completo possível
        //cplex.setParam(IloCplex::Param::MIP::Display, 5); // informações do nó raiz
		//cplex.setOut(env.getNullStream()); // omite o log

        //cplex.setParam(IloCplex::Param::MIP::Strategy::NodeSelect, 0); //0, 1, 2, 3
        //cplex.setParam(IloCplex::Param::MIP::Strategy::Branch, 0); //-1, 0, 1
        //cplex.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 0); //	-1, 0, 1, 2, 3, 4 
        
        cplex.solve(); 
        
		env.out() << "Status = " << cplex.getStatus() << endl;
        env.out() << "Função Objetivo = " << cplex.getObjValue() << endl;
        env.out() << "Tempo   = " << env.getTime() << endl;
        return cplex.getObjValue();
        cplex.end(); //método de solução
		modelo.end(); //modelo matemático
		env.end(); // ambiente
    } catch (IloException& ex) {
        cerr << "Erro Cplex: " << ex << endl;
    } catch (...) {
        cerr << "Erro Cpp:" << endl;
    }
    
}

int main(){

	char inst[15];
	char log1[25];
	clock_t t;
	double f;
	FILE *arquivo1;
	

	for(int i=1; i<21; i++){
		
		sprintf(inst, "inst1/inst%i.txt", i);
		arquivo1 = fopen("tempoobj_mip.txt","aw+");
		t = clock();
		f=mwnpp1(inst);
		t = clock() - t;

		fprintf(arquivo1,"%lf\t%lf\n", ((double)t)/(CLOCKS_PER_SEC), f);
		fclose(arquivo1);

	}

	for(int i=1; i<21; i++){
		
		sprintf(inst, "inst2/inst%i.txt", i);
		arquivo1 = fopen("tempoobj_mip.txt","aw+");
		t = clock();
		f=mwnpp1(inst);
		t = clock() - t;

		fprintf(arquivo1,"%lf\t%lf\n", ((double)t)/(CLOCKS_PER_SEC), f);
		fclose(arquivo1);

	}

	for(int i=1; i<21; i++){
		
		sprintf(inst, "inst3/inst%i.txt", i);
		arquivo1 = fopen("tempoobj_mip.txt","aw+");
		t = clock();
		f=mwnpp1(inst);
		t = clock() - t;

		fprintf(arquivo1,"%lf\t%lf\n", ((double)t)/(CLOCKS_PER_SEC), f);
		fclose(arquivo1);

	}

	for(int i=1; i<21; i++){
		
		sprintf(inst, "inst4/inst%i.txt", i);
		arquivo1 = fopen("tempoobj_mip.txt","aw+");
		t = clock();
		f=mwnpp1(inst);
		t = clock() - t;

		fprintf(arquivo1,"%lf\t%lf\n", ((double)t)/(CLOCKS_PER_SEC), f);
		fclose(arquivo1);

	}



	return 0;
}

