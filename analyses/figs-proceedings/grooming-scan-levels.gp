reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'grooming-scan-levels.pdf'

call 'defaults.gp'


eS=default_Sref
mmin=default_mmin
mmax=default_mmax

R='1.0'
pt=default_pt

set xlabel 'resilience'
set xrange [0:10]
satx(x)=(x>9.9) ? 9.9 : x

set ylabel 'performance'
set yrange [0:7]

set grid dt 3 lw 1

set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'   left at graph 0.03,0.05
set label 103 sprintf('{/*0.8 %g<m<%g GeV}',mmin,mmax) left at graph 0.03,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)    left at graph 0.03,0.15

m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.new | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.new',shape)

set macros


groomers="trimmed tpp tlt lll ttt"
groomer_labels='"trimmed" "tight{/Symbol \304}plain/plain" "tight{/Symbol \304}loose/tight" "loose{/Symbol \304}loose/loose" "tight{/Symbol \304}tight/tight"'

shapes="tau21_beta1 tau21_beta2 N2_beta1 N2_beta2 D2_beta1 D2_beta2 M2_beta1 M2_beta2"
shape_labels='"{/Symbol t}@_{21}^{(1)}" "{/Symbol t}@_{21}^{(2)}" "N@_2^{(1)}" "N@_2^{(2)}" "D@_2^{(1)}" "D@_2^{(2)}" "M@_2^{(1)}" "M@_2^{(2)}"'

set style line 1 dt 1 lc rgb "#000000" lw 2 pt 5
set style line 2 dt 1 lc rgb "#00dd00" lw 2 pt 4
set style line 3 dt 1 lc rgb "#0000ff" lw 2 pt 7
set style line 4 dt 1 lc rgb "#ff0000" lw 2 pt 6
set style line 5 dt 1 lc rgb "#00dddd" lw 2 pt 9

set key spacing 1.5


# produce one plot for each grooming level
do for [ishape=1:words(shapes)]{
    shape=word(shapes,ishape)

    print shape
    
    do for [level in "hadron_parton truth_hadron truth_parton truth_pu50 truth_ATLAS truth_CMS"]{
    #do for [level in "hadron_parton"]{

        levelref=level[:strstrt(level,'_')-1]
        levelalt=level[strstrt(level,'_')+1:]
        levelaltref=levelalt.'-'.levelref

        #print "  ".levelref.' v. '.levelalt
        
        set title '{/*1.4 '.word(shape_labels,ishape).'} ('.levelref.' v. '.levelalt.')'
        plot for [igroomer=1:words(groomers)] m(shape.'_'.word(groomers,igroomer)) u (satx(resilience(default_Sref, column('epsilon_B_'.levelref), column('epsilon_S_'.levelaltref), column('epsilon_B_'.levelaltref)))):(performance(default_Sref, column('epsilon_B_'.levelref), column('epsilon_S_'.levelaltref), column('epsilon_B_'.levelaltref))):((0.001*$2)**0.5*0.6) w linesp ls igroomer ps variable t word(groomer_labels,igroomer)
    }
}
     
set out
