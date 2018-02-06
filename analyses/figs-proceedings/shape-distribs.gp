reset
set term pdfcairo enhanced size 9cm,9cm
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

set xlabel 'v'

set ylabel '1/N dN/dv'

set bmargin at screen 0.12
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


set out 'shape-distribs.pdf'

# print "------------------------------------------------------------------------"
# print all_label
# do for [lab in all_label]{
#     print lab
# }
# print "------------------------------------------------------------------------"


R=default_R

set key maxrow 3 width -6.5
set key title 'WW    dijets'
    

set label 101 '{/*0.8 p_{t,jet}>'.pt.' GeV}'           right at graph 0.97,0.63
set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'  right at graph 0.97,0.68
set label 103 sprintf('{/*0.8 %g<m<%g GeV}',mmin,mmax) right at graph 0.97,0.58



#------------------------------------------------------------------------
# loop over the different setups
do for [i=1:words(all_title)]{
    this_title=word(all_title, i)
    this_tag  =word(all_tag  , i)
    this_label=word(all_label, i)
    this_vmax =word(all_vmax , i)+0.0
    this_dv   =word(all_dv   , i)+0.0
    this_shape=word(all_shape, i)
    this_mass =word(all_mass , i)

    set title '{/*1.4 '.this_label.'}'
    set xrange [0:this_vmax+0.0]
    set yrange [0:word(all_ymax,i)+0.0]


    count(proc,lev)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s%s-pythia8230M13-%s-R%s.ntuple.gz "m_%s>%g" "m_%s<%g"',proc,pt,lev,R,this_mass,mmin,this_mass,mmax))+0.0
    m(proc,lev)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia8230M13-%s-R%s.ntuple.gz %s 0.0 %g %g "m_%s>%g" "m_%s<%g"',proc,pt,lev,R,this_shape,this_vmax,this_dv,this_mass,mmin,this_mass,mmax)

    norm_WW_truth =count('WW'    ,'truth')
    norm_WW_hadron=count('WW'    ,'hadron')
    norm_WW_parton=count('WW'    ,'parton')
    norm_jj_truth =count('dijets','truth')
    norm_jj_hadron=count('dijets','hadron')
    norm_jj_parton=count('dijets','parton')

    plot m('WW'    ,'truth' ) u 2:($4/norm_WW_truth ) w l dt 1 lc 3             lw 3 t 'truth',\
         m('WW'    ,'hadron') u 2:($4/norm_WW_hadron) w l dt 1 lc 1             lw 3 t 'hadron',\
         m('WW'    ,'parton') u 2:($4/norm_WW_parton) w l dt 1 lc rgb "#00cc00" lw 3 t 'parton',\
         m('dijets','truth')  u 2:($4/norm_jj_truth ) w l dt 2 lc 3             lw 3 t ' ',\
         m('dijets','hadron') u 2:($4/norm_jj_hadron) w l dt 2 lc 1             lw 3 t ' ',\
         m('dijets','parton') u 2:($4/norm_jj_parton) w l dt 2 lc rgb "#00cc00" lw 3 t ' '
}

set out
