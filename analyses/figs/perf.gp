reset
set term pdfcairo enhanced
set colors classic
set out 'perf.pdf'

set label 1 '{/Symbol e}_S=0.4' at graph 0.03,0.95
eS=0.4

pt='1000'

set xlabel 'background robustness'
set xrange [0.5:1.5]

set ylabel 'signal efficiency'
set yrange [0:7]

set grid
set arrow 1 from 1.0,0.0 to 1.0,7.0 nohead dt 1 lw 2 lc 7

set label 102 '{/=10 Pythia8(4C), anti-k_t(1)}'    right at graph 0.97,0.60
set label 103 '{/=10 60<m_{tight}<100}'            right at graph 0.97,0.54
set label 101 sprintf('{/=10 {/Symbol e}_S=%g',eS) right at graph 0.97,0.48

set style line 1 dt 1 lc 1 lw 2             pt  1 ps 0.5 # plain-tight
set style line 2 dt 1 lc 3 lw 2             pt  4 ps 0.5 # loose-tight
set style line 3 dt 1 lc 7 lw 2             pt  6 ps 0.5 # tight-tight
set style line 4 dt 1 lc rgb "#00dd00" lw 2 pt  8 ps 0.5 # loose-loose
set style line 5 dt 1 lc 4 lw 2             pt 10 ps 0.5 # trim

set style line 11 dt 2 lc 1 lw 2             pt  2 ps 0.5 # plain-tight
set style line 12 dt 2 lc 3 lw 2             pt  5 ps 0.5 # loose-tight
set style line 13 dt 2 lc 7 lw 2             pt  7 ps 0.5 # tight-tight
set style line 14 dt 2 lc rgb "#00dd00" lw 2 pt  9 ps 0.5 # loose-loose
set style line 15 dt 2 lc 4 lw 2             pt 11 ps 0.5 # trim

shapes="pt lt tt ll"
labs='"plain/tight" "loose/tight" "tight/tight" "loose/loose"'

m(shape,levelref,levelalt)=sprintf('< for pt in 500 1000 2000; do ../scripts/get_perf_robustness.sh ../res/WW${pt}-pythia84C-%s.ntuple.gz ../res/dijets${pt}-pythia84C-%s.ntuple.gz ../res/WW${pt}-pythia84C-%s.ntuple.gz ../res/dijets${pt}-pythia84C-%s.ntuple.gz "%s" %g "mass_tight>60.0" "mass_tight<100.0"; done',levelref,levelref,levelalt,levelalt,shape,eS)


do for [level in "parton hadron"]{
    print "========================================================================"
    print level.' v. truth'
    print "========================================================================"

    set label 11 level.' v. truth' right at graph 0.95,0.37
    
    print "------------------------------------------------------------------------"
    print 'shape dependence'
    print "------------------------------------------------------------------------"
    do for [beta in "2 1"]{
        set label 1 'Solid: N@_2^{('.beta.')}'               at graph 0.03,0.93
        set label 2 'Dashed: {/Symbol t}@_{21}^{('.beta.')}' at graph 0.03,0.85
        
        plot for [i=1:words(shapes)] m('N2_beta'   .beta.'_'.word(shapes,i),'truth',level) u 2:1 w linesp ls    i t word(labs,i),\
             for [i=1:words(shapes)] m('tau21_beta'.beta.'_'.word(shapes,i),'truth',level) u 2:1 w linesp ls 10+i not
    }

    print "------------------------------------------------------------------------"
    print 'beta dependence'
    print "------------------------------------------------------------------------"
    do for [base in "N2 tau21"]{
        set label 1 'Solid: {/Symbol b}=2'  at graph 0.03,0.93
        set label 2 'Dashed: {/Symbol b}=1' at graph 0.03,0.85

        plot for [i=1:words(shapes)] m(base.'_beta2_'.word(shapes,i),'truth',level) u 2:1 w linesp ls    i t word(labs,i),\
             for [i=1:words(shapes)] m(base.'_beta1_'.word(shapes,i),'truth',level) u 2:1 w linesp ls 10+i not
    }
}

set out
