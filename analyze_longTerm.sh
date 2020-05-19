for folder in $1
  do
  #cd to master folder and source your conda and root
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
    	b=${tmp%.*}
	echo $b
    	root -b -q -l 'crop.cpp("'$b'","'$tmpf'","'pictures/cropped'","'pictures/$folder/Temp'")'
    	convert pictures/$folder/Temp/$b-number.JPG -threshold 10% pictures/$folder/Temp/$b-numberbw.JPG 
  	dbnpic=$(python number.py pictures/$folder/Temp/$b-numberbw.JPG pictures/templates 2>&1)
  	mv $entry pictures/$folder/Original/$dbnpic
  	mv pictures/monitoring/cropped/$b-block.JPG pictures/cropped/$dbnpic
  	root -b -q -l 'fiberCounter_number.C("'${dbnpic:(-10):4}'","'${dbnpic:(-5):1}'","'pictures/cropped'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
  	rm pictures/$folder/Temp/$b-number.JPG
  	rm pictures/$folder/Temp/$b-numberbw.JPG
  done
  rm -r pictures/monitoring/$folder/Temp
  root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
  #cp 'analyze_number.sh' "pictures/$folder/Analysis"

  #cp pictures/monitoring/$folder/Analysis/$folder\_1end.csv  updatedb_monitor/blocks.csv
  #cp -r pictures/monitoring/$folder/Original ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Monitoring/Original/$folder
  #cp -r pictures/monitoring/$folder/Analysis ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Monitoring/Analysis/$folder\_pic$folder
  #cd updatedb_monitor
  #conda deactivate
  #conda deactivate
  #python update_fibermonitoring.py


done
