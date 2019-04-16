for folder in 20190410
do
cd ~/sPHENIX/LightTransmission
mkdir "pictures/$folder/Analysis"
	for dbn in 19 90 109 128 141 232 572
	do
    	for end in N W
    	do
		echo processing DBN_$dbn-$end
		root -l -q -b 'crop.cpp('$dbn', "'$end'", "'pictures/$folder/Original'", "'pictures/rootPic'", "'pictures/cropped'")'
		root -l -q -b 'fiberCounter.C('$dbn',"'$end'","'pictures/rootPic'","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/$folder\_2ends.csv'")'
    	done
	done
root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'")'
cp 'analyze.sh' "pictures/$folder/Analysis"
done

