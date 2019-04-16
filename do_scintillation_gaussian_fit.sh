for folder in 20190412
do
	cd ~/sPHENIX/ScintillationTest
    for dbn in 14 109 232 572
    do
	root -l -q -b 'scintillation_gaussian_fit.C('$dbn',"'$folder'","'$folder/$folder\_result.csv'")'
    done
cp 'do_scintillation_gaussian_fit.sh' "$folder/Analysis"
done
