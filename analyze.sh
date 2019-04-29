for folder in 20190423
do
. '/usr/local/root-6.04.02/bin/thisroot.sh'	
cd ~/sPHENIX/LightTransmission
mkdir "pictures/$folder/Analysis"
	for dbn in 335 310 314 239
	do
    	for end in N W
    	do
		echo processing DBN_$dbn-$end
		root -l -q -b 'crop.cpp('$dbn', "'$end'", "'pictures/$folder/Original'", "'pictures/rootPic'", "'pictures/cropped'")'
		root -l -q -b 'fiberCounter.C('$dbn',"'$end'","'pictures/rootPic'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
    	done
	done
root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
cp 'analyze.sh' "pictures/$folder/Analysis"

cp pictures/$folder/Analysis/$folder\_1end.csv  updatedb/blocks.csv
cd updatedb
python update_fibercounting.py
done

