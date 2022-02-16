#ifndef KD_TREE_H
#define KD_TREE_H

// #pragma once

#include <vector>
#include <mpi.h>

#define N_DIM 2


template<typename T>
struct kpoint{
    T points[N_DIM];
    int ndim;

    kpoint();
    kpoint(T points_[]){
        ndim = N_DIM;
        for(int i = 0; i<ndim; ++i){
            points[i] = points_[i];
        }
    }

    T get_n_point(int axis);

    void set_point(int axis, T value);

    void print_kpoints(int axis);

    std::string save_kpoints(int axis);

};

template<typename T>
struct kdnode {
    int axis; // the splitting dimension
    struct kpoint<T> split; // the splitting element
    struct kdnode<T> *left, *right; // the left and right sub-trees

    kdnode();

    void in_order();
    void pre_order();

};



template<typename T>
struct kdnode<T> * build_serial_kdtree( std::vector<struct kpoint<T>> points, int ndim, int axis );

template<typename T>
struct kdnode<T> * build_parallel_kdtree(std::vector<struct kpoint<T>> points, int ndim, int axis, int np, int level, MPI_Comm comm);


#endif

