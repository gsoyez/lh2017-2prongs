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

# concatenate the expression and conditions in an awk script
awk_script="{"
for condition in ${conditions[*]}; do
    awk_script="${awk_script} if (!(${condition})){ next;} "
done
# add the expression
awk_script="${awk_script}print ${expression} }"

# replace variables bby column numbers
index=1
for colname in ${colnames[*]}; do
    awk_script=${awk_script//$colname/"\$"$index}
    ((index++))
done


echo "# awk runs with ${awk_script}"
grep -v "^#" ${ntuple_file} | awk "${awk_script}"
