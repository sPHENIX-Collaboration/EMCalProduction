source /home/sickles-lab/anaconda3/etc/profile.d/conda.sh
eval "$(conda shell.bash hook)"

for folder in 20190514
do
. '/usr/local/build/bin/thisroot.sh'	
cd /home/sickles-lab/sPHENIX/LightTransmission
mkdir "pictures/$folder/Analysis"
	for dbn in 393 346 683 183
	do
    	for end in N W
    	do
		echo processing DBN_$dbn-$end
		root -l -q -b 'fiberCounter_number.C('$dbn',"'$end'","'pictures/cropped'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
    	done
	done
root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
cp 'analyze.sh' "pictures/$folder/Analysis"

cp pictures/$folder/Analysis/$folder\_1end.csv  updatedb/blocks.csv
cp -r pictures/$folder/Original ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Original/$folder
cp -r pictures/$folder/Analysis ~/google-drive/Light\ Transmission\ Test/Analysis/$folder\_pic$folder
cd updatedb
conda deactivate
conda deactivate
python update_fibercounting.py
done