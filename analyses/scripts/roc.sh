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

#----------------------------------------------------------------------
# decide which too to use according to file compression
mygrep="grep"
mycat="cat"
if [[ "$ntuple_file_S" == *.gz ]]; then
    mygrep="zgrep"
    mycat="zcat"
fi

#----------------------------------------------------------------------

script_full_path=$(dirname "$0")

cdtstring=''
while [[ $# -gt 0 ]]; do
    cdtstring="${cdtstring} \"$1\""
    shift
done

# get the number of entries in the file
NentriesS=`${mycat} ${ntuple_file_S} | tail -n1 | sed 's/.*=//'`
NentriesB=`${mycat} ${ntuple_file_B} | tail -n1 | sed 's/.*=//'`

paste <(${script_full_path}/histogram.sh "${ntuple_file_S}" "${expression}" ${vmin} ${vmax} ${dv} ${cdtstring}) \
      <(${script_full_path}/histogram.sh "${ntuple_file_B}" "${expression}" ${vmin} ${vmax} ${dv} ${cdtstring}) \
    | grep -v "^#" \
    | awk -v nStot=${NentriesS}.0 -v nBtot=${NentriesB}.0 'BEGIN{nS=0; nB=0}{nS=nS+$4; nB=nB+$8; print nS/nStot,nB/nBtot}'



