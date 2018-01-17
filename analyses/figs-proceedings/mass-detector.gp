# plot of the mass distribution at detector level
#
# We do it for one detecror model at a time and one type of grooming
# at a time (and overimpose different pt's

reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'mass-detector.pdf'

call 'defaults.gp'

R=default_R

set xlabel 'reconstructed m_W [GeV]'
set xrange [0:200]

set ylabel 'dN/dm_W'

set grid dt 3 lw 1

set key spacing 1.3

set style line 1 dt 2 lc rgb "#ff0000" lw 2
set style line 2 dt 1 lc rgb "#ff0000" lw 2
set style line 3 dt 2 lc rgb "#00dd00" lw 2
set style line 4 dt 1 lc rgb "#00dd00" lw 2
set style line 5 dt 2 lc rgb "#0000ff" lw 2
set style line 6 dt 1 lc rgb "#0000ff" lw 2
set style line 7 dt 2 lc rgb "#000000" lw 2
set style line 8 dt 1 lc rgb "#000000" lw 2

m(lev,pt,mtype)=sprintf('< ../scripts/histogram.sh ../res/lhc14-WW%s-pythia8230M13-%s-R%s.ntuple.gz m_%s 10.0 200 1.0',pt,lev,R,mtype)

set label 66 '{/*2.5 UNCALIBRATED}' center at graph 0.5,0.5 rotate by 45 tc rgb "#cccccc" back

#------------------------------------------------------------------------
# ATLAS v CMS
set title '{/*1.4 ATLAS-like(trimmed) v. CMS-like (tight)}'

set label 1 'solid: ATLAS-like' right at graph 0.95,0.5  front
set label 2 'dashed: CMS-like'  right at graph 0.95,0.42 front

set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}' right at graph 0.97,0.58

plot m('ATLAS',  '500', 'trim' ) u 2:4 w l dt 1     lc 1 lw 3 t 'p_t= 500 GeV',\
     m('ATLAS', '1000', 'trim' ) u 2:4 w l dt 1     lc 3 lw 3 t 'p_t=1000 GeV',\
     m('ATLAS', '2000', 'trim' ) u 2:4 w l dt 1     lc 7 lw 3 t 'p_t=2000 GeV',\
     m('CMS',    '500', 'tight') u 2:4 w l dt (7,4) lc 1 lw 3 not,\
     m('CMS',   '1000', 'tight') u 2:4 w l dt (7,4) lc 3 lw 3 not,\
     m('CMS',   '2000', 'tight') u 2:4 w l dt (7,4) lc 7 lw 3 not


#------------------------------------------------------------------------
# ATLAS
set title '{/*1.4 ATLAS-like}'

set label 1 'solid: ATLAS-like' right at graph 0.95,0.5
set label 2 'dashed: particle'  right at graph 0.95,0.42

set label 102 '{/*0.8 Pythia8(M13), anti-k_t(1)}' right at graph 0.97,0.58

plot m('truth',  '500', 'trim') u 2:4 w l dt (7,4) lc 1 lw 3 not,\
     m('truth', '1000', 'trim') u 2:4 w l dt (7,4) lc 3 lw 3 not,\
     m('truth', '2000', 'trim') u 2:4 w l dt (7,4) lc 7 lw 3 not,\
     m('ATLAS',  '500', 'trim') u 2:4 w l dt 1     lc 1 lw 3 t 'p_t= 500 GeV',\
     m('ATLAS', '1000', 'trim') u 2:4 w l dt 1     lc 3 lw 3 t 'p_t=1000 GeV',\
     m('ATLAS', '2000', 'trim') u 2:4 w l dt 1     lc 7 lw 3 t 'p_t=2000 GeV'

#------------------------------------------------------------------------
# CMS
set title '{/*1.4 CMS-like}'

set label 1 'solid: CMS'       right at graph 0.95,0.5
set label 2 'dashed: particle' right at graph 0.95,0.42

set label 102 '{/*0.8 Pythia8(M13), anti-k_t(0.8)}' right at graph 0.97,0.58

plot m('truth',  '500', 'tight') u 2:4 w l dt (7,4) lc 1 lw 3 not,\
     m('truth', '1000', 'tight') u 2:4 w l dt (7,4) lc 3 lw 3 not,\
     m('truth', '2000', 'tight') u 2:4 w l dt (7,4) lc 7 lw 3 not,\
     m('CMS',    '500', 'tight') u 2:4 w l dt 1     lc 1 lw 3 t 'p_t= 500 GeV',\
     m('CMS',   '1000', 'tight') u 2:4 w l dt 1     lc 3 lw 3 t 'p_t=1000 GeV',\
     m('CMS',   '2000', 'tight') u 2:4 w l dt 1     lc 7 lw 3 t 'p_t=2000 GeV'



set out
