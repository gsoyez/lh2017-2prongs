#!/bin/bash
#
# Helper to extract ntuiples of a given expression under specific (optional) conditions
#
# Usage:
#  extract.sh "ntuple_file" "expression" "optional_condition_list"

function usage {
    echo "Usage:"
    echo "  extract.sh \"ntuple_file\" \"expression\" \"optional_condition_list\""
    exit
}

#----------------------------------------------------------------------
# parse the command line
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; expression="$1" ; shift

nconditions=0
while [[ $# -gt 0 ]]; do
    condition=${1//\"/}
    shift
    conditions[$nconditions]="$condition"
    ((nconditions++))
done

echo "# N-tuple file: ${ntuple_file}"
echo "# binning '${expression}' under the conditions '${conditions[*]}'"

#----------------------------------------------------------------------
# build the awk expression string

# get the column contents
colstring=`grep -m1 "#columns: " ${ntuple_file}`
colstring=${colstring/"#columns: "/}
read -a colnames <<< "$colstring"

# we also want to have a few helpers
declare -A helpers
for variant in "plain:plain" "plain:loose" "plain:tight" "loose:loose" "loose:tight" "tight:tight"; do
    num=${variant%:*}
    den=${variant#*:}
    acr="${num:0:1}${den:0:1}"
    for b in "beta1" "beta2"; do
        helpers["tau21_${b}_${acr}"]="(tau2_${b}_${num}/tau1_${b}_${den})"
        helpers["M2_${b}_${acr}"]="(1e3_${b}_${num}/1e2_${b}_${den})"
        helpers["N2_${b}_${acr}"]="(2e3_${b}_${num}/(1e2_${b}_${num}*1e2_${b}_${den}))"
        helpers["D2_${b}_${acr}"]="(3e3_${b}_${num}/(1e2_${b}_${num}**2*1e2_${b}_${den}))"
    done
done
    
# concatenate the expression and conditions in an awk script
awk_script="{"
for condition in ${conditions[*]}; do
    awk_script="${awk_script} if (!(${condition})){ next;} "
done
# add the expression
awk_script="${awk_script}print ${expression} }"

# replace variables by column numbers
#
# We first replace the helpers then the column names
for helper in ${!helpers[*]}; do
    awk_script=${awk_script//$helper/${helpers[$helper]}}
done
index=1
for colname in ${colnames[*]}; do
    awk_script=${awk_script//$colname/"\$"$index}
    ((index++))
done


echo "# awk runs with ${awk_script}"
grep -v "^#" ${ntuple_file} | awk "${awk_script}"
