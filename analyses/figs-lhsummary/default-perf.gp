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

set key spacing 1.3
set grid dt 3 lw 1

set macros
set title reflabel.' v. '.altlabel 

# x = (a-b)/b = a/b -1 => a/b = x+1
# ---> y = 2 (a-b)/(a+b) = 2x/(x+2)
resilience ="1.0/sqrt((2.0*column('Bcorrection_'.level)/(column('Bcorrection_'.level)+2.0))**2+(2*column('Scorrection_'.level)/(column('Scorrection_'.level)+2.0))**2)"
#resilience ="1.0/sqrt(column('Bcorrection_'.level)**2+column('Scorrection_'.level)**2)"
performance="column('significance_'.levelref)"

m0(shape,R)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep "%s %s" ../quality-measures/qualities-R'.R.'.res',shape,pt)
m(shape,R)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.res',shape)


set out 'default-perf.pdf'

set label 102 '{/*0.8 Pythia8(4C), anti-k_t}' left at graph 0.03,0.05
set label 103 '{/*0.8 60<m<100}'                     left at graph 0.03,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)  left at graph 0.03,0.15

set arrow 1 from 3.5,0.5 to 6.3,0.5   filled lc 7 lw 1
set arrow 2 from 0.35,3.5 to 0.35,6.3 filled lc 7 lw 1
set arrow 3 from 3.7,3.7 to 5.3,5.3   filled lc 7 lw 1
set label 11 'more resilient' center at 4.9,0.75
set label 12 'more'           left   at 0.65,5.85
set label 22 'performant'     left   at 0.45,5.52
set label 13 'better'         center at 4.65,4.35 rotate by 45

plot m0(atlas_tag,default_Ratlas) u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc 7             lw 3 pt 6 ps variable t 'ATLAS-like{/*0.6  (R='.default_Ratlas.')}',\
     m0(cms_tag,default_Rcms)     u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc rgb '#00cc00' lw 3 pt 5 ps variable t 'CMS-like{/*0.6  (R='.default_Rcms.')}' 

set label 51 '{/*0.85  500}' at 2.6,2.5
set label 52 '{/*0.85 1000}' at 2.05,3.15
set label 53 '{/*0.85 2000}' at 1.5,4.85
set label 54 '{/*0.85  500}' at 0.7,3.5
set label 55 '{/*0.85 1000}' at 0.6,4.15

plot m(atlas_tag,default_Ratlas) u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc 7             lw 3 pt 6 ps variable t 'ATLAS-like{/*0.6  (R='.default_Ratlas.')}',\
     m(cms_tag,default_Rcms)     u (@resilience):(@performance):((0.001*$2)**0.5*0.6) w linesp dt 1 lc rgb '#00cc00' lw 3 pt 5 ps variable t 'CMS-like{/*0.6  (R='.default_Rcms.')}'

unset for [i=51:55] label i

set out
