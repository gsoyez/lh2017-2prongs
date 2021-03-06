reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'optimal.pdf'

call 'defaults.gp'

eS=0.4

levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel 'resilience'
set xrange [0:7]

set ylabel 'performance'
set yrange [0:7]

set grid dt 3 lw 1

set label 102 '{/*0.8 Pythia8(4C), anti-k_t(0.8 or 1)}' right at graph 0.97,0.05
set label 103 '{/*0.8 60<m<100}'                        right at graph 0.97,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)     right at graph 0.97,0.15

#          +   x   *   sqr     o       tr     dia
groomers="ppp lll lpl lpp ttt tlt tpt tll tpl tpp trimmed"
#labs='"tight{/Symbol \304}loose/tight" "loose{/Symbol \304}loose/loose" "tight{/Symbol \304}tight/tight" "trim{/Symbol \304}trim/trim" "tight{/Symbol \304}plain/plain"'

shapes="tau21_beta1 tau21_beta2 N2_beta1 N2_beta2 D2_beta1 D2_beta2 M2_beta1 M2_beta2"
#slabs='"{/Symbol t}_{21}" "N_2" "D_2" "M_2"'

# let us hardcode the column numbers for truth v. parton, ATLAS, CMS and pileup
merge(pt)="< bash -c \" cat <( grep -v '^#' ../quality-measures/qualities-R0.8.res | sed 's/^/R0.8-/g') <( grep -v '^#' ../quality-measures/qualities-R1.0.res | sed 's/^/R1.0-/g') \" "

mtruth(pt)=merge(pt).' | awk "{if (\$2=='.pt.'){print \$1,\$9,\$10,\$11}}"  | ../quality-measures/optimal-line'
mpu50 (pt)=merge(pt).' | awk "{if (\$2=='.pt.'){print \$1,\$12,\$13,\$14}}" | ../quality-measures/optimal-line'
matlas(pt)=merge(pt).' | awk "{if (\$2=='.pt.'){print \$1,\$15,\$16,\$17}}" | ../quality-measures/optimal-line'
mcms  (pt)=merge(pt).' | awk "{if (\$2=='.pt.'){print \$1,\$18,\$19,\$20}}" | ../quality-measures/optimal-line'

print mtruth('1000')
set macros

set key spacing 1.2

#------------------------------------------------------------------------
set title reflabel.' v. '.altlabel 
do for [pt in "500 1000 2000"]{
    set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
    plot mtruth(pt) i 0 u 2:3 w p pt 7 ps 0.5 lc 1 t 'all',\
         mtruth(pt).' | grep "^R1.0-D2_beta1_trim"' u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'ATLAS-like',\
         mtruth(pt).' | grep "^R0.8-N2_beta1_tpp"'  u 2:3 w p pt  5 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
         mtruth(pt) i 1 u 2:3 w l lc 3 lw 3 t 'optimal'         
}


# set title 'Pileup({/Symbol m}=40) v. no pileup'
# do for [pt in "500"]{
#     set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
#     plot mpu50(pt) i 0 u 2:3 w p pt 7 ps 0.5 lc 1 t 'all',\
#          mpu50(pt).' | grep "^R1.0-D2_beta1_trim"' u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'ATLAS-like',\
#          mpu50(pt).' | grep "^R0.8-N2_beta1_tpp"'  u 2:3 w p pt  5 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
#          mpu50(pt) i 1 u 2:3 w l lc 3 lw 3 t 'optimal'         
# }
# 
# 
# set title 'ATLAS-like v. particle'
# do for [pt in "500"]{
#     set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
#     plot matlas(pt) i 0 u 2:3 w p pt 7 ps 0.5 lc 1 t 'all',\
#          matlas(pt).' | grep "^R1.0-D2_beta1_trim"' u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'ATLAS-like',\
#          matlas(pt).' | grep "^R0.8-N2_beta1_tpp"'  u 2:3 w p pt  5 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
#          matlas(pt) i 1 u 2:3 w l lc 3 lw 3 t 'optimal'         
# }
# 
# set title 'CMS-like v. particle'
# do for [pt in "500"]{
#     set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
#     plot mcms(pt) i 0 u 2:3 w p pt 7 ps 0.5 lc 1 t 'all',\
#          mcms(pt).' | grep "^R1.0-D2_beta1_trim"' u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'ATLAS-like',\
#          mcms(pt).' | grep "^R0.8-N2_beta1_tpp"'  u 2:3 w p pt  5 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
#          mcms(pt) i 1 u 2:3 w l lc 3 lw 3 t 'optimal'         
# }

#------------------------------------------------------------------------
set title reflabel.' v. '.altlabel 
do for [pt in "500 1000 2000"]{
    set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
    plot mtruth(pt) i 0 u 2:3 w p pt 7 ps 0.7 lc 1 not,\
         mtruth(pt) i 1 u 2:3 w l lc 3 lw 3 not,\
         mtruth(pt).' | grep "^R1.0-D2_beta1_trim"' u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'ATLAS-like',\
         mtruth(pt).' | grep "^R0.8-N2_beta1_tpp"'  u 2:3 w p pt  5 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
         mtruth(pt).' | grep -v "^no" | sed "s/_/./g" ' i 1 u 2:3:1 w labels not rotate by 80 left offset 0.25,0.4 font ",8"
}

# set title 'Pileup({/Symbol m}=40) v. no pileup'
# do for [pt in "500"]{
#     set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
#     plot mpu50(pt) i 0 u 2:3 w p pt 7 ps 0.7 lc 1 not,\
#          mpu50(pt) i 1 u 2:3 w l lc 3 lw 3 not,\
#          mpu50(pt).' | grep -v "^no" | sed "s/_/./g" ' i 1 u 2:3:1 w labels not rotate by 60 left offset 0.25,0.4 font ",8"
# }
# 
# set title 'ATLAS-like v. particle'
# do for [pt in "500"]{
#     set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
#     plot matlas(pt) i 0 u 2:3 w p pt 7 ps 0.7 lc 1 not,\
#          matlas(pt) i 1 u 2:3 w l lc 3 lw 3 not,\
#          matlas(pt).' | grep -v "^no" | sed "s/_/./g" ' i 1 u 2:3:1 w labels not rotate by 60 left offset 0.25,0.4 font ",8"
# }
# 
# set title 'CMS-like v. particle'
# do for [pt in "500"]{
#     set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
#     plot mcms(pt) i 0 u 2:3 w p pt 7 ps 0.7 lc 1 not,\
#          mcms(pt) i 1 u 2:3 w l lc 3 lw 3 not,\
#          mcms(pt).' | grep -v "^no" | sed "s/_/./g" ' i 1 u 2:3:1 w labels not rotate by 60 left offset 0.25,0.4 font ",8"
# }

set out
