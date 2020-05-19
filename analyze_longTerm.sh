for folder in $1
  do
  #cd to master folder and source your conda and root
  cd /home/sickles-lab/sPHENIX/LightTransmission
  source /home/sickles-lab/anaconda3/etc/profile.d/conda.sh
  
  eval "$(conda shell.bash hook)"
  conda activate ocr

  mkdir "pictures/monitoring/$folder/Analysis"
  mkdir "pictures/monitoring/$folder/Original"
  mkdir "pictures/monitoring/$folder/Temp"
  for entry in pictures/$folder/*.JPG
	 do
    echo ${entry:(-12):12}
    root -b -q -l 'crop.cpp("'${entry:(-12):8}'","'${entry:0:(${#entry}-13)}'","'pictures/monitoring/cropped'","'pictures/monitoring/$folder/Temp'")'
    convert pictures/monitoring/$folder/Temp/${entry:(-12):8}-number.JPG -threshold 10% pictures/monitoring/$folder/Temp/${entry:(-12):8}-numberbw.JPG 
  	dbnpic=$(python number.py pictures/monitoring/$folder/Temp/${entry:(-12):8}-numberbw.JPG pictures/templates 2>&1)
  	mv $entry pictures/monitoring/$folder/Original/$dbnpic
  	mv pictures/monitoring/cropped/${entry:(-12):8}-block.JPG pictures/monitoring/cropped/$dbnpic
  	root -b -q -l 'fiberCounter_number.C("'${dbnpic:(-10):4}'","'${dbnpic:(-5):1}'","'pictures/monitoring/cropped'","'pictures/monitoring/$folder/Analysis'","'pictures/monitoring/$folder/Analysis/$folder\_2ends.csv'")'
  	rm pictures/monitoring/$folder/Temp/${entry:(-12):8}-number.JPG
  	rm pictures/monitoring/$folder/Temp/${entry:(-12):8}-numberbw.JPG
    cp pictures/monitoring/cropped/$dbnpic ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Monitoring/Cropped
  done
  rm -r pictures/monitoring/$folder/Temp
  root -l -q -b 'getBetter2.cpp("'pictures/monitoring/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/monitoring/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
  #cp 'analyze_number.sh' "pictures/$folder/Analysis"

  #cp pictures/monitoring/$folder/Analysis/$folder\_1end.csv  updatedb_monitor/blocks.csv
  #cp -r pictures/monitoring/$folder/Original ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Monitoring/Original/$folder
  #cp -r pictures/monitoring/$folder/Analysis ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Monitoring/Analysis/$folder\_pic$folder
  #cd updatedb_monitor
  #conda deactivate
  #conda deactivate
  #python update_fibermonitoring.py


done
