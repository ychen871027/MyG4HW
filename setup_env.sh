#!/bin/sh
#/opt/geant4/10.4.1/bin/geant4.sh
#/opt/cmake/bin/cmake
#/opt/git/bin/

#G4Ver=10.4.0
G4Ver=10.02.p03
#G4Ver=10.04.p02

echo "you are using Geant4: "$G4Ver
. /opt/geant4/$G4Ver/bin/geant4.sh
. /opt/root/root.v6.13.08/bin/thisroot.sh
export PATH=/opt/cmake/bin:/opt/cuda/9.2/bin:/home/chen/local/bin/:$PATH
export MYG4=/opt/geant4/$G4Ver/lib64

echo "env setting finished"
echo $'bin path:\n'$PATH 
echo $'\nMYG4: '$MYG4

#cmake -DGeant4_DIR=/opt/geant4/10.4.0/lib64 ../examples/extended/medical/DICOM/
#cmake -DGeant4_DIR=$MYG4 ../examples/extended/medical/DICOM/

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
