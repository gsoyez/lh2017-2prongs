#!/bin/bash
#
# create a tale with the basic quality measures
echo "# 60<m<100, R=1, Pythia8(4C), lhc14"
echo "#columns: combination pt  sig_truth_hadron Srob_truth_hadron Brob_truth_hadron sig_hadron_parton Srob_hadron_parton Brob_hadron_parton"

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

                echo -n "${base}_beta${beta}_${level} ${pt}"
                
                # various comparisons
                for level_comp in "truth:hadron" "hadron:parton"; do
                    lref=${level_comp%:*}
                    lalt=${level_comp#*:}
                    q=`../scripts/get_perf_robustness.sh ../res/lhc14-WW${pt}-pythia84C-${lref}.ntuple.gz ../res/lhc14-dijets${pt}-pythia84C-${lref}.ntuple.gz ../res/lhc14-WW${pt}-pythia84C-${lalt}.ntuple.gz ../res/lhc14-dijets${pt}-pythia84C-${lalt}.ntuple.gz "${base}_beta${beta}_${lshape}" 0.4 "m_${mass}>60.0" "m_${mass}<100.0"`
                    echo -n " ${q}"
                done
                echo ""
            done # pt loop
        done # grooming level
    done # beta
done # shapes

            

