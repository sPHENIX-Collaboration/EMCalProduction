for folder in 20190218
do
    for dbn in 15 16 27 30 85 92 97 99 106 108 112 116 119 125 130 135 153 161 167 175
    do
	root -l -q -b 'scintillation_gaussian_fit.C('$dbn',"'$folder'")'
    done
done
