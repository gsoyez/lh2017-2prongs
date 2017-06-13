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
set xrange [0:5]

set ylabel 'performance'
set yrange [0:7]

set grid dt 3 lw 1
set key spacing 1.5
set macros

groomers="ppp lll lpl lpp ttt tlt tpt tll tpl tpp trimmed"
groomer_labels='"plain{/Symbol \304}plain/plain" "loose{/Symbol \304}loose/loose" "loose{/Symbol \304}plain/loose" "loose{/Symbol \304}plain/plain" "tight{/Symbol \304}tight/tight" "tight{/Symbol \304}loose/tight" "tight{/Symbol \304}plain/tight" "tight{/Symbol \304}loose/loose" "tight{/Symbol \304}plain/loose" "tight{/Symbol \304}plain/plain" "trimmed"'

shapes="D2_beta1 D2_beta2 N2_beta1 N2_beta2 tau21_beta1 tau21_beta2 M2_beta1 M2_beta2"
shape_labels='"D@_2^{(1)}" "D@_2^{(2)}" "N@_2^{(1)}" "N@_2^{(2)}" "{/Symbol t}@_{21}^{(1)}" "{/Symbol t}@_{21}^{(2)}" "M@_2^{(1)}" "M@_2^{(2)}"'

resilience ="1.0/sqrt(column('Bcorrection_'.level)**2+column('Scorrection_'.level)**2)"
performance="column('significance_'.levelref)"


set style line 1 dt 1 lc rgb "#000000" lw 2 pt 12 # D2
set style line 2 dt 2 lc rgb "#000000" lw 2 pt 13 # D2
set style line 3 dt 1 lc rgb "#00cc00" lw 2 pt 4  # N2
set style line 4 dt 2 lc rgb "#00cc00" lw 2 pt 5  # N2
set style line 5 dt 1 lc rgb "#0000ff" lw 2 pt 6  # tau21
set style line 6 dt 2 lc rgb "#0000ff" lw 2 pt 7  # tau21
set style line 7 dt 1 lc rgb "#ff0000" lw 2 pt 8  # M2
set style line 8 dt 2 lc rgb "#ff0000" lw 2 pt 9  # M2

do for [ R in "0.8 1.0"]{
    set out 'shape-scan-R'.R.'.pdf'

    set label 102 '{/*0.8 Pythia8(4C), anti-k_t('.R.')}' left at graph 0.03,0.05
    set label 103 '{/*0.8 60<m<100}'                     left at graph 0.03,0.10
    set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)  left at graph 0.03,0.15

    m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.res',shape)

    set title reflabel.' v. '.altlabel 

    # produce one plot for each grooming level
    do for [igroomer=1:words(groomers)]{
        groomer=word(groomers,igroomer)
        set title '{/*1.4 '.word(groomer_labels,igroomer).'} ('.reflabel.' v. '.altlabel.')'
        plot for [ishape=1:words(shapes)] m(word(shapes,ishape).'_'.groomer) u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp ls ishape ps variable t word(shape_labels,ishape) 
    }
}
     
set out
