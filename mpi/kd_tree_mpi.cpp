#include <iostream>
#include <algorithm>
#include "kd_tree.hpp"
// #include <vector>
#include <math.h>
#include <string>
#include <sstream>
// #include <mpi.h>

#define N_DIM 2

//++++++++++++++++++        class kpoint        ++++++++++++++
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

template<typename T>
std::string kpoint<T>::save_kpoints(int axis){
    std::string s ("");
    s += "[";

    for(auto i=0;i<ndim;++i){
        s += std::to_string(points[i]);
        s += ",";
    }
    s += ";";
    s += std::to_string(axis);
    s += "]";

    return s;
}


//++++++++++++      class knode         +++++++++++++++++++++++++

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
            right -> in_order();

        }
    }



//  ++++++++++       Other functions...      +++++++++++++++++++++

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
std::vector<struct kpoint<T>> choose_splitting_point(std::vector<struct kpoint<T>> points, int n, int myaxis){
    //use std::nth_element
    //your array won't be sorted, 
    //but you are only guaranteed instead that your nth element is in position n.

    // print_array(*points,n,myaxis);

    auto beg = points.begin();
    int half = n/2;

    std::nth_element(beg, beg + half, points.end(), 
        [&myaxis](kpoint<T>& a, kpoint<T>& b){return a.get_n_point(myaxis) < b.get_n_point(myaxis);}
    );


    return points; //return the first element of the sort array

}

template<typename T>
struct kdnode<T> * build_serial_kdtree( std::vector<struct kpoint<T>> points, int ndim, int axis ) {

/*
* points is a pointer to the relevant section of the data set;
* N is the number of points to be considered, from points to points+N
* ndim is the number of dimensions of the data points
* axis is the dimension used previsously as the splitting one
*/

    struct kdnode<T>* node = new kdnode<T>; 
    const int N = points.size();
    int myaxis = (axis+1) % ndim;

    if( N == 1 ) { //a leaf with the point *points;

        node->left = new kdnode<T>;
        node->right = new kdnode<T>;
        node->axis = myaxis;
        node->split = points.at(0);

    }else{

        auto mypoint = choose_splitting_point( points, N, myaxis);

        // the splitting point
        int half = N/2;

        //al posto di creare ogni volta, si potrebbe usare sempre lo stesso e prendere da una parte all'altra
        std::vector<struct kpoint<T>> left_points(mypoint.begin(), mypoint.begin() + half);
        std::vector<struct kpoint<T>> right_points(mypoint.begin() + half + 1, mypoint.end());
        //+1 so we can delet the splitting point

        node->axis = myaxis;

        node->split = mypoint.at(half);

        // to opt to save a pointer, instead
        node->left = build_serial_kdtree( left_points, ndim, myaxis );
        

        //if original size points is 2, is 1 for left, 1 for split and 0 for right;

        if( N != 2)
            node -> right = build_serial_kdtree( right_points, ndim, myaxis );
        else
            node -> right = new kdnode<T>;

    }

    return node;
    
}

// https://stackoverflow.com/questions/36021305/mpi-send-struct-with-a-vector-property-in-c
template<typename T>
std::string serialize_node(struct kdnode<T> *kdtree){

    std::string s ("");

    if( kdtree -> axis != -1) {
            
            s = kdtree -> split.save_kpoints( kdtree -> axis);


            s += "(" + serialize_node(kdtree -> left) + ")";
            
            s += "(" + serialize_node(kdtree -> right) + ")";

        }

    return s;
}

template<typename T>
struct kdnode<T> * deserialize_node(std::string data){

    std::string s = data;

    if ( s.size() == 0 )
        return new kdnode<T>; 
    
    if ( s[0] == ')' ) 
        return new kdnode<T>; 

    int j = 0;

    while ( j < s.size() && s[j] != '(' ) 
        j ++;

    T arr[N_DIM];
    std::string temp_str;
    if ( s[1] == '[' ){
        
        temp_str = s.substr(2, j-5);

    }else{
        // T arr1[N_DIM]{std::stoi(s.substr(1, j-5))}; //only the number of the array
        // for(int i =0; i<N_DIM; ++i)
        //     arr[i] = arr1[i];
        temp_str = s.substr(1, j-5);

    }

    int temp_val;
    std::istringstream iss(temp_str);
    for(int i =0; i<N_DIM; ++i) {
        iss >> temp_val;
        arr[i] = temp_val;
        if (iss.peek() == ',')
            iss.ignore();
    }
        
    struct kpoint<T> point(arr);
    struct kdnode<T> * root = new kdnode<T>;
    root -> split = point;

