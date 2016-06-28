#!/bin/bash
ROOTCDW=$1
ROOTTEST=$2

declare -a sets=("baseline" "cameraJitter" "dynamicBackground" "intermittentObjectMotion" "shadow" "thermal")

declare -a baseline=("highway" "office" "pedestrians" "PETS2006")
declare -a cameraJitter=("badminton" "boulevard" "sidewalk" "traffic")
declare -a dynamicBackground=("boats" "canoe" "fall" "fountain01" "fountain02" "overpass")
declare -a intermittentObjectMotion=("abandonedBox" "parking" "sofa" "streetLight" "tramstop" "winterDriveway")
declare -a shadow=("backdoor" "bungalows" "busStation" "copyMachine" "cubicle" "peopleInShade")
declare -a thermal=("corridor" "diningRoom" "lakeSide" "library" "park")

intexit() {
  kill -HUP -$$
}

hupexit() {
  echo
  echo "Interrupted"
  exit
}

trap hupexit HUP
trap intexit INT


for j in "${sets[@]}"
do
  URL=${j}"[@]"
  for i in ${!URL}
  do
    INPUT=$ROOTCDW/$j/$i/input
    OUTPUT_CT=$ROOTTEST/$j/$i
    OUTPUT_HU=$ROOTTEST/$j/$i
    #echo "$INPUT"
    if [ ! -d "$OUTPUT_CT" ]; then
      mkdir $OUTPUT_CT
    fi
    echo "$j/$i/"
    python tests/segmentation_lima.py -f -i $INPUT -uc -l1 5.0 -l2 3.0 -o $OUTPUT_CT
    #if [ ! -d "$OUTPUT_HU" ]; then
    #  mkdir $OUTPUT_HU
    #fi
    #echo "$j/$i/hulls"
    #python tests/segmentation_lima.py -f -i $INPUT -uh -l1 5.0 -l2 3.0 -o $OUTPUT_HU
  done
done

wait
