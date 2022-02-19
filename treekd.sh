#!/bin/sh

#PBS -q dssc_gpu
#PBS -l nodes=1:ppn=24
#PBS -l walltime=00:30:00

# cd /u/dssc/tomve/Ass2
cd $PBS_O_WORKDIR
rm treekd.sh.*

module load openmpi-4.1.1+gnu-9.3.0

export orte_base_help_aggregate=0

make all
make mp
make ompi

./main.x
./main_mp.x
make run_ompi np=4