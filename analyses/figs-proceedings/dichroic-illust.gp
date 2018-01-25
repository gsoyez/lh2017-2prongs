# illustrate how dichroic works by plotting signal and background distributioons for dichroic and for alternatives

reset
set term pdfcairo enhanced size 9cm,8cm
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

set xlabel '{/Symbol t}@_{21}^{(2)}'

set ylabel '1/N dN/d{/Symbol t}@_{21}^{(2)}'

set bmargin at screen 0.16
set tmargin at screen 0.91
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


set out 'dichroic-illust.pdf'

# print "------------------------------------------------------------------------"
# print all_label
# do for [lab in all_label]{
#     print lab
# }
# print "------------------------------------------------------------------------"


R=default_R
lev='truth'

set key maxrow 3 width -8.5
set key title 'WW    dijets'
    

set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'          right at graph 0.97,0.59
set label 101 '{/*0.8 p_{t,jet}>'.pt.' GeV}'                   right at graph 0.97,0.53
set label 103 sprintf('{/*0.8 %g<m_{tight}<%g GeV}',mmin,mmax) right at graph 0.97,0.47


#------------------------------------------------------------------------
# loop over the different setups

#set title '{/*1.4 N-subjettiness variants}'
set xrange [0:1.0]
set yrange [0:*]

count(proc)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s%s-pythia8230M13-%s-R%s.ntuple.gz "m_tight>%g" "m_tight<%g"',proc,pt,lev,R,mmin,mmax))+0.0
norm_WW=count('WW'    )
norm_jj=count('dijets')

m(proc,var)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia8230M13-%s-R%s.ntuple.gz tau21_beta2_%s 0.0 %g %g "m_tight>%g" "m_tight<%g"',proc,pt,lev,R,var,1.0,0.01,mmin,mmax)

plot m('WW'    ,'tt') u 2:($4/norm_WW) w l dt 2 lc 3             lw 3 t 'tight',\
     m('WW'    ,'ll') u 2:($4/norm_WW) w l dt 2 lc 1             lw 3 t 'loose',\
     m('WW'    ,'lt') u 2:($4/norm_WW) w l dt 2 lc rgb "#00cc00" lw 3 t 'dichroic',\
     m('dijets','tt') u 2:($4/norm_jj) w l dt 1 lc 3             lw 3 t ' ',\
     m('dijets','ll') u 2:($4/norm_jj) w l dt 1 lc 1             lw 3 t ' ',\
     m('dijets','lt') u 2:($4/norm_jj) w l dt 1 lc rgb "#00cc00" lw 3 t ' '


set out
