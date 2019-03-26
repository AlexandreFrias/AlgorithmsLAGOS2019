CFLAGS= -Wall -m64 -g -w
CXX=clang++
HOME=/home/alexandre/Dowloads/computador_dell/cplex
ILOG=/opt/ibm/ILOG/CPLEX_Studio1271
CPPFLAGS= -DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEXLIB=-L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread

all: comp_vndb comp_model 

comp_vndb:  
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o VNDB_mwnpp  VNDB_mwnpp.cpp  $(CPLEXLIB) 

comp_model:  
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o mwnpp mwnpp.cpp  $(CPLEXLIB)  