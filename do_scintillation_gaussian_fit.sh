for tester in $2
	do
	for folder in $1
		do
		cd /home/sickles-lab/sPHENIX/ScintillationTest
    		for dbn in $folder/*.txt
    			do
			DBN=${dbn%.*}
			DBN=${DBN#*/}
			DBN=${DBN#*dbn}
			root -l -q -b 'scintillation_gaussian_fit.C("'$DBN'","'$folder'","'$folder/$folder\_result.csv'")'
    		done
    		root -l -q -b 'doRatio.cpp("'$folder'","'$folder\_result.csv'","'$folder/$folder\_ratio.csv'", "'$tester'")'
		cp 'do_scintillation_gaussian_fit.sh' "$folder"
		cp $folder/$folder\_ratio.csv  updatedb/blocks.csv
		cd updatedb
		python update_scintillation.py
		
		cd /home/sickles-lab/sPHENIX/ScintillationTest
		cp -r $folder ~/google-drive/ScintillationTest\ Test

	done
done
