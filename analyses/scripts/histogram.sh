#!/bin/bash
#
# Helper to build histograms out of the ntuples
#
# Usage:
#  histogram.sh "ntuple_file" "expression" lo hi binsize "optional_condition_list"

function usage {
    echo "Usage:"
    echo "  histogram.sh \"ntuple_file\" \"expression\" lo hi binsize \"optional_condition_list\""
    exit
}

#----------------------------------------------------------------------
# parse the command line
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; expression="$1" ; shift
if [[ $# -eq 0 ]]; then usage; fi; hist_lo="$1"    ; shift
if [[ $# -eq 0 ]]; then usage; fi; hist_hi="$1"    ; shift
if [[ $# -eq 0 ]]; then usage; fi; binsize="$1"    ; shift

script_full_path=$(dirname "$0")

cdtstring=''
while [[ $# -gt 0 ]]; do
    cdtstring="${cdtstring} \"$1\""
    shift
done

${script_full_path}/extract.sh "${ntuple_file}" "${expression}" ${cdtstring} | ${script_full_path}/histogram.pl $hist_lo $hist_hi $binsize


