#include <iostream>
#include "kd_tree.hpp"
#include "kd_tree_mp.cpp"
#include <random>
#include <array>
#include <omp.h>
#include <chrono>

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

    int n = std::stoi(argv[1]);

    int ndim = N_DIM;

    auto points = generatePoints<int>(ndim, n);

    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    struct kdnode<int>* kdtree;

    start = std::chrono::high_resolution_clock::now();
            
    #pragma omp parallel shared(points) private(start, end)
    {
        #pragma omp single
        {

            kdtree = build_kdtree<int>(points, ndim, 1);
            // kdtree = build_kdtree<int>(0, points.size() - 1, ndim, 1);
            
        }
    }

    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;

    // std::cout<<"\nTime to buld the tree: "<<diff.count() <<" s\n";
    std::cout<<diff.count()<<std::endl;;

    // kdtree -> in_order();


    return 0;
}

// export OMP_NUM_THREADS=4
