for folder in sector0_small
cd ~/sPHENIX/LightTransmission
do
mkdir "$folder/Analysis"
	for dbn in 5
	do
    	for end in N W
    	do
		echo processing DBN_$dbn-$end
		root -l -q -b 'fiberCounter.C('$dbn',"'$end'","'pictures/cropped'","'$folder/Analysis'","'$folder/Analysis/$folder\_2ends.csv'")'
    	done
	done
root -l -q -b 'getBetter2.cpp("'$folder/Analysis'", "'$folder\_2ends.csv'","'$folder/Analysis/$folder\_1end.csv'")'
done


