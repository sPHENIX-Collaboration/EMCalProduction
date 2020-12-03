#!/bin/bash
#save cropped pictures under $folder directly, with original photos renamed in folder $folder/Original. Crop symmetrically. Photo naming format: DBN_XXXX-N.JPG. #Use four digits only and change N to W for wide end picture
cd /home/sickles-lab/sPHENIX/LightTransmission
source /home/sickles-lab/anaconda3/etc/profile.d/conda.sh
eval "$(conda shell.bash hook)"
conda activate ocr

for folder in $1
do


  mkdir -p "pictures/$folder/Analysis"
  for entry in pictures/$folder/*.JPG
	do
    	tmpf=${entry%/*}
	    tmp=${a##*/}
    	b=${tmp%.*}
      tmpdbn=${b%-*}
	  echo "processing $b'
  	mv pictures/$folder/$entry.JPG pictures/cropped/$b.JPG
  	root -b -q -l 'fiberCounter_number.C("'${c##*DBN_}'","'${b##*-}'","'pictures/cropped'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
    cp pictures/cropped/$b.JPG ~/google-drive/QA\ tests/Light\ Transmission\ Test/Block\ pictures/Cropped
  done
  fn=${folder%.*}
  dot=${folder#*.}
  new=$fn.$dot
  root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$new'")'
  cp 'analyze_number.sh' "pictures/$folder/Analysis"

  cp pictures/$folder/Analysis/$folder\_1end.csv  updatedb/blocks.csv
  cp -r pictures/$folder/Original ~/google-drive/QA\ tests/Light\ Transmission\ Test/Block\ pictures/Original/$folder
  cp -r pictures/$folder/Analysis ~/google-drive/QA\ tests/Light\ Transmission\ Test/Analysis/$folder\_pic$folder
  cd updatedb
  conda deactivate
  conda deactivate
  python update_fibercounting.py


done
