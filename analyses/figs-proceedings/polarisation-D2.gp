reset
set term pdfcairo enhanced size 9cm,8.5cm
set colors classic

call 'defaults.gp'

pt=default_pt

mmin=default_mmin
mmax=default_mmax

levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel 'LHDD@_2^{(2)} [D@_2^{(2)}(t{/Symbol \304}l/t)]'

set ylabel '1/N dN/dD@_2^{(2)}(t{/Symbol \304}l/t)'

set bmargin at screen 0.12
set tmargin at screen 0.97
set lmargin at screen 0.2
set rmargin at screen 0.97

set grid dt 3 lw 1

set key spacing 1.3


set style line 1 dt 2 lc rgb "#ff0000" lw 2
set style line 2 dt 1 lc rgb "#ff0000" lw 2
set style line 3 dt 2 lc rgb "#00dd00" lw 2
set style line 4 dt 1 lc rgb "#00dd00" lw 2
set style line 5 dt 2 lc rgb "#0000ff" lw 2
set style line 6 dt 1 lc rgb "#0000ff" lw 2
set style line 7 dt 2 lc rgb "#000000" lw 2
set style line 8 dt 1 lc rgb "#000000" lw 2


set out 'polarisation-D2.pdf'

# print "------------------------------------------------------------------------"
# print all_label
# do for [lab in all_label]{
#     print lab
# }
# print "------------------------------------------------------------------------"


R=default_R



set label 101 '{/*0.8 p_{t,jet}>'.pt.' GeV}'           right at graph 0.97,0.63
set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'  right at graph 0.97,0.68
set label 103 sprintf('{/*0.8 %g<m<%g GeV}',mmin,mmax) right at graph 0.97,0.58



#------------------------------------------------------------------------
# loop over the different setups
i=words(all_title)

this_title=word(all_title, i)
this_tag  =word(all_tag  , i)
this_label=word(all_label, i)
this_vmax =word(all_vmax , i)+0.0
this_dv   =word(all_dv   , i)+0.0
this_shape=word(all_shape, i)
this_mass =word(all_mass , i)

set xrange [0:this_vmax+0.0]
set yrange [0:word(all_ymax,i)+0.0]

set xrange [0:2]
set yrange [0:0.09]


count(proc)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s-R0.8.ntuple.gz "m_%s>%g" "m_%s<%g"',proc,this_mass,mmin,this_mass,mmax))+0.0
m(proc)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s-R0.8.ntuple.gz %s 0.0 %g %g "m_%s>%g" "m_%s<%g"',proc,this_shape,this_vmax,this_dv,this_mass,mmin,this_mass,mmax)

norm_jj =count('dijets1000-pythia8230M13-truth')
norm_SM =count('WW1000-pythia8230M13-truth')
norm_tr =count('WW-M3000-trans-pythia8M13')
norm_lg =count('WW-M3000-long-pythia8M13')          

plot m('dijets1000-pythia8230M13-truth') u 2:($4/norm_jj) w l dt 2 lc 7             lw 3 t 'QCD di-jets',\
     m('WW1000-pythia8230M13-truth')     u 2:($4/norm_SM) w l dt 1 lc 3             lw 3 t 'WW-SM',\
     m('WW-M3000-trans-pythia8M13')      u 2:($4/norm_tr) w l dt 1 lc 1             lw 3 t 'WW-transverse',\
     m('WW-M3000-long-pythia8M13')       u 2:($4/norm_lg) w l dt 1 lc rgb "#00cc00" lw 3 t 'WW-longitudinal'

set out
