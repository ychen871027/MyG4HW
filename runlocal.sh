#!/bin/bash

##1: mac 2: outputfile prefix 3: randomseed 4: beamtype 5: material type 6: steplength 7 cut value
## macfile   11     random      pencil      g4air/vg4water/eleWater  1(unit)cm(minus:off)  1(unit:mm)


export ROOTOUT=/home/chen/data/WaterwithAir_Output/OS_10.2.3_EmStandardOpt4
#export ROOTOUT=/home/chen/HomeWork/density_build_10.4.beta_debug/testopt4_10um
mkdir -p $ROOTOUT

#./MyG4HW run_test.mac 11 11 pencil g4air -1 1 >& log.1&
#./MyG4HW run_test.mac 11 11 pencil vg4water -1 1 >& log.1&

printf "\n"
echo ">>>>>>>outputroot will be saved into the following path<<<<<<"
echo "............................................................."
echo $ROOTOUT
echo "............................................................."
printf "\n"

echo ">>>>>>start to run the Geant4 simulation from now on<<<<<<<<<"
printf "\n"

if [ -n "$1" ]; then
  echo "....you are running one simulation...."
  ./MyG4HW run_test.mac 11 11 pencil $1 -1 1 >& log.$1&
  jobs
else
  echo "....you are running multiple simulation....."

  jobm="."
  for i in {1..1}
  do
    printf "job <"%2s" > submitting"$jobm"\n" "$i"
    #echo $i $RANDOM
    ./MyG4HW run_10k.mac $i $RANDOM pencil g4air    -1 1 >& log.$i.g4air    & # > /dev/null 2&>1 &
    ./MyG4HW run_10k.mac $i $RANDOM pencil vg4water -1 1 >& log.$i.vg4water & # > /dev/null 2&>1 &
    ./MyG4HW run_10k.mac $i $RANDOM pencil eleWater -1 1 >& log.$i.eleWater & # > /dev/null 2&>1 &
    #./MyG4HW run_10k.mac $i $RANDOM > /dev/null 2&>1&
    #./MyG4HW run_10k.mac $i $RANDOM >& log.$i.$RANDOM &
    #./MyG4HW run_10k.mac test_galactic.$i.root >& log.$i._galactic &
    jobm=$jobm"."
  done

  printf "\n"
  echo "......The jobs have been submitted into background............"

fi
