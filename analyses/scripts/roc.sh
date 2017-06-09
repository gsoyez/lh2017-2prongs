#!/bin/bash
#
# Helper to build histograms out of the ntuples
#
# Usage:
#   roc.sh "ntuple_file_S" "ntuple_file_B" eps_S_min eps_S_max delta_eps_S "optional_condition_list"

function usage {
    echo "Usage:"
    echo "  roc.sh \"ntuple_file_S\" \"ntuple_file_B\" \"expression\" vmin vmax dv \"optional_condition_list\""
    exit
}

#----------------------------------------------------------------------
# parse the command line
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file_S="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file_B="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; expression="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; vmin="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; vmax="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; dv="$1"  ; shift

cdtstring=''
while [[ $# -gt 0 ]]; do
    cdtstring="${cdtstring} \"$1\""
    shift
done

# get the number of entries in the file
NentriesS=`tail -n1 ${ntuple_file_S} | sed 's/.*=//'`
NentriesB=`tail -n1 ${ntuple_file_B} | sed 's/.*=//'`

paste <(./histogram.sh "${ntuple_file_S}" "${expression}" ${vmin} ${vmax} ${dv} ${cdtstring}) \
      <(./histogram.sh "${ntuple_file_B}" "${expression}" ${vmin} ${vmax} ${dv} ${cdtstring}) \
    | grep -v "^#" \
    | awk -v nStot=${NentriesS}.0 -v nBtot=${NentriesB}.0 'BEGIN{nS=0; nB=0}{nS=nS+$4; nB=nB+$8; print nS/nStot,nB/nBtot}'



