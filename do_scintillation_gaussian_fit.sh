for tester in Xiaoning
	do
	for folder in 20190605
		do
		cd ~/sPHENIX/ScintillationTest
    	for dbn in $folder/*.txt
    		do
			root -l -q -b 'scintillation_gaussian_fit.C('${dbn:12:(${#dbn}-15)}',"'$folder'","'$folder/$folder\_result.csv'")'
    	done
    	root -l -q -b 'doRatio.cpp("'$folder'","'$folder\_result.csv'","'$folder/$folder\_ratio.csv'", "'$tester'")'
		cp 'do_scintillation_gaussian_fit.sh' "$folder"
		cp $folder/$folder\_ratio.csv  updatedb/blocks.csv
		cd updatedb
		python update_scintillation.py


	done
done
