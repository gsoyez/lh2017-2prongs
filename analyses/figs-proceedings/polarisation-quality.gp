reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'polarisation-quality.pdf'

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

set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'   left at graph 0.03,0.05
set label 103 sprintf('{/*0.8 %g<m<%g GeV}',mmin,mmax) left at graph 0.03,0.10
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)    left at graph 0.03,0.15

m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-polarisation-R'.R.'.new | sed "s/#columns: //"; grep %s ../quality-measures/qualities-polarisation-R'.R.'.new',shape)

set macros


groomers="ppp lpp lpl lll tpp tpl tll tpt tlt ttt trimmed"
groomer_labels='"plain{/Symbol \304}plain/plain" "loose{/Symbol \304}plain/plain" "loose{/Symbol \304}plain/loose" "loose{/Symbol \304}loose/loose{/*0.7 (loose)}" "tight{/Symbol \304}plain/plain{/*0.7 (old-CMS-like)}" "tight{/Symbol \304}plain/loose" "tight{/Symbol \304}loose/loose" "tight{/Symbol \304}plain/tight" "tight{/Symbol \304}loose/tight{/*0.8 (dichroic)}" "tight{/Symbol \304}tight/tight{/*0.8 (CMS-like)}""trimmed {/*0.7 (ATLAS-like)}"'

shapes="M2_beta1 tau21_beta1 N2_beta1 D2_beta1 D2_beta2"
shape_labels='"M@_2^{(1)}" "{/Symbol t}@_{21}^{(1)}" "N@_2^{(1)} {/*0.8 (CMS-like)}" "D@_2^{(1)} {/*0.8 (ATLAS-like)}" "D@_2^{(2)} {/*0.8 (dichroic)}"'

set style line 1 dt (12,6,4,6) lc rgb "#00dddd" pt 13 
set style line 2 dt (4,3)      lc rgb "#00dd00" pt 11 
set style line 3 dt (12,6)     lc rgb "#ff0000" pt 9  
set style line 4 dt (8,6)      lc rgb "#000000" pt 5  
set style line 5 dt 1          lc rgb "#0000ff" pt 7  

set style line 1 dt 1     lc rgb "#ff0000" pt 9  
set style line 2 dt 1     lc rgb "#000000" pt 5  
set style line 3 dt 1     lc rgb "#0000ff" pt 7  
set style line 4 dt (8,6) lc rgb "#ff0000" pt 9  
set style line 5 dt (8,6) lc rgb "#000000" pt 5  
set style line 6 dt (8,6) lc rgb "#0000ff" pt 7  

set key spacing 1.2

# have wider lines for the reference points
set key top right noreverse Right
set key maxrows 3 width -10
set key title 'trans      long'

set title '{/*1.4 dependence on polarisation}

plot m('D2_beta1_trimmed') u (satx(resilience(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_trans'), column('epsilon_B_sm')))):(performance(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_trans'), column('epsilon_B_sm'))):((0.001*$2)**0.5*0.6) w linesp ls 1 ps variable lw 2 t 'ATLAS-like',\
     m('N2_beta1_ttt')     u (satx(resilience(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_trans'), column('epsilon_B_sm')))):(performance(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_trans'), column('epsilon_B_sm'))):((0.001*$2)**0.5*0.6) w linesp ls 2 ps variable lw 2 t 'CMS-like',\
     m('D2_beta2_tlt')     u (satx(resilience(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_trans'), column('epsilon_B_sm')))):(performance(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_trans'), column('epsilon_B_sm'))):((0.001*$2)**0.5*0.6) w linesp ls 3 ps variable lw 2 t 'dichroic D_2',\
     m('D2_beta1_trimmed') u (satx(resilience(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_long'),  column('epsilon_B_sm')))):(performance(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_long'),  column('epsilon_B_sm'))):((0.001*$2)**0.5*0.6) w linesp ls 4 ps variable lw 2 t ' ',\
     m('N2_beta1_ttt')     u (satx(resilience(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_long'),  column('epsilon_B_sm')))):(performance(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_long'),  column('epsilon_B_sm'))):((0.001*$2)**0.5*0.6) w linesp ls 5 ps variable lw 2 t ' ',\
     m('D2_beta2_tlt')     u (satx(resilience(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_long'),  column('epsilon_B_sm')))):(performance(column('epsilon_S_SM'), column('epsilon_B_sm'), column('epsilon_S_long'),  column('epsilon_B_sm'))):((0.001*$2)**0.5*0.6) w linesp ls 6 ps variable lw 2 t ' '
     
set out
