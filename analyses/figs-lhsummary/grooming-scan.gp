reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic

call 'defaults.gp'

eS=0.4

pt=default_pt
levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel 'resilience'
set xrange [0:7]

set ylabel 'performance'
set yrange [0:7]

set grid dt 3 lw 1


resilience ="1.0/sqrt(column('Bcorrection_'.level)**2+column('Scorrection_'.level)**2)"
performance="column('significance_'.levelref)"

set macros


groomers="trimmed tpp ttt tlt lll"
#groomer_labels='"trimmed" "tight{/Symbol \304}plain/plain" "tight{/Symbol \304}tight/tight" "tight{/Symbol \304}loose/tight" "loose{/Symbol \304}loose/loose"'
groomer_labels='"ATLAS-like" "CMS-like" "all tight" "dichroic" "all loose"'
Rs=      "1.0 0.8 1.0 1.0 0.8"
# R assignment: everything that involves plain is 0.8
# lll is 0.8

shapes="tau21_beta1 tau21_beta2 N2_beta1 N2_beta2 D2_beta1 D2_beta2 M2_beta1 M2_beta2"
shape_labels='"{/Symbol t}@_{21}^{(1)}" "{/Symbol t}@_{21}^{(2)}" "N@_2^{(1)}" "N@_2^{(2)}" "D@_2^{(1)}" "D@_2^{(2)}" "M@_2^{(1)}" "M@_2^{(2)}"'

set style line 1 dt 1 lc rgb "#000000" lw 2 pt 13
set style line 2 dt 1 lc rgb "#00cc00" lw 2 pt 5
set style line 3 dt 1 lc rgb "#00dddd" lw 2 pt 9
set style line 4 dt 1 lc rgb "#0000ff" lw 2 pt 7
set style line 5 dt 1 lc rgb "#ff0000" lw 2 pt 3

set key spacing 1.5

set out 'grooming-scan.pdf'

set label 102 '{/*0.8 Pythia8(4C), anti-k_t}'        left at graph 0.03,0.05
set label 103 '{/*0.8 60<m<100}'                     left at graph 0.03,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)  left at graph 0.03,0.15

m(shape,Rv)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.Rv.'.res | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.Rv.'.res',shape)

# produce one plot for each grooming level
do for [ishape=1:words(shapes)]{
    shape=word(shapes,ishape)
    set title '{/*1.4 '.word(shape_labels,ishape).'} ('.reflabel.' v. '.altlabel.')'
    plot for [igroomer=1:words(groomers)] m(shape.'_'.word(groomers,igroomer),word(Rs,igroomer)) u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp ls igroomer ps variable t word(groomer_labels,igroomer).'{/*0.6  (R='.word(Rs,igroomer).')}'
}
     
set out
