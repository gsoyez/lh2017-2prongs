reset
set term pdfcairo enhanced
set colors classic
set out 'mass-distribs.pdf'

mmin=0.0
mmax=150.0
dm=2.0
set xlabel 'm [GeV]'
set xrange [mmin:mmax]

pt='1000'

groomers="plain loose tight trim"

set label 101 '{/=10 p_{t,jet}>'.pt.' GeV}'     right at graph 0.97,0.6
set label 102 '{/=10 Pythia8(4C), anti-k_t(1)}' right at graph 0.97,0.54

set style line 1 dt 1 lc 1 lw 2  # plain
set style line 2 dt 1 lc 3 lw 2  # loose
set style line 3 dt 1 lc 7 lw 2  # tight
set style line 4 dt 1 lc rgb "#00dd00" lw 2 # trim

set style line 11 dt 2 lc 1 lw 2  # plain
set style line 12 dt 2 lc 3 lw 2  # loose
set style line 13 dt 2 lc 7 lw 2  # tight
set style line 14 dt 2 lc rgb "#00dd00" lw 2 # trim


m(proc,level,groom)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s-pythia84C-%s.ntuple.gz "mass_%s" %g %g %g',proc,level,groom,mmin,mmax,dm)


do for [proc in "WW dijets"]{
    do for [level in "parton truth"]{
        set label 1 proc.', '.level right at graph 0.97,0.45
        plot for [igroom=1:words(groomers)] m(proc.pt,level,word(groomers,igroom)) u 2:4 w l ls igroom    t word(groomers,igroom)
    }
}

set out
