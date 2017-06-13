# plot of the mass distribution at detector level
#
# We do it for one detecror model at a time and one type of grooming
# at a time (and overimpose different pt's

reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'mass-width-detector.pdf'

call 'defaults.gp'

R=default_R

set xlabel 'jet p_t [GeV]'
set xrange [400:2500]
set log x
set xtics add ( "500"  500)
set xtics add ("2000" 2000)


set grid dt 3 lw 1

set label 102 '{/*0.8 Pythia8(4C), anti-k_t('.R.')}' right at graph 0.97,0.1

set key spacing 1.3

set style line 1 dt 2 lc rgb "#ff0000" lw 2
set style line 2 dt 1 lc rgb "#ff0000" lw 2
set style line 3 dt 2 lc rgb "#00dd00" lw 2
set style line 4 dt 1 lc rgb "#00dd00" lw 2
set style line 5 dt 2 lc rgb "#0000ff" lw 2
set style line 6 dt 1 lc rgb "#0000ff" lw 2
set style line 7 dt 2 lc rgb "#000000" lw 2
set style line 8 dt 1 lc rgb "#000000" lw 2

m(lev,mtype)=sprintf('< for pt in 500 1000 2000; do echo -n "$pt "; ../scripts/histogram.sh ../res/lhc14-WW${pt}-pythia84C-%s-R%s.ntuple.gz m_%s 0.0 500 1.0 2>/dev/null | normalise.py | ../scripts/get-width.py -f 0.4; done',lev,R,mtype)


set key top left reverse Left

set multiplot layout 2,1
#------------------------------------------------------------------------
# ATLAS

set ylabel 'W mass (trim)'
set yrange [70:120]
plot m('ATLAS', 'trim') u ($1*1.04):3:2:4 w yerr dt 1 lc 7 lw 3 pt 13 t 'ATLAS-like',\
     m('pu50',  'trim') u ($1*1.00):3:2:4 w yerr dt 1 lc 1 lw 3 pt  8 t 'PU({/Symbol m}=40',\
     m('truth', 'trim') u ($1/1.04):3:2:4 w yerr dt 1 lc 3 lw 3 pt  2 t 'truth',\
     80.4 w l dt 2 lc 7 not


#------------------------------------------------------------------------
# CMS

set ylabel 'W mass (tight)'
set yrange [70:120]
plot m('CMS',   'tight') u ($1*1.04):3:2:4 w yerr dt 1 lc rgb "#00cc00" lw 3 pt 5 t 'CMS-like',\
     m('pu50',  'tight') u ($1*1.00):3:2:4 w yerr dt 1 lc 1             lw 3 pt 8 t 'PU({/Symbol m}=40',\
     m('truth', 'tight') u ($1/1.04):3:2:4 w yerr dt 1 lc 3             lw 3 pt 2 t 'truth',\
     80.4 w l dt 2 lc 7 not



unset multiplot


set out
