#!/bin/bash 

for i in {11873..11882}
do 
  echo $i
  kill $i 
  #./MyG4HW run_10k.mac $i >& log.$i.galactic &
  #./MyG4HW run_10k.mac test_galactic.$i.root >& log.$i._galactic &
done

