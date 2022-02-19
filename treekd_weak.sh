#!/bin/sh

#PBS -q dssc_gpu
#PBS -l nodes=1:ppn=24
#PBS -l walltime=1:00:00


# cd /u/dssc/tomve/Ass2
cd $PBS_O_WORKDIR
rm treekd_weak.sh.*

module load openmpi-4.1.1+gnu-9.3.0

# export 		OMP_PLACES=cores
# export 		OMP_PROC_BIND=close
# export		MV2_ENABLE_AFFINITY=0

make all
make mp
make ompi


# rm output/serial.out
# for i in 1 2 4 8 16 24
# do
#     ./main.x 100000000 >> output/serial.out
# done

# rm output/mp.out
# for i in 1 2 4 8 16 24
# do
#     export OMP_NUM_THREADS=${i}
#     ./main_mp.x 100000000 >> output/mp.out
# done



rm output/ompi.out
for i in 1 2 4 8 16 24
do
    mpirun -np ${i} --map-by socket ./main_mpi.x 100000000 >> output/ompi.out
done

