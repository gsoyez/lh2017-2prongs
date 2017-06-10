#!/bin/bash
#
# get the cut on a variable that would correspond to a requested signal efficiency
#
# Usage:
#  get_perf_robustness.sh "ntuple_file_S_reference" "ntuple_file_B_reference" "ntuple_file_S_alternative" "ntuple_file_B_alternative" "expression" requested_efficiency "optional_condition_list"
#
# Output:
#      eff_ref          robu_S         robu_B
#   [S/sqrt(B)]_ref  [S_alt/S_ref]  [B_alt/B_ref]
#
# Note:
#   (eff_alt-eff_ref)/(eff_alt+eff_ref) = (eff_alt/eff_ref-1)/(eff_alt/eff_ref+1)
#                                       = (rubu_S/sqrt(robu_B)-1)/(rubu_S/sqrt(robu_B)+1)
#                                       = (rubu_S-sqrt(robu_B))/(rubu_S+sqrt(robu_B))
function usage {
    echo "Usage:"
    echo "  get_perf_robustness.sh \"ntuple_file_S_reference\" \"ntuple_file_B_reference\" \"ntuple_file_S_alternative\" \"ntuple_file_B_alternative\" \"expression\" \"requested_efficiency \"optional_condition_list\""
    exit
}

#----------------------------------------------------------------------
# parse the command line
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file_S_reference="$1"  ; shift
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file_B_reference="$1"  ; shift
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file_S_alternative="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; ntuple_file_B_alternative="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; expression="$1"; shift
if [[ $# -eq 0 ]]; then usage; fi; effSref="$1"   ; shift

#----------------------------------------------------------------------
script_full_path=$(dirname "$0")

cdtstring=''
while [[ $# -gt 0 ]]; do
    cdtstring="${cdtstring} \"$1\""
    shift
done

# we first need to get the cut as obtained y the reference signal
vcut=`${script_full_path}/get_cut.sh "${ntuple_file_S_reference}" "${expression}" $effSref ${cdtstring}`
effBref=`${script_full_path}/get_efficiency.sh "${ntuple_file_B_reference}"   ${cdtstring} "${expression}<${vcut}"`
effSalt=`${script_full_path}/get_efficiency.sh "${ntuple_file_S_alternative}" ${cdtstring} "${expression}<${vcut}"`
effBalt=`${script_full_path}/get_efficiency.sh "${ntuple_file_B_alternative}" ${cdtstring} "${expression}<${vcut}"`

# and construct the observables: perf and robustness
perf=`echo "$effSref/sqrt($effBref)" | bc -l`
#robu=`echo "($effSalt/sqrt($effBalt)-$effSref/sqrt($effBref))/($effSalt/sqrt($effBalt)+$effSref/sqrt($effBref))" | bc -l`
robuS=`echo "$effSalt/$effSref" | bc -l`
robuB=`echo "$effBalt/$effBref" | bc -l`
echo "$perf $robuS $robuB"


