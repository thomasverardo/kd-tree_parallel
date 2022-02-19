all:
	mpic++ Serial/main.cpp Serial/kd_tree.cpp -o main.x -std=c++11
	
mp: 
	mpic++ -fopenmp Openmp/main_mp.cpp Openmp/kd_tree_mp.cpp -o main_mp.x

mp_debug:
	mpic++ -fopenmp -g Openmp/main_mp.cpp Openmp/kd_tree_mp.cpp -o main_mp.x -DDEBUG

ompi: 
	mpic++ mpi/main_mpi.cpp mpi/kd_tree_mpi.cpp -o main_mpi.x

ompi_debug:
	mpic++ mpi/main_mpi.cpp mpi/kd_tree_mpi.cpp -o main_mpi.x -DDEBUG -g

run_ompi: 
	mpirun -np $(np) ./main_mpi.x 1000000

run_ompi2: 
	mpirun -np 5 ./main_mpi.x 1000000

val_ompi2:
	mpirun -np 4 valgrind -s ./main_mpi.x 1000000

clean:
	rm *.x
	
	
#g++ -Wall -g -c kd_tree.cpp -o kd_tree.o
#g++ -Wall -g -c main.cpp -o main.o
#g++ -o aaa kd_tree.o main.o -LKD_TREE_H