    temp_val = (int)s[j-2] - 48 ;//the numbers starts from 48 in the ASCII code
    root -> axis = temp_val; //where axis is saved

    int left = 0, i = j;

    // find separation between left and right definition
    while ( i < s.size() )  {

        if ( s[i] == '(' ) 
            left ++;
        else if ( s[i] == ')' ) 
            left --;

        if ( left == 0 ) {
            break;
        }
        i ++;
    }

    if ( j < s.size() - 1 ) {
        root->left = deserialize_node<T>(s.substr(j + 1, i - 1 - j));
    }
    if ( i + 1 < s.size() - 1 ) {
        root->right = deserialize_node<T>(s.substr(i + 2, s.size() - i - 2));   
    }
    return root;



}


template<typename T>
struct kdnode<T> * build_parallel_kdtree(std::vector<struct kpoint<T>> points, int ndim, int axis, int np, int level, MPI_Comm comm){

    //if np <=1 --> use serial kd_tree

    //IT ONLY ACCEPT NP POWER OF 2

    


    int size, irank;
    MPI_Status status;
    // MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &irank);

    struct kdnode<T>* node = new kdnode<T>; 
    const int N = points.size();
    int myaxis = (axis+1) % ndim;

    if ( N == 1 ){

        #ifdef DEBUG
            std::cout<<"\n Processor n: "<<irank<<", axis = "<<myaxis;
        #endif

        node->left = new kdnode<T>;
        node->right = new kdnode<T>;
        node->axis = myaxis;
        node->split = points.at(0);

    }else if ( np <= 1){

        #ifdef DEBUG
            std::cout<<"\n\tONE PROCESSOR\n";
        #endif

        node = build_serial_kdtree(points, ndim, myaxis);

    }else if( np != pow( 2,level ) ) {// build the three in serial

            
            auto mypoint = choose_splitting_point( points, N, myaxis);

            // the splitting point
            int half = N/2;

            //al posto di creare ogni volta, si potrebbe usare sempre lo stesso e prendere da una parte all'altra
            std::vector<struct kpoint<T>> left_points(mypoint.begin(), mypoint.begin() + half);
            std::vector<struct kpoint<T>> right_points(mypoint.begin() + half + 1, mypoint.end());
            //+1 so we can delet the splitting point

            node->axis = myaxis;

            node->split = mypoint.at(half);

            // to opt to save a pointer, instead
            level = level + 1;
            node->left = build_parallel_kdtree( left_points, ndim, myaxis, np, level, comm);
            
            if( N != 2)
                node -> right = build_parallel_kdtree( right_points, ndim, myaxis, np, level, comm);
            else
                node -> right = new kdnode<T>;




    } else{ // BUILD REAL PARALLEL KD-NODE
        // if np/2 == 2^(level)
        // then I don't increment the level 
        //when it arrives in the good level
        //here start the parallelization of the tree

        #ifdef DEBUG
                // std::cout<<"The last tree";
            #endif

        auto mypoint = choose_splitting_point( points, N, myaxis);

        // the splitting point
        int half = N/2;

        //al posto di creare ogni volta, si potrebbe usare sempre lo stesso e prendere da una parte all'altra
        std::vector<struct kpoint<T>> left_points(mypoint.begin(), mypoint.begin() + half);
        std::vector<struct kpoint<T>> right_points(mypoint.begin() + half + 1, mypoint.end());

        node->axis = myaxis;
        node->split = mypoint.at(half);


        //split communicator and create a communicator for each son (left and right)
        // int col = irank % np;
        // int key = irank * 10; // ex. from 4 to 40 the rank of processor
        // MPI_Comm column_comm;
        // MPI_Comm_split( comm , col , key , &column_comm );


        if( irank % 2 == 0){
            
            //return same level, so it will goes forever in the previous else
            // node->left = build_parallel_kdtree( left_points, ndim, myaxis, np, level, comm );
            node -> left = build_serial_kdtree(left_points, ndim, myaxis);

            // MPI_Send( const void* buf , int count , MPI_Datatype datatype , int dest , int tag , MPI_Comm comm);

        }else{
            
            //return same level, so it will goes forever in the previous else
            if( N != 2)
                // node -> right = build_parallel_kdtree( right_points, ndim, myaxis, np, level, comm);
                node -> right = build_serial_kdtree(right_points, ndim, myaxis);
            else
                node -> right = new kdnode<T>;

        }
        
        
        

        #ifdef DEBUG

        #endif

            


    }
    
    
    return node;


    


}
