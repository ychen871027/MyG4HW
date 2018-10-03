#!/bin/bash

#MyG4HW option
#usage:
#./MyG4HW [options].....
#    -h, --help              show this message.
#    -v, --verbose           show verbose message.
#    -i, --macrofile         set macro file.
#    -s, --seed Number       set seed number.
#    -o, --outputfilename    set Output name.
#    -c, --configurefile     set voxel configure file.
#    -m, --EMstandard        set flag to use EM standard PL.
#    -x, --ImortDensityList  set flag to run density list.

#--------------------configuration file for voxel ---------------------------
#Line1: physicis list
#Line2: how many materias in water phantom 
#Line3: voxel number Nx Ny Nz
#Line4: voxel size x y z [mm]
#Line5: beam type: pencil/broad
#Line6: world volume material construction
#Line7: step limiter flag (-1: off >0: step limit[cm])
#Line8: cut value [mm]
#Line9: MSC algorithms: fMinimal
#Line10: beam particle: gamma/e-
#Line11: pencil: number with []MeV] unit; broad: 6MV/18MV
#Line12: material of water phantom from 5cm to 10cm
#Line13: read density from txt file under main application option[x]
#--------------------------------------------------------------------

printf "\n"
echo ">>>>>>>outputroot will be saved into the following path<<<<<<"
echo "............................................................."
echo $ROOTOUT
echo "............................................................."
printf "\n"

echo ">>>>>>start to run the Geant4 simulation from now on<<<<<<<<<"
printf "\n"

VOXELCONF="voxel_data_broad.dat"

for voxconf in $VOXELCONF
do
echo "voxel configuration: " $voxconf

prefix=$(echo $voxconf | cut -d "." -f1)
echo $prefix
SaveDir="TestTidy"
echo $SaveDir

if [ -n "$1" ]; then
  echo "....you are running one simulation...."

  TDIR=/home/chen/data/
  while [ -d "${TDIR}${SaveDir}" ]
  do
    echo $ROOTOUT" exist"
    SaveDir=${SaveDir}_00
  done

  export ROOTOUT=${TDIR}${SaveDir}
  echo $ROOTOUT
  mkdir -p $ROOTOUT

  ./MyG4HW -i run_test.mac -s $RANDOM -o output.${prefix}.${1} -c $voxconf -v >& $ROOTOUT/log.${1}.$prefix &

  ls -lh $ROOTOUT
else
  echo "....you are running multiple simulation....."

  jobm="."
  #for i in {81..90}
  for i in {1..2}
  do
    printf "job <"%2s" > submitting"$jobm"\n" "$i"
    #echo $i $RANDOM
    export ROOTOUT=/home/chen/data/$SaveDir
    mkdir -p $ROOTOUT
    outputprefix=output.${prefix}.${i}
    while [ -f "$ROOTOUT/log.${outputprefix}" ]
    do
      echo $ROOTOUT/log.${outputprefix}
      echo "filse exist"
      outputprefix=${outputprefix}_00
    done

    echo $outputprefix

    ./MyG4HW -i run_10k.mac -s $RANDOM -o ${outputprefix} -c $voxconf >& $ROOTOUT/log.${outputprefix} &

    jobm=$jobm"."
  done

  printf "\n"
  echo "......The jobs have been submitted into background............"

fi
done
jobs
