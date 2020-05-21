for folder in $1
  do
  cd /home/sickles-lab/sPHENIX/LightTransmission
  source /home/sickles-lab/anaconda3/etc/profile.d/conda.sh
  eval "$(conda shell.bash hook)"
  conda activate ocr

  mkdir "pictures/$folder/Analysis"
  mkdir "pictures/$folder/Original"
  mkdir "pictures/$folder/Temp"
  for entry in pictures/$folder/*.JPG
	 do
    	tmpf=${entry%/*}
    	tmp=${entry#*/}
	tmp=${tmp#*/}
    	b=${tmp%.*}
	echo $b
    root -b -q -l 'crop.cpp("'$b'","'$tmpf'","'pictures/cropped'","'pictures/$folder/Temp'")'
    convert pictures/$folder/Temp/$b-number.JPG -threshold 10% pictures/$folder/Temp/$b-numberbw.JPG 
  	dbnpic=$(python number.py pictures/$folder/Temp/$b-numberbw.JPG pictures/templates 2>&1)
  	mv $entry pictures/$folder/Original/$dbnpic
  	mv pictures/cropped/$b-block.JPG pictures/cropped/$dbnpic
  	root -b -q -l 'fiberCounter_number.C("'${dbnpic:(-10):4}'","'${dbnpic:(-5):1}'","'pictures/cropped'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
  	rm pictures/$folder/Temp/$b-number.JPG
  	rm pictures/$folder/Temp/$b-numberbw.JPG
    cp pictures/cropped/$dbnpic ~/google-drive/QA\ tests/Light\ Transmission\ Test/Block\ pictures/Cropped
  done
  rm -r pictures/$folder/Temp
  root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
  cp 'analyze_number.sh' "pictures/$folder/Analysis"

  cp pictures/$folder/Analysis/$folder\_1end.csv  updatedb/blocks.csv
  cp -r pictures/$folder/Original ~/google-drive/QA\ tests/Light\ Transmission\ Test/Block\ pictures/Original/$folder
  cp -r pictures/$folder/Analysis ~/google-drive/QA\ tests/Light\ Transmission\ Test/Analysis/$folder\_pic$folder
  cd updatedb
  conda deactivate
  conda deactivate
  python update_fibercounting.py


done
