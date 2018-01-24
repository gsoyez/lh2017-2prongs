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


set out 'polarisation-zg-roc.pdf'


#------------------------------------------------------------------------
# 

R='0.8'
grm='tight'
set label 101 '{/*0.8 p_{t,jet}>'.pt.' GeV}'                    right at graph 0.97,0.16
set label 102 '{/*0.8 Pythia8(M13), anti-k_t('.R.')}'           right at graph 0.97,0.11
set label 103 sprintf('{/*0.8 %g<m_{%s}<%g GeV}',mmin,grm,mmax) right at graph 0.97,0.06


fileSM=sprintf('../res/lhc14-WW%d-pythia8230M13-truth-R%s.ntuple.gz',1*pt, R)
fileTr=sprintf('../res/lhc14-WW-M%d-trans-pythia8M13-R%s.ntuple.gz', 3*pt, R)
fileLg=sprintf('../res/lhc14-WW-M%d-long-pythia8M13-R%s.ntuple.gz',  3*pt, R)

roc(fileS,fileB)=sprintf('< ../scripts/roc.sh %s %s 0.5-zg_%s 0.0 0.4 0.01 "m_%s>%g" "m_%s<%g"',fileS,fileB,grm,grm,mmin,grm,mmax)


set xlabel '{/Symbol e}_S (WW-polarised)'
set ylabel '{/Symbol e}_B (WW-SM)'

set key top left reverse Left

plot x w l dt 3 lw 2 lc 7 not,\
     roc(fileLg, fileSM) u 1:2 w l dt 1 lc 1 lw 3 t 'WW-longitudinal',\
     roc(fileTr, fileSM) u 1:2 w l dt 1 lc 3 lw 3 t 'WW-transverse'

set out
