#include <iostream>
#include <algorithm>
#include "kd_tree_v2.hpp"
#include <vector>

// #define NDIM 3

//class kpoint
template<typename T>
kpoint<T>::kpoint(){
    // points = {};
    ndim = N_DIM;
}

template<typename T>
T kpoint<T>::get_n_point(int axis){
    return points[axis];
}

template<typename T>
void kpoint<T>::set_point(int axis, T value){
    points[axis] = value;
}

template<typename T>
void kpoint<T>::print_kpoints(int axis){
    std::cout<<"( ";
    for(auto i=0;i<ndim;++i){
        std::cout<<points[i]<<", ";
    }
    std::cout<<"axis="<<axis<<") ";

    return;
}


//class knode
template<typename T>
kdnode<T>::kdnode(){
    axis = -1; //indicates that the knode is empty
}

template<typename T>
void kdnode<T>::in_order(){

        if( axis != -1) {
            
            left -> in_order();
            this -> split.print_kpoints(axis);
            right -> in_order();

        }
    }

template<typename T>
void kdnode<T>::pre_order(){

        if( axis != -1) {
            
            
            this -> split.print_kpoints(axis);
            left -> in_order();
            std::cout<<" right: ";
            right -> in_order();

        }
    }



//  ++++++++++       Other functions...      ++++++++++++++++++
template<typename T>
void print_array(kpoint<T> &arr, int n, int axis){
    for(auto i=0; i<n; ++i){
        for(auto j=0; j<axis; ++j){
            std::cout<<arr.get_n_point(j)<<" ";
        }
        std::cout<<std::endl;
    }
}

template<typename T>
std::vector<struct kpoint<T>> choose_splitting_point(std::vector<struct kpoint<T>> points, int start, int end, int myaxis){
    //use std::nth_element
    //your array won't be sorted, 
    //but you are only guaranteed instead that your nth element is in position n.

    // print_array(*points,n,myaxis);

    auto beg = points.begin();

    int half = start + (end-start)/2;

    std::nth_element(beg + start, beg + half, beg + end, 
        [&myaxis](kpoint<T>& a, kpoint<T>& b){return a.get_n_point(myaxis) < b.get_n_point(myaxis);}
    );


    return points; //return the first element of the sort array

}

template<typename T>
struct kdnode<T> * build_kdtree( std::vector<struct kpoint<T>> points, int start, int end, int ndim, int axis ) {

    /*
    * points is a pointer to the relevant section of the data set;
    * N is the number of points to be considered, from points to points+N
    * ndim is the number of dimensions of the data points
    * axis is the dimension used previsously as the splitting one
    */

    struct kdnode<T>* node = new kdnode<T>; 
    const int points_dim = end - start;
    int myaxis = (axis+1) % ndim;

    if( points_dim == 1 ) { //a leaf with the point *points;

        node->left = new kdnode<T>;
        node->right = new kdnode<T>;
        node->axis = myaxis;
        node->split = points.at(start);

    }else{

        //new "sorted" vector
        auto mypoint = choose_splitting_point( points, start, end, myaxis);

        // the splitting point
        int half = start + points_dim/2;


        node->axis = myaxis;

        node->split = mypoint.at(half);

        // to opt to save a pointer, instead
        node->left = build_kdtree( mypoint, start, half, ndim, myaxis );

        //if original size points is 2, is 1 for left, 1 for split and 0 for right;
        if( points_dim != 2)
            node -> right = build_kdtree( mypoint, half + 1, end, ndim, myaxis );
        else
            node -> right = new kdnode<T>;
        
    }

    return node;
    
}









