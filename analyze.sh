for folder in 20190307
do
cd ~/sPHENIX/LightTransmission
mkdir "pictures/$folder/Analysis"
	for dbn in 114 117 185 149
	do
    	for end in N W
    	do
		echo processing DBN_$dbn-$end
		root -l -q -b 'fiberCounter.C('$dbn',"'$end'","'pictures/cropped'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
    	done
	done
root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'")'
done


