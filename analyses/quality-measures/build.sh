#!/bin/bash
#
# create a tale with the basic quality measures

levelpairs="hadron_parton truth_hadron truth_parton pu50_truth ATLAS_truth CMS_truth"
Rs="0.8 1.0"

for R in $Rs; do
    file="qualities-R${R}.new"
    echo "# lhc14, Pythia8(4C), R=${R}, 60<m<100, epsilon_S(first)=0.4"  | tee -a ${file}
    echo "# significance is defined as epsS/sqrt(epsB) calculated on the reference"  | tee -a ${file}
    echo "# correction is defined as (eps_alt-eps_ref)/eps_ref calculated either on the signal or on the background"  | tee -a ${file}
    echo "# Note also that when 2 levels are specified, they are ordered as ref_alt"  | tee -a ${file}
    echo -n "#columns: combination pt "  | tee -a ${file}
    for l in $levelpairs; do
        echo -n " significance_${l%%_*} Scorrection_${l} Bcorrection_${l}"  | tee -a ${file}
    done
    echo ""   | tee -a ${file}

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

                    echo -n "${base}_beta${beta}_${level} ${pt}"  | tee -a ${file}
                
                    # various comparisons
                    for level_comp in $levelpairs; do
                        lref=${level_comp%_*}
                        lalt=${level_comp#*_}
                        q=`../scripts/get_perf_robustness.sh ../res/lhc14-WW${pt}-pythia84C-${lref}-R${R}.ntuple.gz ../res/lhc14-dijets${pt}-pythia84C-${lref}-R${R}.ntuple.gz ../res/lhc14-WW${pt}-pythia84C-${lalt}-R${R}.ntuple.gz ../res/lhc14-dijets${pt}-pythia84C-${lalt}-R${R}.ntuple.gz "${base}_beta${beta}_${lshape}" 0.4 "m_${mass}>60.0" "m_${mass}<100.0"`
                        echo -n " ${q}"  | tee -a ${file}
                    done
                    echo ""  | tee -a ${file}
                done # pt loop
            done # grooming level
        done # beta
    done # shapes

done # R values    

