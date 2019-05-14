for folder in 20190514
do
. '/usr/local/build/bin/thisroot.sh'	
cd ~/sPHENIX/LightTransmission
mkdir "pictures/$folder/Analysis"
	for dbn in 393 346 683 183
	do
    	for end in N W
    	do
		echo processing DBN_$dbn-$end
		root -l -q -b 'fiberCounter.C('$dbn',"'$end'","'pictures/$folder/Original'","'pictures/cropped'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
    	done
	done
root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
cp 'analyze.sh' "pictures/$folder/Analysis"

cp pictures/$folder/Analysis/$folder\_1end.csv  updatedb/blocks.csv
cd updatedb
python update_fibercounting.py
done

