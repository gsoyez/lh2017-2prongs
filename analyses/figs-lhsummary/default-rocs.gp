reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'default-rocs.pdf'

call 'defaults.gp'

R=default_R
pt=default_pt
levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel '{/Symbol e}_S'
set xrange [0:0.8]

set ylabel '{/Symbol e}_B'
set yrange [0.0001:0.2]
set log y
set format y '10^{%T}'

set grid dt 3 lw 1

set label 101 '{/*0.8 p_{t,jet}>'.pt.' GeV}'         right at graph 0.97,0.15
set label 102 '{/*0.8 Pythia8(4C), anti-k_t('.R.')}' right at graph 0.97,0.10
set label 103 '{/*0.8 60<m<100}'                     right at graph 0.97,0.05

set key spacing 1.3
set key top left reverse Left

#------------------------------------------------------------------------
# ATLAS
set title '{/*1.4 ATLAS-like} ('.reflabel.' v. '.altlabel.')'

m(level)=sprintf('< ../scripts/roc.sh ../res/lhc14-WW%s-pythia84C-%s-R'.R.'.ntuple.gz ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz "%s" 0.0 %g %g "mass_%s>60.0" "mass_%s<100.0"',pt,level,pt,level,atlas_shape,atlas_vmax,atlas_dv,atlas_mass,atlas_mass)
eS1="0.4"
cut=system(sprintf('../scripts/get_cut.sh        ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz "%s" 0.4 "mass_%s>60.0" "mass_%s<100.0"',pt,levelref,atlas_shape,atlas_mass,atlas_mass))
eB1=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelref,atlas_mass,atlas_mass,atlas_shape,cut))
eS2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,atlas_mass,atlas_mass,atlas_shape,cut))
eB2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,atlas_mass,atlas_mass,atlas_shape,cut))

set arrow 1 from eS1+0.0,0.0001  to eS1+0.0,eB1+0.0 nohead dt 2 lw 1.5 lc 3
set arrow 2 from eS1+0.0,eB1+0.0 to eS2+0.0,eB2+0.0 nohead dt 2 lw 1.5 lc 7

set label 21 '{/*0.8 performance}'         at eS1+0.03,sqrt(0.0001*eB1)       rotate by 90
set label 22 '{/*0.8 1/resilience}' center at 0.5*(eS1+eS2),1.7*sqrt(eB1*eB2) rotate by 35

print m(levelref)
plot m(levelref) u 1:2 w l dt 1 lw 3 lc 3 t levelref,\
     m(levelalt) u 1:2 w l dt 1 lw 3 lc 1 t levelalt,\
     '< echo "'.eS1.' '.eB1.'"' u 1:2 w p pt 5 ps 1 lc 3 not,\
     '< echo "'.eS2.' '.eB2.'"' u 1:2 w p pt 5 ps 1 lc 1 not

#------------------------------------------------------------------------
# CMS
set title '{/*1.4 CMS-like} ('.reflabel.' v. '.altlabel.')'

m(level)=sprintf('< ../scripts/roc.sh ../res/lhc14-WW%s-pythia84C-%s-R'.R.'.ntuple.gz ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz "%s" 0.0 %g %g "mass_%s>60.0" "mass_%s<100.0"',pt,level,pt,level,cms_shape,cms_vmax,cms_dv,cms_mass,cms_mass)
eS1="0.4"
cut=system(sprintf('../scripts/get_cut.sh        ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz "%s" 0.4 "mass_%s>60.0" "mass_%s<100.0"',pt,levelref,cms_shape,cms_mass,cms_mass))
eB1=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelref,cms_mass,cms_mass,cms_shape,cut))
eS2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,cms_mass,cms_mass,cms_shape,cut))
eB2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,cms_mass,cms_mass,cms_shape,cut))

set arrow 1 from eS1+0.0,0.0001  to eS1+0.0,eB1+0.0 nohead dt 2 lw 1.5 lc 3
set arrow 2 from eS1+0.0,eB1+0.0 to eS2+0.0,eB2+0.0 nohead dt 2 lw 1.5 lc 7

set label 21 '{/*0.8 performance}'         at eS1+0.03,sqrt(0.0001*eB1)       rotate by 90
set label 22 '{/*0.8 1/resilience}' center at 0.5*(eS1+eS2),1.7*sqrt(eB1*eB2) rotate by 35

