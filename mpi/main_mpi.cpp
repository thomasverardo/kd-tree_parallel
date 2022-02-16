#include <iostream>
#include "kd_tree.hpp"
#include "kd_tree_mpi.cpp"
#include <random>
#include <array>
#include <chrono>
#include <mpi.h>

#define MAX 10000000

template<typename T>
std::vector<struct kpoint<T>> generatePoints(const int ndim, const int npoints){

    srand((unsigned) time(0));

    std::vector<struct kpoint<T>> points;
    struct kpoint<T> temp;


    for(auto i=0; i<npoints; ++i){
        for(auto j=0; j<ndim; j++){
            temp.set_point(j, (T)rand() / MAX);
        }
        points.push_back(temp);
    }

    return points;
}

int main(int argc, char **argv){

    int irank, size;
    MPI_Comm comm; 
    
    MPI_Init( &argc, &argv );
    
    
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &irank );

            


    int ndim = N_DIM, n=8;
    int start_axis = 1;

    auto points = generatePoints<int>(ndim, n);

    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    double mpi_start, mpi_end;
    struct kdnode<int>* kdtree;

    start = std::chrono::high_resolution_clock::now();
    mpi_start = MPI_Wtime();

    

    kdtree = build_parallel_kdtree<int>(points, ndim, start_axis, size, 0, MPI_COMM_WORLD);

    end = std::chrono::high_resolution_clock::now();
    mpi_end = MPI_Wtime();

    

    std::chrono::duration<double> diff = end - start;
    double total_mpi = mpi_end - mpi_start;

    // std::cout<<"\nTime to buld the tree: "<<diff.count() <<" s";
    // std::cout<<"\nMPI time: "<<total_mpi<<std::endl;

    kdtree -> pre_order();

    std::string kdtree_ser = serialize_node(kdtree);

    std::cout<<std::endl<<std::endl<<kdtree_ser<<std::endl<<std::endl;

    auto aaaa = deserialize_node<int>(kdtree_ser);

    aaaa -> pre_order();

    
    MPI_Finalize();
    

    return 0;
}
