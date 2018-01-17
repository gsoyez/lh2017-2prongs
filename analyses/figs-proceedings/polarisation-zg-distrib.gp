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


set out 'polarisation-zg-distrib.pdf'


#------------------------------------------------------------------------
# 

R='0.8'
grm='tight'
set label 101 '{/*0.8 p_{t,jet}>'.pt.' GeV}'                    right at graph 0.97,0.6
set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'           right at graph 0.97,0.55
set label 103 sprintf('{/*0.8 %g<m_{%s}<%g GeV}',mmin,grm,mmax) right at graph 0.97,0.5


filejj=sprintf('../res/lhc14-dijets%d-pythia8230M13-truth-R%s.ntuple.gz',   1*pt, R)
fileSM=sprintf('../res/lhc14-WW%d-pythia8230M13-truth-R%s.ntuple.gz',   1*pt, R)
fileTr=sprintf('../res/lhc14-WW-M%d-trans-pythia8M13-R%s.ntuple.gz', 3*pt, R)
fileLg=sprintf('../res/lhc14-WW-M%d-long-pythia8M13-R%s.ntuple.gz',  3*pt, R)

count(file,grm)=system(sprintf('../scripts/get_count.sh %s "m_%s>%g" "m_%s<%g"',file,grm,mmin,grm,mmax))+0.0
m(file,grm)=sprintf('< ../scripts/histogram.sh %s zg_%s 0.1 0.5 0.02 "m_%s>%g" "m_%s<%g"',file,grm,grm,mmin,grm,mmax)


set xlabel 'z_{g,'.grm.'}'
set xrange [0.1:0.5]

set ylabel '1/N dN/dz_{g,'.grm.'}'

norm_jj=count(filejj, grm)
norm_SM=count(fileSM, grm)
norm_Tr=count(fileTr, grm)
norm_Lg=count(fileLg, grm)

plot m(fileLg, grm) u 2:($4/norm_Lg) w l dt 1 lc 1             lw 3 t 'WW-longitudinal',\
     m(fileTr, grm) u 2:($4/norm_Tr) w l dt 1 lc 3             lw 3 t 'WW-transverse',\
     m(fileSM, grm) u 2:($4/norm_SM) w l dt 1 lc rgb "#00bb00" lw 3 t 'WW-SM ',\
     m(filejj, grm) u 2:($4/norm_jj) w l dt 2 lc 7             lw 3 t 'dijets (SM)'

set out