print m(levelref)
plot m(levelref) u 1:2 w l dt 1 lw 3 lc 3 t levelref,\
     m(levelalt) u 1:2 w l dt 1 lw 3 lc 1 t levelalt,\
     '< echo "'.eS1.' '.eB1.'"' u 1:2 w p pt 5 ps 1 lc 3 not,\
     '< echo "'.eS2.' '.eB2.'"' u 1:2 w p pt 5 ps 1 lc 1 not

#------------------------------------------------------------------------
# resilient
set title '{/*1.4 resilience-oriented} ('.reflabel.' v. '.altlabel.')'

m(level)=sprintf('< ../scripts/roc.sh ../res/lhc14-WW%s-pythia84C-%s-R'.R.'.ntuple.gz ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz "%s" 0.0 %g %g "mass_%s>60.0" "mass_%s<100.0"',pt,level,pt,level,resi_shape,resi_vmax,resi_dv,resi_mass,resi_mass)
eS1="0.4"
cut=system(sprintf('../scripts/get_cut.sh        ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz "%s" 0.4 "mass_%s>60.0" "mass_%s<100.0"',pt,levelref,resi_shape,resi_mass,resi_mass))
eB1=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelref,resi_mass,resi_mass,resi_shape,cut))
eS2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,resi_mass,resi_mass,resi_shape,cut))
eB2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,resi_mass,resi_mass,resi_shape,cut))

set arrow 1 from eS1+0.0,0.0001  to eS1+0.0,eB1+0.0 nohead dt 2 lw 1.5 lc 3
set arrow 2 from eS1+0.0,eB1+0.0 to eS2+0.0,eB2+0.0 nohead dt 2 lw 1.5 lc 7

set label 21 '{/*0.8 performance}'         at eS1+0.03,sqrt(0.0001*eB1)       rotate by 90
set label 22 '{/*0.8 1/resilience}' center at 0.5*(eS1+eS2),1.7*sqrt(eB1*eB2) rotate by 35

print m(levelref)
plot m(levelref) u 1:2 w l dt 1 lw 3 lc 3 t levelref,\
     m(levelalt) u 1:2 w l dt 1 lw 3 lc 1 t levelalt,\
     '< echo "'.eS1.' '.eB1.'"' u 1:2 w p pt 5 ps 1 lc 3 not,\
     '< echo "'.eS2.' '.eB2.'"' u 1:2 w p pt 5 ps 1 lc 1 not

#------------------------------------------------------------------------
# performant
set title '{/*1.4 performance-oriented} ('.reflabel.' v. '.altlabel.')'

m(level)=sprintf('< ../scripts/roc.sh ../res/lhc14-WW%s-pythia84C-%s-R'.R.'.ntuple.gz ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz "%s" 0.0 %g %g "mass_%s>60.0" "mass_%s<100.0"',pt,level,pt,level,perf_shape,perf_vmax,perf_dv,perf_mass,perf_mass)
eS1="0.4"
cut=system(sprintf('../scripts/get_cut.sh        ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz "%s" 0.4 "mass_%s>60.0" "mass_%s<100.0"',pt,levelref,perf_shape,perf_mass,perf_mass))
eB1=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelref,perf_mass,perf_mass,perf_shape,cut))
eS2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-WW%s-pythia84C-%s-R'    .R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,perf_mass,perf_mass,perf_shape,cut))
eB2=system(sprintf('../scripts/get_efficiency.sh ../res/lhc14-dijets%s-pythia84C-%s-R'.R.'.ntuple.gz  "mass_%s>60.0" "mass_%s<100.0" "%s<%s"',pt,levelalt,perf_mass,perf_mass,perf_shape,cut))

set arrow 1 from eS1+0.0,0.0001  to eS1+0.0,eB1+0.0 nohead dt 2 lw 1.5 lc 3
set arrow 2 from eS1+0.0,eB1+0.0 to eS2+0.0,eB2+0.0 nohead dt 2 lw 1.5 lc 7

set label 21 '{/*0.8 performance}'         at eS1+0.03,sqrt(0.0001*eB1)       rotate by 90
set label 22 '{/*0.8 1/resilience}' center at 0.5*(eS1+eS2),1.7*sqrt(eB1*eB2) rotate by 35

print m(levelref)
plot m(levelref) u 1:2 w l dt 1 lw 3 lc 3 t levelref,\
     m(levelalt) u 1:2 w l dt 1 lw 3 lc 1 t levelalt,\
     '< echo "'.eS1.' '.eB1.'"' u 1:2 w p pt 5 ps 1 lc 3 not,\
     '< echo "'.eS2.' '.eB2.'"' u 1:2 w p pt 5 ps 1 lc 1 not

set out
