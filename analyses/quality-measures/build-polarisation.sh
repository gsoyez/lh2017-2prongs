#!/bin/bash
#
# create a tale with the basic quality measures for the polarisation robustness
#
# Here we fix the cut on the shape so as to acheived a given signal
# (W) efficiency as computed on SM WW events. For that cut, we compute
# the signal efficiency for WW resonnances (transverse and
# longitudinal) and for the QCD background.
#
# everything is done at particle/truth level
#
# all we have to do here is compute the background efficiency for a
# signal efficiency of 0.4, varying our definition of the signal

levelpairs="sm_trans sm_long"
Rs="0.8 1.0"

for R in $Rs; do
    file="qualities-polarisation-R${R}.new"
    echo "# lhc14, Pythia8.230(Monash13), R=${R}, 65<m<105, epsilon_S(first)=0.4"  | tee -a ${file}
    echo "#columns: combination pt epsilon_S_SM epsilon_S_trans epsilon_S_long  epsilon_B_sm"  | tee -a ${file}

    # basic shapes
    for base in tau21 D2 M2 N2; do
        # lopo over beta values
        for beta in 1 2; do    # mass, num, den (mass<=den<=num)
            for level in ppp lll lpl lpp ttt tlt tpt tll tpl tpp trimmed; do
                m=${level:0:1}
                case $m in
                    p) mass="plain"
                       ;;
                    l) mass="loose"
                       ;;
                    t) mass="tight"
                       ;;
                esac
                lshape=${level:1:3}

                if [[ ${level} == "trimmed" ]]; then
                    m="trim"
                    lshape="trimmed"
                fi
                
                # loop over pt values
                for pt in 500 1000 2000; do

                    M=$((${pt}*3))

                    echo -n "${base}_beta${beta}_${level} ${pt} 0.4"  | tee -a ${file}

                    # get the cut from the SM signal
                    vcut=`../scripts/get_cut.sh ../res/lhc14-WW${pt}-pythia8230M13-truth-R${R}.ntuple.gz "${base}_beta${beta}_${lshape}" 0.4 "m_${mass}>65.0" "m_${mass}<105.0"`

                    if [[ -z ${vcut} ]]; then
                        echo " 0.0 0.0 0.0"  | tee -a ${file}
                    else
                        # for that cut, get the signal efficiency for the transverse WW resonnance, the longitudinal one and the QCD background
                        effT=`../scripts/get_efficiency.sh ../res/lhc14-WW-M${M}-trans-pythia8M13-R${R}.ntuple.gz       "m_${mass}>65.0" "m_${mass}<105.0" "${base}_beta${beta}_${lshape}<${vcut}"`
                        effL=`../scripts/get_efficiency.sh ../res/lhc14-WW-M${M}-long-pythia8M13-R${R}.ntuple.gz        "m_${mass}>65.0" "m_${mass}<105.0" "${base}_beta${beta}_${lshape}<${vcut}"`
                        effB=`../scripts/get_efficiency.sh ../res/lhc14-dijets${pt}-pythia8230M13-truth-R${R}.ntuple.gz "m_${mass}>65.0" "m_${mass}<105.0" "${base}_beta${beta}_${lshape}<${vcut}"`
                        
                        echo " ${effT} ${effL} ${effB}"  | tee -a ${file}
                    fi
                        
                done # pt loop
            done # grooming level
        done # beta
    done # shapes

done # R values    

