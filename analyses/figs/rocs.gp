reset
set term pdfcairo enhanced
set colors classic
set out 'rocs.pdf'

set label 1 'N_2' at graph 0.03,0.95
base='N2'
vmin=0.0
vmax=1.0
dv=0.02

pt='1000'
R='0.8'

set xlabel '{/Symbol e}_S'
set xrange [0:1]

set ylabel '{/Symbol e}_BB'
set yrange [0.0001:1]
set log y

set label 101 '{/=10 p_{t,jet}>'.pt.' GeV}'         right at graph 0.97,0.6
set label 102 '{/=10 Pythia8(4C), anti-k_t('.R.')}' right at graph 0.97,0.54
set label 103 '{/=10 60<m_{tight}<100}'             right at graph 0.97,0.48

set style line 1 dt 1 lc 1 lw 2  # plain
set style line 2 dt 1 lc 3 lw 2  # loose
set style line 3 dt 1 lc 7 lw 2  # tight
set style line 4 dt 1 lc rgb "#00dd00" lw 2 # trim
set style line 5 dt 1 lc 4lw 2 # trim

set style line 11 dt 2 lc 1 lw 2  # plain
set style line 12 dt 2 lc 3 lw 2  # loose
set style line 13 dt 2 lc 7 lw 2  # tight
set style line 14 dt 2 lc rgb "#00dd00" lw 2 # trim
set style line 15 dt 2 lc 4 lw 2 # trim

shapes=base.'_beta2_pt '.base.'_beta2_lt '.base.'_beta2_tt '.base.'_beta2_ll '.base.'_beta2_pp'
labs="pt lt tt ll pp"
m(pt,level,shape)=sprintf('< ../scripts/roc.sh ../res/lhc14-WW%s-pythia84C-%s-R'.R.'.ntuple.gz ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz "%s" %g %g %g "mass_tight>60.0" "mass_tight<100.0"',pt,level,pt,level,shape,vmin,vmax,dv)

print m('1000','parton','N2_beta2_lt')
do for [level in "parton truth"]{
    set label 1 level right at graph 0.95,0.37
    plot for [i=1:words(shapes)] m(pt,level,word(shapes,i)) u 1:2 w l ls i t word(labs,i)
}

set out
