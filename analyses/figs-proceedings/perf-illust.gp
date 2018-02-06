reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic

call 'defaults.gp'

eS=0.4

pt=default_pt
levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt
levelaltref=levelalt.'-'.levelref

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel 'resilience'
set xrange [0:7]

set ylabel 'performance'
set yrange [0:7]

set key spacing 1.3
set grid dt 3 lw 1

set macros

R=default_R

m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.new | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.new',shape)


set out 'perf-illusr.pdf'  # renamed as 2prong_sweep_pt in the proceedings

set label 102 '{/*0.8 Pythia8(4C), anti-k_t(R='.R.')}' left at graph 0.03,0.05
set label 103 '{/*0.8 60<m<100}'                       left at graph 0.03,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)    left at graph 0.03,0.15

set arrow 1 from 3.5,0.5 to 6.3,0.5   filled lc 7 lw 1
set arrow 2 from 0.35,3.5 to 0.35,6.3 filled lc 7 lw 1
set arrow 3 from 3.7,3.7 to 5.3,5.3   filled lc 7 lw 1
set label 11 'more resilient'  center at 4.9,0.75
#set label 12 'more'           left   at 0.65,5.85
#set label 22 'performant'     left   at 0.45,5.52
set label 12 'more performant' center at 0.6,4.8 rotate by 90
set label 13 'better'          center at 4.65,4.35 rotate by 45

set label 51 '{/*0.85 p_t>500}' at 2.4,2.6
set label 52 '{/*0.85 1000}' at 2.2,3.2
set label 53 '{/*0.85 2000}' at 0.9,4.45
#set label 54 '{/*0.85  500}' at 0.7,3.5
#set label 55 '{/*0.85 1000}' at 0.6,4.15

perf="performance(".sprintf("%g",default_Sref).", column('epsilon_B_'.levelref), column('epsilon_S_'.levelaltref), column('epsilon_B_'.levelaltref))"
resi="resilience(".sprintf("%g",default_Sref).", column('epsilon_B_'.levelref), column('epsilon_S_'.levelaltref), column('epsilon_B_'.levelaltref))"

plot m(atlas_tag) u (@resi):(@perf):((0.001*$2)**0.5*0.6) w linesp dt 1 lc 7             lw 3 pt 5 ps variable t 'ATLAS-like',\
     m(cms_tag  ) u (@resi):(@perf):((0.001*$2)**0.5*0.6) w linesp dt 1 lc rgb '#00cc00' lw 3 pt 9 ps variable t 'CMS-like',\
     m(perf_tag ) u (@resi):(@perf):((0.001*$2)**0.5*0.6) w linesp dt 1 lc 3             lw 3 pt 7 ps variable t 'LHDD@_2^{(2)}'

unset for [i=51:55] label i

set out
