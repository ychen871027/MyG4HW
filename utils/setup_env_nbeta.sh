#!/bin/sh
#/opt/geant4/10.4.1/bin/geant4.sh
#/opt/cmake/bin/cmake
#/opt/git/bin/

#G4Ver=10.4.0
G4Ver=10.04.p02
#G4Ver=10.04.p02

echo "you are using Geant4: "$G4Ver
. /home/chen/HomeWork_Tidy/geant4.10.04.p02-beta/bin/geant4.sh
#. /opt/geant4/$G4Ver/bin/geant4.sh
#. /opt/root/root.v6.13.08/bin/thisroot.sh
. /opt/root6/6.14.02/bin/thisroot.sh
#export PATH=/opt/cmake/bin:/opt/cuda/9.2/bin:/home/chen/local/bin/:$PATH
export PATH=/opt/cmake/bin:/opt/cuda/9.2/bin:/home/chen/local/bin:/usr/lib64/qt4/bin:$PATH
#export MYG4=/home/chen/HomeWork_Tidy/geant4.10.02.p03-beta/lib64
export MYG4=/home/chen/HomeWork_Tidy/geant4.10.04.p02-beta/lib64
#export MYG4=/opt/geant4/$G4Ver/lib64

echo "env setting finished"
echo $'bin path:\n'$PATH
echo $'\nMYG4: '$MYG4

#cmake -DGeant4_DIR=/opt/geant4/10.4.0/lib64 ../examples/extended/medical/DICOM/
#cmake -DGeant4_DIR=$MYG4 ../examples/extended/medical/DICOM/

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
alias MkMyG4='cmake -DGeant4_DIR=/home/chen/HomeWork_Tidy/geant4.10.04.p02-beta/lib64 ../MyG4HW'
# compile beta
alias GoG4Beta='cd /home/chen/Downloads/geant4_install/geant4.10.04.p02-build/'
#alias GoG4Beta='cd /home/chen/Downloads/geant4_install/geant4.10.02.p03-build/'
alias MkG4Beta='cmake -DCMAKE_INSTALL_PREFIX=/home/chen/HomeWork_Tidy/geant4.10.04.p02-beta -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=OFF -DGEANT4_USE_OPENGL_X11=ON ../geant4.10.04.p02'
#alias MkG4Beta='cmake -DCMAKE_INSTALL_PREFIX=/home/chen/HomeWork/geant4.10.04.p02-beta -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=ON ../geant4.10.04.p02'
#alias MkG4Beta='cmake -DCMAKE_INSTALL_PREFIX=/home/chen/HomeWork/geant4.10.02.p03-beta -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=ON ../geant4.10.02.p03'
