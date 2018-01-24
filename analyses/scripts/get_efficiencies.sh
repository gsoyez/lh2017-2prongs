#!/bin/bash
#
# for a given signal efficiency for the reference level (defining a
# fixed cut on the shape variable), return the signal efficiency for
# the alternative level and the backgroung efficiency for both the
# reference and alternate background files
#
# Usage:
#  get_efficiencies.sh "ntuple_file_S_reference" "ntuple_file_B_reference" "ntuple_file_S_alternative" "ntuple_file_B_alternative" "expression" requested_efficiency "optional_condition_list"
#
# Output:
#   eps_S_alt  eps_B_ref  eps_B_alt
#
# This should allow us tu build any quality measures later

function usage {
    echo "Usage:"
    echo "  get_efficiencies.sh \"ntuple_file_S_reference\" \"ntuple_file_B_reference\" \"ntuple_file_S_alternative\" \"ntuple_file_B_alternative\" \"expression\" \"requested_efficiency \"optional_condition_list\""
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

if [[ -z ${vcut} ]]; then
    echo "0.0 0.0 0.0"
    exit 1
fi

effBref=`${script_full_path}/get_efficiency.sh "${ntuple_file_B_reference}"   ${cdtstring} "${expression}<${vcut}"`
effSalt=`${script_full_path}/get_efficiency.sh "${ntuple_file_S_alternative}" ${cdtstring} "${expression}<${vcut}"`
effBalt=`${script_full_path}/get_efficiency.sh "${ntuple_file_B_alternative}" ${cdtstring} "${expression}<${vcut}"`

echo "$effSalt $effBref $effBalt"


