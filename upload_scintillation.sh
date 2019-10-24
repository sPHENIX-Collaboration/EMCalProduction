cd /home/sickles-lab/sPHENIX/ScintillationTest
today=`date +%Y%m%d`
for folder in $today*
do
	if [ ${#folder} -eq 9 ]
	then
		echo "No test done on $today"
	else
		cp -r $folder ~/google-drive/Scintillation\ Test/$folder
	fi
done
