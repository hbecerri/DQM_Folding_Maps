
#!/bin/bash

read -p "eta: " eta
read -p "phi: " phi

cd ../Demo/DemoAnalyzer/python
cmsRun ConfFile_cfg.py 
mv *.root ../../../Tracking_DQM
cd ../../../Tracking_DQM

rm -f *.txt
rm -f *.pdf

x_bin=$(python -<<EOF
var1=$eta
n=0
for i in range(11):
    if var1 - i*0.6 > -3:
        n +=1
    else:
        break
print('{}'.format(n))
EOF
)

y_bin=$(python -<<EOF
var2 = $phi
n=-10
for i in range(20):
    if var2 - i*0.32 > -3.2:
        n +=1
    else:
        break
if n <= 0:
    n= n-1
print('{}'.format(n))
EOF
)

#def. new variable for folding method 2
if (( y_bin == 1  |  y_bin == 2  |  y_bin == 3  |  y_bin == 4  |  y_bin == 5 ))
then
f_bin=$((5+$y_bin))
fi
if (( y_bin == -1 ||  y_bin == -10 ))
then
f_bin=5
fi
if (( y_bin == -9  ||  y_bin == -2 ))
then
f_bin=4
fi
if (( y_bin == -8  ||  y_bin == -3 ))
then
f_bin=3
fi
if (( y_bin == -7  ||  y_bin == -4 ))
then
f_bin=2
fi
if (( y_bin == -6  ||  y_bin == -5 ))
then
f_bin=1
fi

#array to def. the cuts in pT
#declare -a array=("0" "3" "6" "9" "12" "15" "18" "21" "24" "27" "30" "33" "36" "39" "42")

#array to only draw the folfed maps.
declare -a array=("0")

# get length of an array
arraylength=${#array[@]}

# use for loop to run over all the cuts
for (( i=1; i<${arraylength}+1; i++ ));
do
max=${array[$i-1]}
var_eta='tree_track_phi'
var_phi='tree_track_eta'
cut='tree_track_pt'
root -l -b -q "Folding_map.C(\"${var_eta}\",\"${var_phi}\",\"${cut}\",${max},${x_bin},${y_bin},${f_bin})"
done

#Create Ratio Plot
root -l -b -q "Plot_ratio.C"
