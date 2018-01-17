reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic

call 'defaults.gp'


set xlabel 'm [GeV]'
set ylabel 'm/N dN/dm'

set bmargin at screen 0.12
set tmargin at screen 0.91
set lmargin at screen 0.2
set rmargin at screen 0.955

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


set out 'mass-distribs.pdf'

# print "------------------------------------------------------------------------"
# print all_label
# do for [lab in all_label]{
#     print lab
# }
# print "------------------------------------------------------------------------"


pt=default_pt

R=default_R

set label 101 '{/*0.8 Pythia8(M13)}'    left at graph 0.025,0.955
set label 102 '{/*0.8 anti-k_t('.R.')}' left at graph 0.025,0.91
set label 103 '{/*0.8 p_{t,jet}>1 TeV}' left at graph 0.025,0.865

mmin=6.0
mmax=1000.0

set xrange [mmin:mmax]
set log x

set yrange [0:0.03]


#------------------------------------------------------------------------
# loop over the different setups
count(proc,lev)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s%s-pythia8230M13-%s-R%s.ntuple.gz',proc,pt,lev,R))+0.0
norm_jj_truth =count('dijets','truth')
norm_jj_hadron=count('dijets','hadron')
norm_jj_parton=count('dijets','parton')

m(proc,lev,grm)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia8230M13-%s-R%s.ntuple.gz "(m_%s>0) ? log(m_%s) : -1000.0" %g %g %g',proc,pt,lev,R,grm,grm,log(mmin),log(mmax),log(mmax/mmin)/100.0)

set title '{/*1.4 groomed mass}'
set key maxrow 3 width -6
set key at graph 0.96,0.925
set label 1 'tight      loose       trim' right at graph 0.93,0.95
plot m('dijets','truth' ,'tight')   u (exp($2)):($4/norm_jj_truth ) w l dt 1          lc 3             lw 3 t 'truth',\
     m('dijets','hadron','tight')   u (exp($2)):($4/norm_jj_hadron) w l dt 1          lc 1             lw 3 t 'hadron',\
     m('dijets','parton','tight')   u (exp($2)):($4/norm_jj_parton) w l dt 1          lc rgb "#00cc00" lw 3 t 'parton',\
     m('dijets','truth' ,'loose')   u (exp($2)):($4/norm_jj_truth ) w l dt 2          lc 3             lw 3 t ' ',\
     m('dijets','hadron','loose')   u (exp($2)):($4/norm_jj_hadron) w l dt 2          lc 1             lw 3 t ' ',\
     m('dijets','parton','loose')   u (exp($2)):($4/norm_jj_parton) w l dt 2          lc rgb "#00cc00" lw 3 t ' ',\
     m('dijets','truth' ,'trimmed') u (exp($2)):($4/norm_jj_truth ) w l dt (12,6,4,6) lc 3             lw 3 t ' ',\
     m('dijets','hadron','trimmed') u (exp($2)):($4/norm_jj_hadron) w l dt (12,6,4,6) lc 1             lw 3 t ' ',\
     m('dijets','parton','trimmed') u (exp($2)):($4/norm_jj_parton) w l dt (12,6,4,6) lc rgb "#00cc00" lw 3 t ' '

# do for [grm in "tight loose trimmed"]{
#     set title '{/*1.4 '.grm.'}'
# 
#     m(proc,lev)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia8230M13-%s-R%s.ntuple.gz "(m_%s>0) ? log(m_%s) : -1000.0" %g %g %g',proc,pt,lev,R,grm,grm,log(mmin),log(mmax),log(mmax/mmin)/100.0)
# 
# 
#     plot m('dijets','truth')  u (exp($2)):($4/norm_jj_truth ) w l dt 1 lc 3             lw 3 t 'truth',\
#          m('dijets','hadron') u (exp($2)):($4/norm_jj_hadron) w l dt 1 lc 1             lw 3 t 'hadron',\
#          m('dijets','parton') u (exp($2)):($4/norm_jj_parton) w l dt 1 lc rgb "#00cc00" lw 3 t 'parton'
# }

set out
