reset
set term pdfcairo enhanced
set colors classic
set out 'shape-distribs.pdf'

set label 1 'N_2' at graph 0.03,0.95
base='N2'
vmin=0.0
vmax=1.0
dv=0.02

pt='500'
R='1.0'


set xlabel 'v'
set xrange [vmin:vmax]

set label 101 '{/=10 p_{t,jet}>'.pt.' GeV}'         right at graph 0.97,0.6
set label 102 '{/=10 Pythia8(4C), anti-k_t('.R.')}' right at graph 0.97,0.54
set label 103 '{/=10 60<m_{tight}<100}'             right at graph 0.97,0.48

set style line 1 dt 1 lc 1 lw 2  # plain
set style line 2 dt 1 lc 3 lw 2  # loose
set style line 3 dt 1 lc 7 lw 2  # tight
set style line 4 dt 1 lc rgb "#00dd00" lw 2 
set style line 5 dt 1 lc 4lw 2 

set style line 11 dt 2 lc 1 lw 2  # plain
set style line 12 dt 2 lc 3 lw 2  # loose
set style line 13 dt 2 lc 7 lw 2  # tight
set style line 14 dt 2 lc rgb "#00dd00" lw 2 # trim
set style line 15 dt 2 lc 4 lw 2 # trim

shapes=base.'_beta2_pt '.base.'_beta2_lt '.base.'_beta2_tt '.base.'_beta2_ll '.base.'_beta2_pp'
labs="pt lt tt ll pp"
m(proc,level,shape)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s-pythia84C-%s-R'.R.'.ntuple.gz "%s" %g %g %g "mass_tight>60.0" "mass_tight<100.0"',proc,level,shape,vmin,vmax,dv)

do for [proc in "WW dijets"]{
    do for [level in "parton truth"]{
        set label 1 proc.', '.level right at graph 0.97,0.37
        plot for [i=1:words(shapes)] m(proc.pt,level,word(shapes,i)) u 2:4 w l ls i t word(labs,i)
    }
}

set out
