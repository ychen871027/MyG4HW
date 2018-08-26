#!/bin/bash

##1: mac 2: outputfile prefix 3: randomseed 4: beamtype 5: material type 6: steplength 7 cut value 8 MSCstepAlg
## macfile   11     random      pencil      g4air/vg4water/eleWater  1(unit)cm(minus:off)  1(unit:mm)

printf "\n"
echo ">>>>>>>outputroot will be saved into the following path<<<<<<"
echo "............................................................."
echo $ROOTOUT
echo "............................................................."
printf "\n"

echo ">>>>>>start to run the Geant4 simulation from now on<<<<<<<<<"
printf "\n"

MSCstepAlg="fMinimal
fUseSafety
fUseSafetyPlus
fUseDistanceToBoundary
"

for MSCSTEPALG in $MSCstepAlg
do
echo "type: " $MSCSTEPALG
prefix="MyPL_MSCOnly_"
#prefix="MyPL_MSCOnly_"
prefix=$prefix$MSCSTEPALG
SaveDir="WaterwithAir_Output_Length"
if [ -n "$1" ]; then
  echo "....you are running one simulation...."
  export ROOTOUT=/home/chen/data/$SaveDir/Beta_10.4.2_${prefix}_g4air
  mkdir -p $ROOTOUT
  ./MyG4HW run_test.mac 11 11 pencil g4air    -1 1 $MSCSTEPALG >& $ROOTOUT/log.g4air.$1&
  
  export  ROOTOUT=/home/chen/data/$SaveDir/Beta_10.4.2_${prefix}_vg4water
  mkdir -p $ROOTOUT
  ./MyG4HW run_test.mac 11 11 pencil vg4water -1 1 $MSCSTEPALG >& $ROOTOUT/log.vg4water.$1&
  
  export ROOTOUT=/home/chen/data/$SaveDir/Beta_10.4.2_${prefix}_elewater
  mkdir -p $ROOTOUT
  ./MyG4HW run_test.mac 11 11 pencil eleWater -1 1 $MSCSTEPALG >& $ROOTOUT/log.elewater.$1&

else
  echo "....you are running multiple simulation....."

  jobm="."
  for i in {1..1}
  do
    printf "job <"%2s" > submitting"$jobm"\n" "$i"
    #echo $i $RANDOM

    export ROOTOUT=/home/chen/data/$SaveDir/Beta_10.4.2_${prefix}_g4air
    mkdir -p $ROOTOUT
    ./MyG4HW run_10k.mac $i $RANDOM pencil g4air    -1 1 $MSCSTEPALG >& $ROOTOUT/log.$i.g4air    & 
    export  ROOTOUT=/home/chen/data/$SaveDir/Beta_10.4.2_${prefix}_vg4water
    mkdir -p $ROOTOUT
    ./MyG4HW run_10k.mac $i $RANDOM pencil vg4water -1 1 $MSCSTEPALG >& $ROOTOUT/log.$i.vg4water & 
    export ROOTOUT=/home/chen/data/$SaveDir/Beta_10.4.2_${prefix}_elewater
    mkdir -p $ROOTOUT
    ./MyG4HW run_10k.mac $i $RANDOM pencil eleWater -1 1 $MSCSTEPALG >& $ROOTOUT/log.$i.eleWater & 

    jobm=$jobm"."
  done

  printf "\n"
  echo "......The jobs have been submitted into background............"

fi
done
jobs
