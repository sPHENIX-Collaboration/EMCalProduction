#source /home/sickles-lab/anaconda3/etc/profile.d/conda.sh
#eval "$(conda shell.bash hook)"

for folder in 
do
#. '/usr/local/build/bin/thisroot.sh'	
#cd /home/sickles-lab/sPHENIX/LightTransmission
mkdir "pictures/$folder/Analysis"
	#for dbn in 164 90164 172 90172 184 90184 190 90190 210 90210 226 90226 321 90321 353 90353 371 90371 381 90381 383 90383 664 90664 759 90759 1070 91070 1112 91112 1370 91370 1377 91377 1392 91392 1394 91394
	#for dbn in LT_278_20200306 LT_PSR_380_20200306 LT_278_20200306 LT_PSR_380_20200306 LT_360_20200306 LT_PSR_415_20200306 LT_360_20200306 LT_PSR_415_20200306 LT_433_20200306 LT_PSR_426_20200306 LT_433_20200306 LT_PSR_426_20200306 LT_515_20200306	 LT_PSR_496_20200306 LT_515_20200306 LT_PSR_496_20200306 LT_PSR_193_20200306 LT_PSR_503_20200306 LT_PSR_193_20200306 LT_PSR_503_20200306 LT_PSR_299_20200306 LT_PSR_651_20200306 LT_PSR_299_20200306 LT_PSR_651_20200306 LT_PSR_351_20200306 LT_PSR_676_20200306 LT_PSR_351_20200306 LT_PSR_676_20200306
	#for dbn in LT_PSR_1181_20200306 LT_726_20200306 LT_PSR_1204_20200306 LT_PSR_471_20200306 LT_PSR_460_20200306 LT_PSR_571_20200306
	#for dbn in LT_726_20200306
	#for dbn in LT_193_20200309 LT_503_20200309 LT_PS_1204_20200309 LT_PS_415_20200309 LT_PS_496_20200309 LT_351_20200309 LT_676_20200309 LT_PS_299_20200309 LT_PS_460_20200309 LT_PS_571_20200309 LT_426_20200309 LT_PS_1181_20200309 LT_PS_380_20200309 LT_PS_471_20200309 LT_PS_651_20200309
	for dbn in LT_351_20200309 LT_676_20200309
	do
    	for end in n w
    	do
		echo processing $dbn-$end
		root -l -q -b 'fiberCounter_monitor.C("'$dbn'","'$end'","pictures/$folder","'pictures/$folder/Analysis'","'pictures/$folder/Analysis/results_all_2ends.csv'")'
    	done
	done
#root -l -q -b 'getBetter2.cpp("'pictures/$folder/Analysis'", "'pictures/$folder\_2ends.csv'","'pictures/$folder/Analysis/$folder\_1end.csv'","'$folder'")'
#cp 'analyze.sh' "pictures/$folder/Analysis"

#cp pictures/$folder/Analysis/$folder\_1end.csv  updatedb/blocks.csv
#cp -r pictures/$folder/Original ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Original/$folder
#cp -r pictures/$folder/Analysis ~/google-drive/Light\ Transmission\ Test/Analysis/$folder\_pic$folder
#cd updatedb
#conda deactivate
#conda deactivate
#python update_fibercounting.py
done



