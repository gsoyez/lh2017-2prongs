reset
set term pdfcairo enhanced
set colors classic
set out 'perf.pdf'

set label 1 '{/Symbol e}_S=0.4' at graph 0.03,0.95
eS=0.4

pt='1000'
R='0.8'

set xlabel 'background robustness'
set xrange [0.4:1.4]

set ylabel 'signal significance'
set yrange [0:7]

set grid
set arrow 1 from 1.0,0.0 to 1.0,7.0 nohead dt 1 lw 2 lc 7

set label 102 '{/=10 Pythia8(4C), anti-k_t('.R.')}'  right at graph 0.97,0.60
set label 103 '{/=10 60<m_{tight}<100}'              right at graph 0.97,0.54
set label 101 sprintf('{/=10 {/Symbol e}_S=%g',eS)   right at graph 0.97,0.48

set style line 1 dt 1 lc 7 lw 2             pt  6 ps 0.6 #  tight  loose  tight
set style line 2 dt 1 lc rgb "#00dd00" lw 2 pt  4 ps 0.6 #  loose  loose  loose
set style line 3 dt 1 lc 4 lw 2             pt  2 ps 0.6 #  tight  tight  tight
set style line 4 dt 1 lc 1 lw 2             pt  8 ps 0.6 #  trim   trim   trim 
set style line 5 dt 1 lc 3 lw 2             pt 12 ps 0.6 #  tight  plain  plain

set style line 11 dt 2 lc 7 lw 2             pt  7 ps 0.6 #  tight  loose  tight
set style line 12 dt 2 lc rgb "#00dd00" lw 2 pt  5 ps 0.6 #  loose  loose  loose
set style line 13 dt 2 lc 4 lw 2             pt  1 ps 0.6 #  tight  tight  tight
set style line 14 dt 2 lc 1 lw 2             pt  9 ps 0.6 #  trim   trim   trim 
set style line 15 dt 2 lc 3 lw 2             pt 13 ps 0.6 #  tight  plain  plain


# list of grooming combinations:
#        mass   numer  denom
#        tight  loose  tight
#        loose  loose  loose
#        tight  tight  tight
#        trim   trim   trim
#        tight  plain  plain
grm_levels="tlt lll ttt trimmed tpp"
labs='"tight{/Symbol \304}loose/tight" "loose{/Symbol \304}loose/loose" "tight{/Symbol \304}tight/tight" "trim{/Symbol \304}trim/trim" "tight{/Symbol \304}plain/plain"'

shapes="tau21 N2 D2 M2"
slabs='"{/Symbol t}_{21}" "N_2" "D_2" "M_2"'

m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.res',shape)

set macros


do for [level in "hadron_parton truth_hadron"]{
    print "========================================================================"
    print level
    print "========================================================================"

    set label 11 level[:strstrt(level,'_')-1].' v. '.level[strstrt(level,'_')+1:] right at graph 0.95,0.37
    
    print 'shape dependence'
    do for [beta in "2 1"]{
        set label 1 'Solid: N@_2^{('.beta.')}'               at graph 0.03,0.93
        set label 2 'Dashed: {/Symbol t}@_{21}^{('.beta.')}' at graph 0.03,0.85

        plot for [i=1:words(grm_levels)] m('N2_beta'   .beta.'_'.word(grm_levels,i)) u (column('Brob_'.level)):(column('sig_'.level)) w linesp ls    i t word(labs,i),\
             for [i=1:words(grm_levels)] m('tau21_beta'.beta.'_'.word(grm_levels,i)) u (column('Brob_'.level)):(column('sig_'.level)) w linesp ls 10+i not
    }

    print 'beta dependence'
    
    do for [ishape=1:words(shapes)]{
        shape=word(shapes,ishape)
        lshape=word(slabs,ishape)
        set label 1 'Solid: '.lshape.' {/Symbol b}=2'  at graph 0.03,0.93
        set label 2 'Dashed: '.lshape.' {/Symbol b}=1' at graph 0.03,0.85

        plot for [i=1:words(grm_levels)] m(shape.'_beta2_'.word(grm_levels,i)) u (column('Brob_'.level)):(column('sig_'.level)) w linesp ls    i t word(labs,i),\
             for [i=1:words(grm_levels)] m(shape.'_beta1_'.word(grm_levels,i)) u (column('Brob_'.level)):(column('sig_'.level)) w linesp ls 10+i not
    }
}

set out
