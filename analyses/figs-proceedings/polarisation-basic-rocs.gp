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

set xlabel '{/Symbol e}_S'
set ylabel '{/Symbol e}_B'
set log y
set yrange [1e-4:0.2]

set bmargin at screen 0.12
set tmargin at screen 0.91
set lmargin at screen 0.2
set rmargin at screen 0.97

set grid dt 3 lw 1

set key spacing 1.2


set style line 1 dt 2 lc rgb "#ff0000" lw 2
set style line 2 dt 1 lc rgb "#ff0000" lw 2
set style line 3 dt 2 lc rgb "#00dd00" lw 2
set style line 4 dt 1 lc rgb "#00dd00" lw 2
set style line 5 dt 2 lc rgb "#0000ff" lw 2
set style line 6 dt 1 lc rgb "#0000ff" lw 2
set style line 7 dt 2 lc rgb "#000000" lw 2
set style line 8 dt 1 lc rgb "#000000" lw 2


set out 'polarisation-basic-rocs.pdf'


R=default_R

set key maxrow 3 width -11.0
set key bottom right


set label 101 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'  left at graph 0.03,0.94
set label 102 '{/*0.8 p_{t,jet}>'.pt.' GeV}'           left at graph 0.03,0.89
set label 103 sprintf('{/*0.8 %g<m<%g GeV}',mmin,mmax) left at graph 0.03,0.84


unset title 
set label 1 'ATLAS       CMS     Dichroic' right at graph 0.96,0.215


fileSM=sprintf('../res/lhc14-WW%d-pythia8230M13-truth-R%s.ntuple.gz',1*pt, R)
fileTr=sprintf('../res/lhc14-WW-M%d-trans-pythia8M13-R%s.ntuple.gz', 3*pt, R)
fileLg=sprintf('../res/lhc14-WW-M%d-long-pythia8M13-R%s.ntuple.gz',  3*pt, R)

m(fS,shape,mass,vmax,dv)=sprintf('< ../scripts/roc.sh %s ../res/lhc14-dijets%s-pythia8230M13-truth-R%s.ntuple.gz %s 0.0 %g %g "m_%s>%g" "m_%s<%g"',fS,pt,R,shape,vmax+0.0,dv+0.0,mass,mmin,mass,mmax)

plot m(fileSM, word(all_shape,1), word(all_mass,1), word(all_vmax,1), word(all_dv,1)) u 1:2 w l dt 1          lc 3             lw 3 t 'SM',\
     m(fileTr, word(all_shape,1), word(all_mass,1), word(all_vmax,1), word(all_dv,1)) u 1:2 w l dt 1          lc 1             lw 3 t 'Transverse',\
     m(fileLg, word(all_shape,1), word(all_mass,1), word(all_vmax,1), word(all_dv,1)) u 1:2 w l dt 1          lc rgb "#00cc00" lw 3 t 'Longitudinal',\
     m(fileSM, word(all_shape,2), word(all_mass,2), word(all_vmax,2), word(all_dv,2)) u 1:2 w l dt 2          lc 3             lw 3 t ' ',\
     m(fileTr, word(all_shape,2), word(all_mass,2), word(all_vmax,2), word(all_dv,2)) u 1:2 w l dt 2          lc 1             lw 3 t ' ',\
     m(fileLg, word(all_shape,2), word(all_mass,2), word(all_vmax,2), word(all_dv,2)) u 1:2 w l dt 2          lc rgb "#00cc00" lw 3 t ' ',\
     m(fileSM, word(all_shape,3), word(all_mass,3), word(all_vmax,3), word(all_dv,3)) u 1:2 w l dt (12,6,4,6) lc 3             lw 3 t ' ',\
     m(fileTr, word(all_shape,3), word(all_mass,3), word(all_vmax,3), word(all_dv,3)) u 1:2 w l dt (12,6,4,6) lc 1             lw 3 t ' ',\
     m(fileLg, word(all_shape,3), word(all_mass,3), word(all_vmax,3), word(all_dv,3)) u 1:2 w l dt (12,6,4,6) lc rgb "#00cc00" lw 3 t ' '

set out
