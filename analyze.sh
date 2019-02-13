for dbn in 5 9 11 13 17 23 32 34 35 37 38 40 41 42 46 47 48 61 65 66 68 69 70 71 80 81 83 88 89 96 97 100 101 104 110 120 124 131 136 138 140 143 145 147 155 157 158 159
do
    for end in N W
    do
	echo processing DBN_$dbn-$end
	root -l -q -b 'fiberCounter2.C('$dbn',"'$end'")'
    done
done

