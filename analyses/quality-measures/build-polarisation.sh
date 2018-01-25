#!/bin/bash
#
# create a tale with the basic quality measures for the polarisation robustness
#
# Here the "background" sample is kept fixed (i.e. taken as the same
# for the reference and alternative levels), and the signal is varied:
# we go from SM to longitudinal or transverse WW resonance
#
# THis means that we actually test how much our background rejection
# depends on the sample used to fix the signal efficiency
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
    echo "#columns: combination pt epsilon_S_all epsilon_B_sm epsilon_B_trans epsilon_B_long"  | tee -a ${file}

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
                    effB=`../scripts/get_efficiency.sh ../res/lhc14-dijets${pt}-pythia8230M13-truth-R${R}.ntuple.gz "m_${mass}>65.0" "m_${mass}<105.0" "${base}_beta${beta}_${lshape}<${vcut}"`
                    echo -n " ${effB}"  | tee -a ${file}
                    
                    # get the cut from the SM signal
                    vcut=`../scripts/get_cut.sh ../res/lhc14-WW-M${M}-trans-pythia8M13-R${R}.ntuple.gz "${base}_beta${beta}_${lshape}" 0.4 "m_${mass}>65.0" "m_${mass}<105.0"`
                    effB=`../scripts/get_efficiency.sh ../res/lhc14-dijets${pt}-pythia8230M13-truth-R${R}.ntuple.gz "m_${mass}>65.0" "m_${mass}<105.0" "${base}_beta${beta}_${lshape}<${vcut}"`
                    echo -n " ${effB}"  | tee -a ${file}
                    
                    # get the cut from the SM signal
                    vcut=`../scripts/get_cut.sh ../res/lhc14-WW-M${M}-long-pythia8M13-R${R}.ntuple.gz "${base}_beta${beta}_${lshape}" 0.4 "m_${mass}>65.0" "m_${mass}<105.0"`
                    effB=`../scripts/get_efficiency.sh ../res/lhc14-dijets${pt}-pythia8230M13-truth-R${R}.ntuple.gz "m_${mass}>65.0" "m_${mass}<105.0" "${base}_beta${beta}_${lshape}<${vcut}"`
                    echo -n " ${effB}"  | tee -a ${file}
                    
                    echo ""  | tee -a ${file}
                done # pt loop
            done # grooming level
        done # beta
    done # shapes

done # R values    

