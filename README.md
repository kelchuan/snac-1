This document describes how to build SNAC to reproduce the models presented in

Tian and Choi (2016), "Effects of axially variable diking rates on
faulting at slow spreading mid-ocean ridges",  
_Earth Planet. Sci. Lett._, in press.

# Table of Contents
1. [Getting the code](#get-the-code)
2. [Building SNAC](#build-SNAC)
3. [Running SNAC](#running-SNAC)
4. [Post-processing and visualizing](#post-processing-and-visualization)

# Get the code
Check out the `waterp` branch from `https://github.com/kelchuan/snac-1`
```BASH
> git clone https://github.com/kelchuan/snac-1
> cd snac-1
> git checkout waterp
```

# Build SNAC
## Install dependencies
 - Recent MPI: e.g., OpenMPI >1.6 or MPICH2 >1.5.
 - libxml2

## Set some environment variables for safety
For example,
```
export MPI_DIR=/opt/local
export MPI_BINDIR=${MPI_DIR}/bin
export MPI_INCDIR=${MPI_DIR}/include/mpich2
export MPI_LIBDIR=${MPI_DIR}/lib
export MPI_RUN=${MPI_BINDIR}/mpirun
export PATH=${MPI_BINDIR}:{$PATH}
export LD_LIBRARY_PATH=${MPI_LIBDIR}:{LD_LIBRARY_PATH}
export CC=mpicc
export CXX=mpicxx

export SNAC_DIR=${HOME}/Src/SNAC
export SNAC_BLDDIR=${SNAC_DIR}/build
export SNAC_BINDIR=${SNAC_BLDDIR}/bin
export SNAC_INCDIR=${SNAC_BLDDIR}/include
export SNAC_LIBDIR=${SNAC_BLDDIR}/lib
export PATH=${SNAC_BINDIR}:{$PATH}
export LD_LIBRARY_PATH=${SNAC_LIBDIR}:{LD_LIBRARY_PATH}
```

## Configure and build
In `snac-1/`
```BASH
> MAKE=gmake CC=mpicc CXX=mpic++ ./configure.sh
> make
```
* If configure fails, delete `snac-1/Makefile.system` before retry.
* Build logs might seem to indicate errors during the builidng but they actually only warnings that can be ignored.

# Running SNAC

Input files are in `snac-1/Snac/examples/input_file_for_3D_M_paper`.

For instance, to run a 3D model with M 0.5 to 0.7, linear and the fast weakening,
```BASH
> cd snac-1/Snac/examples/input_file_for_3D_M_paper/3D
> mpirun -np 128 Snac ./M57Lin_fast.xml
```

# Post-processing and visualization

* To generate VTS and PVTS files for all the outputs, go to the output directory specified in the input file and 
```
> snac2vtk .
```

* The VTS files can be tarred and bzipped with `snac-1/Snac/snac2vtk/vts_tar_cjvf.sh`
This script will create an bzipped tar file, `lowc2.tar.bz2`, which contain all the
VTS and PVTS files in the current directory. Go to the output directory and run
```
> bash snac-1/Snac/snac2vtk/vts_tar_cjvf.sh
```

* [ParaView](http://www.paraview.org/) or [VisIt](https://wci.llnl.gov/simulation/computer-codes/visit) can visualize the VTS and PVTS files.
