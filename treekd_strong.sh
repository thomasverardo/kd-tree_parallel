#!/bin/sh

#PBS -q dssc_gpu
#PBS -l nodes=1:ppn=24
#PBS -l walltime=1:10:00


cd $PBS_O_WORKDIR
rm treekd_strong.sh.*

module load openmpi-4.1.1+gnu-9.3.0

export OMP_PLACES=cores
export OMP_PROC_BIND=true
export OMP_WAIT_POLICY=active
export OMP_DYNAMIC=false

make clean
make all
make mp_o3
# make mp
make ompi

# rm output/serial.out
# for i in 1 2 4 8 16 24
# do
#     ./main.x 100000000 >> output/serial.out
# done



# rm output/mp.out
# for i in 1 2 4 8 16 24
rm output/mp_1_24.out
for i in {1..24};
do
    export OMP_NUM_THREADS=${i}
    ./main_mp.x 100000000 >> output/mp.out
done



rm output/serialize.out
for i in 1 2 4 8 16
do
    mpirun -np ${i} --map-by socket ./main_mpi.x 10000000 >> output/serialize.out
done


#######################    TIME    #######################

#NON SERVE#
# rm output/mp_1_24.out
# for i in {1..24};
# rm output/mp_time.out
# for i in 1 2 4 8 16 24
# do
#     export OMP_NUM_THREADS=${i}
#     /usr/bin/time -f "exe: %E" ./main_mp.x 100000000 &>> output/mp_time.out
#     # /usr/bin/time ./main_mp.x 100000 &>> output/mp_time.out
    
# done


# rm output/ompi_time.out
# for i in 1 2 4 8 16
# do
#     /usr/bin/time -f "exe: %E" mpirun -np ${i} --map-by socket ./main_mpi.x 100000000 &>> output/ompi_time.out
# done



# export OMP_NUM_THREADS=1
# ./main_mp.x 100000000 