all:
	mpic++ main_mpi.cpp kd_tree_mpi.cpp -o main_mpi.x
	# mpirun -np 4 ./main_mpi.x

ompi:
	mpic++ main_mpi_hybrid.cpp kd_tree_mpi_hybrid.cpp -o main_mpi_hybrid.x -O3
	
run_ompi: 
	mpirun -np 4 ./main_mpi_hybrid.x 10000000

clean:
	rm *.x
	
	
#g++ -Wall -g -c kd_tree.cpp -o kd_tree.o
#g++ -Wall -g -c main.cpp -o main.o
#g++ -o aaa kd_tree.o main.o -LKD_TREE_H



