all: lsh cube #cluster

# cluster: mainCLUSTER.o inputCLUSTER.o item.o exhaustive.o method.o utils.o hash.o clustering.o
# 	g++ -o cluster mainCLUSTER.o inputCLUSTER.o item.o exhaustive.o method.o utils.o hash.o clustering.o -O3

lsh: mainLSH.o inputLSH.o item.o exhaustive.o method.o utils.o hash.o LSH.o
	g++ -o lsh  mainLSH.o inputLSH.o item.o exhaustive.o method.o utils.o hash.o LSH.o -O3 -g

cube: mainCUBE.o inputCUBE.o item.o exhaustive.o method.o utils.o hash.o CUBE.o
	g++ -o cube mainCUBE.o inputCUBE.o item.o exhaustive.o method.o utils.o hash.o CUBE.o -O3

# mainCLUSTER.o: main.cpp utils.h item.h input.h
# 	g++ -c main.cpp -o mainCLUSTER.o -D CLUSTER_FLAG 

mainCUBE.o: main.cpp utils.h item.h input.h
	g++ -c main.cpp -o mainCUBE.o -D HYPERCUBE_FLAG

mainLSH.o: main.cpp utils.h item.h input.h
	g++ -c main.cpp -o mainLSH.o -D LSH_FLAG

# inputCLUSTER.o: input.cpp input.h utils.h exhaustive.h method.h clustering.h
# 	g++ -c input.cpp -o inputCLUSTER.o -D CLUSTER_FLAG 

inputLSH.o: input.cpp input.h item.h utils.h exhaustive.h method.h LSH.h 
	g++ -c  input.cpp -o inputLSH.o -D LSH_FLAG

inputCUBE.o: input.cpp input.h item.h utils.h exhaustive.h method.h CUBE.h
	g++ -c  input.cpp -o inputCUBE.o  -D HYPERCUBE_FLAG

# clustering.o: clustering.cpp clustering.h utils.h method.h hash.h item.h 
# 	g++ -c clustering.cpp -O3

CUBE.o : CUBE.cpp CUBE.h hash.h utils.h
	g++ -c CUBE.cpp  -O3

LSH.o: LSH.cpp LSH.h hash.h utils.h
	g++ -c LSH.cpp  -O3

hash.o: hash.cpp hash.h utils.h
	g++ -c hash.cpp -O3

exhaustive.o: exhaustive.cpp exhaustive.h method.h
	g++ -c exhaustive.cpp -O3

method.o: method.cpp method.h utils.h
	g++ -c method.cpp -O3

utils.o: utils.cpp utils.h
	g++ -c utils.cpp -O3

item.o: item.cpp item.h 
	g++ -c item.cpp

clear:
	rm -r *.o lsh cube

