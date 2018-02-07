# just do the scans we include in the proceedings

reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'shape-scan.pdf'

call 'defaults.gp'


eS=default_Sref
mmin=default_mmin
mmax=default_mmax

R=default_R
pt=default_pt

set xlabel 'resilience'
set xrange [0:10]
satx(x)=(x>9.9) ? 9.9 : x

set ylabel 'performance'
set yrange [0:7.2]

set grid dt 3 lw 1

set label 102 '{/*0.8 Pythia8(M13), anti-k_t(R='.R.')}' left at graph 0.03,0.05
set label 103 sprintf('{/*0.8 %g<m<%g GeV}',mmin,mmax)  left at graph 0.03,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)     left at graph 0.03,0.15

m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.new | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.new',shape)

set macros


#groomers="ppp lpp lpl lll tpp tpl tll tpt tlt ttt trimmed"
#groomer_labels='"plain{/Symbol \304}plain/plain" "loose{/Symbol \304}plain/plain" "loose{/Symbol \304}plain/loose" "loose{/Symbol \304}loose/loose{/*0.7 (loose)}" "tight{/Symbol \304}plain/plain{/*0.7 (old-CMS-like)}" "tight{/Symbol \304}plain/loose" "tight{/Symbol \304}loose/loose" "tight{/Symbol \304}plain/tight" "tight{/Symbol \304}loose/tight{/*0.8 (dichroic)}" "tight{/Symbol \304}tight/tight{/*0.8 (CMS-like)}""trimmed {/*0.7 (ATLAS-like)}"'

groomers="tlt ttt trimmed"
groomer_labels='"tight{/Symbol \304}loose/tight{/*0.8 (LHDT)}" "tight{/Symbol \304}tight/tight{/*0.8 (CMS-like)}""trimmed {/*0.7 (ATLAS-like)}"'

shapes="M2_beta1 tau21_beta1 N2_beta1 D2_beta1 D2_beta2"
shape_labels='"M@_2^{(1)}" "{/Symbol t}@_{21}^{(1)}" "N@_2^{(1)} {/*0.8 (CMS-like)}" "D@_2^{(1)} {/*0.8 (ATLAS-like)}" "D@_2^{(2)} {/*0.8 (LHDT)}"'

set style line 1 dt (12,6,4,6) lc rgb "#00dddd" pt 13  # M2(1)
set style line 2 dt (4,3)      lc rgb "#00dd00" pt 11  # tau21(1)
set style line 3 dt (12,6)     lc rgb "#ff0000" pt 9   # N2(1)
set style line 4 dt (8,6)      lc rgb "#000000" pt 5   # D2(1)
set style line 5 dt 1          lc rgb "#0000ff" pt 7   # D2(2)

set key spacing 1.2

# have wider lines for the reference points
custom_lw(igroomer,ishape)=(((ishape==4) && (igroomer==3))) ? 4 : ((((ishape==3) && (igroomer==2))) ? 4 : ((((ishape==5)*(igroomer==1))) ? 4 : 2))

# produce one plot for each grooming level
do for [igroomer=1:words(groomers)]{
    groomer=word(groomers,igroomer)

    print groomer
    
    do for [level in "hadron_parton truth_hadron"]{
        if (level eq "truth_hadron"){
            set key top left reverse Left
        } else {
            if(level eq "truth_pu50"){
                set key top left reverse Left
            } else {
                set key top right noreverse Right
            }
        }
        
        levelref=level[0:strstrt(level,'_')-1]
        levelalt=level[strstrt(level,'_')+1:-1]
        levelaltref=levelalt.'-'.levelref

        set title '{/*1.4 '.word(groomer_labels,igroomer).'}: '.levelalt.' {/Symbol \256} '.levelref
        plot for [ishape=1:words(shapes)] m(word(shapes,ishape).'_'.groomer) u (satx(resilience(default_Sref, column('epsilon_B_'.levelref), column('epsilon_S_'.levelaltref), column('epsilon_B_'.levelaltref)))):(performance(default_Sref, column('epsilon_B_'.levelref), column('epsilon_S_'.levelaltref), column('epsilon_B_'.levelaltref))):((0.001*$2)**0.5*0.6) w linesp ls ishape ps variable lw custom_lw(igroomer, ishape) t word(shape_labels,ishape)
    }
}
     
set out
