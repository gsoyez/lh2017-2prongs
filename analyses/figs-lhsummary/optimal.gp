reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'optimal.pdf'

call 'defaults.gp'

eS=0.4

R=default_R
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

set label 102 '{/*0.8 Pythia8(4C), anti-k_t('.R.')}' right at graph 0.97,0.05
set label 103 '{/*0.8 60<m<100}'                     right at graph 0.97,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)  right at graph 0.97,0.15

#          +   x   *   sqr     o       tr     dia
groomers="ppp lll lpl lpp ttt tlt tpt tll tpl tpp trimmed"
#labs='"tight{/Symbol \304}loose/tight" "loose{/Symbol \304}loose/loose" "tight{/Symbol \304}tight/tight" "trim{/Symbol \304}trim/trim" "tight{/Symbol \304}plain/plain"'

shapes="tau21_beta1 tau21_beta2 N2_beta1 N2_beta2 D2_beta1 D2_beta2 M2_beta1 M2_beta2"
#slabs='"{/Symbol t}_{21}" "N_2" "D_2" "M_2"'

# let us hardcode the column numbers for truth v. parton:
m(pt)='< grep -v "^#" ../quality-measures/qualities-R'.R.'.res | awk "{if (\$2=='.pt.'){print \$1,\$9,\$10,\$11}}" | ../quality-measures/optimal-line'

resilience ="1.0/sqrt(column('Bcorrection_'.level)**2+column('Scorrection_'.level)**2)"
performance="column('significance_'.levelref)"

set macros

mycolour(i)=(i==0) ? 'rgb "#ff0000"' : ((i==1) ? 'rgb "#00dd00"' : ((i==2) ? 'rgb "#0000ff"' : 'rgb "#ffffff"'))
mydt(i)=(i-1)%2
mypoint(ig)=(ig<=9) ? ig : ig+2

set style line 1 dt 2 lc rgb "#ff0000" lw 2
set style line 2 dt 1 lc rgb "#ff0000" lw 2
set style line 3 dt 2 lc rgb "#000000" lw 2
set style line 4 dt 1 lc rgb "#000000" lw 2
set style line 5 dt 2 lc rgb "#0000ff" lw 2
set style line 6 dt 1 lc rgb "#0000ff" lw 2
set style line 7 dt 2 lc rgb "#00dd00" lw 2
set style line 8 dt 1 lc rgb "#00dd00" lw 2

set title reflabel.' v. '.altlabel 


set key spacing 1.2
do for [pt in "500 1000 2000"]{
    set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
    plot m(pt) i 0 u 2:3 w p pt 7 ps 0.5 lc 1 t 'all',\
         m(pt).' | grep "^D2_beta1_trim"' u 2:3 w p pt  9 ps 1.2 lc rgb "#00dd00" t 'ATLAS-like',\
         m(pt).' | grep "^N2_beta1_tpp"'  u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'CMS-like',\
         m(pt) i 1 u 2:3 w l lc 3 lw 3 t 'optimal'
         
}


do for [pt in "500 1000 2000"]{
    set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96
    plot m(pt) i 0 u 2:3 w p pt 7 ps 0.7 lc 1 not,\
         m(pt) i 1 u 2:3 w l lc 3 lw 3 not,\
         m(pt).' | grep -v "^no" | sed "s/_/./g" ' i 1 u 2:3:1 w labels not rotate by 60 left offset 0.25,0.4 font ",8"
         
}

set out
