source /home/sickles-lab/anaconda3/etc/profile.d/conda.sh
eval "$(conda shell.bash hook)"
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
			root -l -q -b 'scintillation_gaussian_fit.C('$DBN',"'$folder'","'$folder/$folder\_result.csv'")'
    		done
		new=$folder
		if [ ${#folder} -gt 8 ]
		then
			fn=${folder%-*}
			dot=${folder#*-}
			new=$fn.$dot
		fi
    		root -l -q -b 'doRatio.cpp("'$folder'","'$folder\_result.csv'","'$folder/$folder\_ratio.csv'", "'$tester'","'$new'")'
		cp 'do_scintillation_gaussian_fit.sh' "$folder"
		cp $folder/$folder\_ratio.csv  updatedb/blocks.csv
		cd updatedb
		conda deactivate
		conda deactivate
		python update_scintillation.py
		
		cd /home/sickles-lab/sPHENIX/ScintillationTest
		cp -r $folder ~/google-drive/ScintillationTest\ Test

	done
done
