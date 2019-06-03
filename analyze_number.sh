for folder in 20190603
do
cd ~/sPHENIX/LightTransmission
source ~/anaconda3/etc/profile.d/conda.sh
eval "$(conda shell.bash hook)"
conda activate ocr

mkdir "pictures/$folder/Analysis"
mkdir "pictures/$folder/Original"
mkdir "pictures/$folder/Temp"
	for entry in pictures/$folder/*.JPG
	do
  		echo ${entry:(-12):12}
  		root -b -q -l 'crop.cpp("'${entry:(-12):8}'","'${entry:0:(${#entry}-13)}'","'pictures/cropped'","'pictures/$folder/Temp'")'
  		convert pictures/$folder/Temp/${entry:(-12):8}-number.JPG -threshold 10% pictures/$folder/Temp/${entry:(-12):8}-numberbw.JPG 
  		dbnpic=$(python number.py pictures/$folder/Temp/${entry:(-12):8}-numberbw.JPG pictures/templates 2>&1)
  		mv $entry pictures/$folder/Original/$dbnpic
  		mv pictures/cropped/${entry:(-12):8}-block.JPG pictures/cropped/$dbnpic
  		root -b -q -l 'fiberCounter_number.C("'${dbnpic:(-10):4}'","'${dbnpic:(-5):1}'","'pictures/cropped'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
  		rm pictures/$folder/Temp/${entry:(-12):8}-number.JPG
  		rm pictures/$folder/Temp/${entry:(-12):8}-numberbw.JPG
	done
rm -r pictures/$folder/Temp
root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
cp 'analyze_number.sh' "pictures/$folder/Analysis"

cp pictures/$folder/Analysis/$folder\_1end.csv  updatedb/blocks.csv
cd updatedb
conda deactivate
conda deactivate
python update_fibercounting.py
done
