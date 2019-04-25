for folder in 20190326
do
. '/usr/local/root-6.04.02/bin/thisroot.sh'	
cd ~/sPHENIX/LightTransmission
	for dbn in 29 44 73 95 102 103 127 133 142 156 160 170 179 186 187 190 236
	do
    	for end in N W
    	do
		echo processing DBN_$dbn-$end
		root -l -q -b 'cropFast.cpp('$dbn', "'$end'", "'pictures/$folder/Original'", "'IgnoreMe/limits.csv'")'
    	done
	done
done

