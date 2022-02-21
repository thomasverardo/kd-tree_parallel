#!/bin/sh

#PBS -q dssc_gpu
#PBS -l nodes=1:ppn=24
#PBS -l walltime=1:10:00


cd $PBS_O_WORKDIR
rm treekd_weak.sh.*

module load openmpi-4.1.1+gnu-9.3.0

export OMP_PLACES=cores
export OMP_PROC_BIND=true
export OMP_WAIT_POLICY=active
export OMP_DYNAMIC=false 

make clean
make all
make mp_o3
make ompi

start=10000000

# rm output/mp_weak.out
# for i in 1 2 4 8 16
# do
#     export OMP_NUM_THREADS=${i}
#     a=$(( i * start ))
#     ./main_mp.x ${a} >> output/mp_weak.out
# done



rm output/ompi_weak.out
for i in 1 2 4 8 16
do
    a=$(( i * start ))
    mpirun -np ${i} --map-by socket ./main_mpi.x ${a} >> output/ompi_weak.out
done



# export OMP_NUM_THREADS=1
# ./main_mp.x 100000000 