reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'default-perf.pdf'

call 'defaults.gp'

eS=0.4

R=default_R
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

set label 102 '{/*0.8 Pythia8(4C), anti-k_t('.R.')}' left at graph 0.03,0.05
set label 103 '{/*0.8 60<m<100}'                     left at graph 0.03,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)  left at graph 0.03,0.15

m0(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep "%s %s" ../quality-measures/qualities-R'.R.'.res',shape,pt)

m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.res',shape)

resilience ="1.0/sqrt(column('Bcorrection_'.level)**2+column('Scorrection_'.level)**2)"
performance="column('significance_'.levelref)"

set macros
set title reflabel.' v. '.altlabel 

set arrow 1 from 2.5,0.5 to 4.5,0.5   filled lc 7 lw 1
set arrow 2 from 0.35,3.5 to 0.35,6.3 filled lc 7 lw 1
set arrow 3 from 3.0,4.22 to 4.0,5.6  filled lc 7 lw 1
set label 11 'more resilient' center at 3.5,0.75
set label 12 'more'           left   at 0.65,5.85
set label 22 'performant'     left   at 0.45,5.55
set label 13 'better'         center at 3.5,5.2 rotate by 43

plot m0(atlas_tag) u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc 7             lw 3 pt 6 ps variable t 'ATLAS',\
     m0(cms_tag)   u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc rgb '#00cc00' lw 3 pt 5 ps variable t 'CMS' 

set label 51 '{/*0.85  500}' at 2.15,2.5
set label 52 '{/*0.85 1000}' at 1.75,3.15
set label 53 '{/*0.85 2000}' at 0.9,5.0
set label 54 '{/*0.85  500}' at 0.95,3.5
set label 55 '{/*0.85 1000}' at 0.95,4.15

plot m(atlas_tag) u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc 7             lw 3 pt 6 ps variable t 'ATLAS',\
     m(cms_tag)   u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc rgb '#00cc00' lw 3 pt 5 ps variable t 'CMS' 
     
set out
