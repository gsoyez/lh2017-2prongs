#!/bin/bash
#
# Helper to build histograms out of the ntuples
#
# Usage:
#   get_efficiency.sh "ntuple_file" "optional_condition_list"

function usage {
    echo "Usage:"
    echo "  get_efficiency.sh \"ntuple_file_S\" \"optional_condition_list\""
    exit
}

#----------------------------------------------------------------------
# parse the command line
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file="$1"; shift
nconditions=0
while [[ $# -gt 0 ]]; do
    condition=${1//\"/}
    shift
    conditions[$nconditions]="$condition"
    ((nconditions++))
done

#----------------------------------------------------------------------
# build the awk expression string

# get the column contents
colstring=`grep -m1 "#columns: " ${ntuple_file}`
colstring=${colstring/"#columns: "/}
read -a colnames <<< "$colstring"

# concatenate the expression and conditions in an awk script
awk_script="BEGIN{n=0}{"
for condition in ${conditions[*]}; do
    awk_script="${awk_script} if (!(${condition})){ next;} "
done
# record the event
awk_script="${awk_script}n=n+1}END{print n}"

# replace variables bby column numbers
index=1
for colname in ${colnames[*]}; do
    awk_script=${awk_script//$colname/"\$"$index}
    ((index++))
done

ntotal=`tail -n1 ${ntuple_file} | sed 's/.*=//'`
nkept=`grep -v "^#" ${ntuple_file} | awk "${awk_script}"`

echo "$nkept.0/$ntotal" | bc -l
