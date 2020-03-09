cd /home/sickles-lab/sPHENIX/LightTransmission
today=`date +%Y%m%d`
for folder in pictures/$today*
	do
	if [ ${#folder} -eq 18 ]
	then
		echo "No test done on $today"
	else
	#echo $folder/Original
	#echo $folder/Analysis
		cp -r $folder/Original ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Original/${folder:9:(${#folder}-9)}
  		cp -r $folder/Analysis ~/google-drive/Light\ Transmission\ Test/Analysis/${folder:9:(${#folder}-9)}\_pic${folder:9:(${#folder}-9)}
  		for entry in $folder/Original/*.JPG
  			do
  			#echo ${entry:(-14):14}
			cp pictures/cropped/${entry:(-14):14} ~/google-drive/Light\ Transmission\ Test/Block\ pictures/Cropped/
		done
	fi
done
