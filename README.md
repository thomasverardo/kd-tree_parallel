# Second Assignment FHPC course 2021 / 2022

## Build a kd-tree for 2-dimensional data

 Kd-trees are a data structures presented originally by Friedman, Bentley and Finkel in 1977 to represent a set of k -dimensional data in order to make them efficiently searchable.
In spite of its age, or more likely thanks to it and to the large amount of research and improvements accumulated in time, kd- trees are still a good pragmatical choice to perform knn (k-nearest neighbours) operations in many cases.
In this assignment you are required to write a parallel code that builds a kd- tree for k=2. You must implement both the MPI and the OpenMP version.

In order to simplify the task, the following 2 assumptions hold:

* [A1] the data set, and hence the related kd- tree, can be assumed immutable , i.e. you can neglect the insertion/deletion operations;
* [A2] the data points can be assumed to be homogeneously distributed in all the k dimensions.

You find a formal introduction^2 to kd- trees in the uploaded paper “The k-d tree data structure and a proof for neighborhood computation in expected logarithmic time”, which contains also pseudo-code that guides you to.

### Some elements about the kd-trees
A kd- tree is a tree whose basic concept is to compare against 1 dimension at a time per level cycling cleverly through the dimensions so that to keep the tree balanced. The, at each iteration , it bi-sects the data along the chosen dimension , creating a “ left- ” and “ right- ” sub trees for which the 2 conditions and hold respectively, where is a point in the data set and the sub-script indicates the component of the dimension (in you may visualize it better as the and cooordinates).

 In general, the choice of the pivotal point is clearly a critical step in the building of the tree since from that it descends how balanced the resulting tree will be. In fact, a large fraction of theoretical work on trees is exactly about that.
However, here is where the assumption A2 above comes in to simplify this assignment; since you can assume that your data are homogeneously distributed in every dimension, a simple and good choice is to pick the median element along each dimension.
 Then, the basic data structure for a node of your kd- tree should resemble to something like the following
 
```
#if !defined(DOUBLE_PRECISION)
#define float_t float
#else
#define float_t double
#endif
#define NDIM 2
	struct kpoint float_t[NDIM];
	struct kdnode {
	int axis; // the splitting dimension
	kpoint split; // the splitting element
	struct kdnode *left, *right; // the left and right sub-trees
}
```

 NOTE: the floating point size of the coordinates could be either float or double; in the code snippet here above you find the way to decide it at compile-time.

 A node which has no children node (i.e. left and right are NULL pointers) contains only a data point and is called a leaf. The choice of which is the splitting dimension at each iteration is also critical. A quite common basic choice is to round-robin through the dimensions, like

```
struct kdnode * build_kdtree( <current data set>, int ndim, int axis )
{
	// axis is the splitting dimension from the previous call
	// ( may be -1 for the first call)
	int myaxis = (axis+ 1 )%ndim;
	struct kdnode *this_node = (struct kdnode*)malloc(sizeof(struct kdnode));
	this_node.axis = myaxis;

	this_node.left = build_kdtree( <left_points>, ndim, myaxis);
	this_node.right = build_kdtree( <right_points>, ndim, myaxis);
	return this_node;
}
```

This strategy is good enough if the data space is sufficiently “squared”; otherwise, you may end up with nodes extremely elongated along one direction. Moreover, in the previous code snippet the check about the fact that at least 2 points are present in the chosen direction, which is mandatory, is absent.

Another possible and simple strategy is to pick the dimension of maximum extent. In other words, you may check the extent of the data domain along each direction and, if the extents are different by more than a threshold, you choose as current plsitting direction the one with the maximum extent (even if it was used in the previous iteration).

In this seconda case you treat “automatically” the case for strongly degenerated data distribution (in your case imagin a “stripe” distribution along either the or the axis).
All-in-all, a pseudo algorithm to build a kd- tree may resemplbe to something like the following:




```
struct kdnode * build_kdtree( kpoint *points, int N, int ndim, int axis )
	/*
	* points is a pointer to the relevant section of the data set;
	* N is the number of points to be considered, from points to points+N
	* ndim is the number of dimensions of the data points
	* axis is the dimension used previsously as the splitting one
	*/
{
	if( N == 1 ) return a leaf with the point *points;

	struct kdnode *node;
	//
	// ... here you should either allocate the memory for a new node
	// like in the classical linked-list, or implement something different
	// and more efficient in terms of tree-traversal
	//
	int myaxis = choose_splitting_dimension( points, ndim, axis); //
	implement the choice for
	// the
	splitting dimension
	kpoint *mypoint = choose_splitting_point( points, myaxis); // pick-up
	the splitting point

	struct kpoint *left_points, *right_points;
	//
	// ... here you individuate the left- and right- points
	//

	node -> axis = myaxis;
	node -> split = *splitting_point; // here we save a data point, but
	it's up to youù
	// to opt to save a pointer, instead
	node -> left = build_kdtree( left_points, N_left, ndim, myaxis );
	node -> right = build_kdtree( right_points, N_right, ndim, myaxis );
	return node;
}
```


### Parallelization

 You are required to implement both an MPI and an OpenMP versions of the code. Alternatively, you could opt for a single hybrid MPI-OpenMP version.
The parallelization strategy may be pretty similar. One of the simplest strategy, given assumptions A1 and A2, is that you may distribute the data among your processes/threads (let’s call them “tasks” in the following, meaning either MPI processes or OpenMP threads).

 For instance, if you have 2 tasks, you may determine the first splitting and hence distribute the data among the two workers. Each task may then proceed independently. The same strategy holds for a larger number of tasks in an abvisou way. If you choose this approach, you can assume to use a number of tasks that is a power of 2.
 
 
 
## How to run on ORFEO

```
module load openmpi-4.1.1+gnu-9.3.0
```

Serial code:

```
make

./main.x N -->  where N is the number of points
```

OpenMP code:

```
make mp

./main_mp.x N --> where N is the number of points
```

Open MPI code:

```
make ompi

make run_ompi np=NP --> where NP is the number of processes
make run_ompi3 np=NP n=N --> where NP is the number of processes and N the number of points
make run_ompi2 --> with default configuration
```




1. Jerome Harold Friedman, Jon Louis Bentley, and Raphael Ari Finkel. “An algorithm for finding best matches in logarithmic expected time”. In:
ACM Transactions on Mathematical Software (TOMS) 3.3 (1977), pp. 209–226. See the materials added in the sub-folder materials/ in the
assignment folder. ↩
2. the wikipedia page for the kd-trees is also a good starting point for your needs. ↩


