reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic

call 'defaults.gp'

pt=default_pt
levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel 'v'

set ylabel '1/N_{truth} dN/dv'
set yrange [0:0.05]

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


do for [R in "0.8 1.0"]{
    set out 'shape-distribs-R'.R.'.pdf'

    set label 101 '{/*0.8 p_{t,jet}>'.pt.' GeV}'         right at graph 0.97,0.63
    set label 102 '{/*0.8 Pythia8(4C), anti-k_t('.R.')}' right at graph 0.97,0.58
    set label 103 '{/*0.8 60<m<100}'                     right at graph 0.97,0.53

    #------------------------------------------------------------------------
    # ATLAS
    set title '{/*1.4 ATLAS-like} ('.reflabel.' v. '.altlabel.')'
    set xrange [0:atlas_vmax+0.0]

    count(proc,lev)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz "m_%s>60" "m_%s<100"',proc,pt,lev,R,atlas_mass,atlas_mass))+0.0
    m(proc,lev)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz %s 0.0 %g %g "m_%s>60" "m_%s<100"',proc,pt,lev,R,atlas_shape,atlas_vmax,atlas_dv,atlas_mass,atlas_mass)

    norm_WW=count('WW'    ,levelref)
    norm_jj=count('dijets',levelref)

    plot m('WW'    ,levelref) u 2:($4/norm_WW) w l dt 1 lc 3 lw 3 t 'signal, '.levelref,\
         m('WW'    ,levelalt) u 2:($4/norm_WW) w l dt 1 lc 1 lw 3 t 'signal, '.levelalt,\
         m('dijets',levelref) u 2:($4/norm_jj) w l dt 2 lc 3 lw 3 t 'bkg, '   .levelref,\
         m('dijets',levelalt) u 2:($4/norm_jj) w l dt 2 lc 1 lw 3 t 'bkg, '   .levelalt

    #------------------------------------------------------------------------
    # CMS
    set title '{/*1.4 CMS-like} ('.reflabel.' v. '.altlabel.')'
    set xrange [0:cms_vmax+0.0]

    set key top left reverse Left
    set label 101 left at graph 0.03,0.63
    set label 102 left at graph 0.03,0.58
    set label 103 left at graph 0.03,0.53

    count(proc,lev)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz "m_%s>60" "m_%s<100"',proc,pt,lev,R,cms_mass,cms_mass))+0.0
    m(proc,lev)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz %s 0.0 %g %g "m_%s>60" "m_%s<100"',proc,pt,lev,R,cms_shape,cms_vmax,cms_dv,cms_mass,cms_mass)

    norm_WW=count('WW'    ,levelref)
    norm_jj=count('dijets',levelref)

    plot m('WW'    ,levelref) u 2:($4/norm_WW) w l dt 1 lc 3 lw 3 t 'signal, '.levelref,\
         m('WW'    ,levelalt) u 2:($4/norm_WW) w l dt 1 lc 1 lw 3 t 'signal, '.levelalt,\
         m('dijets',levelref) u 2:($4/norm_jj) w l dt 2 lc 3 lw 3 t 'bkg, '   .levelref,\
         m('dijets',levelalt) u 2:($4/norm_jj) w l dt 2 lc 1 lw 3 t 'bkg, '   .levelalt

    set key top right noreverse Right
    set label 101 right at graph 0.97,0.63
    set label 102 right at graph 0.97,0.58
    set label 103 right at graph 0.97,0.53

    #------------------------------------------------------------------------
    # resilient
    set title '{/*1.4 resilience-oriented} ('.reflabel.' v. '.altlabel.')'
    set xrange [0:resi_vmax+0.0]

    count(proc,lev)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz "m_%s>60" "m_%s<100"',proc,pt,lev,R,resi_mass,resi_mass))+0.0
    m(proc,lev)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz %s 0.0 %g %g "m_%s>60" "m_%s<100"',proc,pt,lev,R,resi_shape,resi_vmax,resi_dv,resi_mass,resi_mass)

    norm_WW=count('WW'    ,levelref)
    norm_jj=count('dijets',levelref)

    plot m('WW'    ,levelref) u 2:($4/norm_WW) w l dt 1 lc 3 lw 3 t 'signal, '.levelref,\
         m('WW'    ,levelalt) u 2:($4/norm_WW) w l dt 1 lc 1 lw 3 t 'signal, '.levelalt,\
         m('dijets',levelref) u 2:($4/norm_jj) w l dt 2 lc 3 lw 3 t 'bkg, '   .levelref,\
         m('dijets',levelalt) u 2:($4/norm_jj) w l dt 2 lc 1 lw 3 t 'bkg, '   .levelalt

    #------------------------------------------------------------------------
    # performant
    set title '{/*1.4 performance-oriented} ('.reflabel.' v. '.altlabel.')'
    set xrange [0:perf_vmax+0.0]

    count(proc,lev)=system(sprintf('../scripts/get_count.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz "m_%s>60" "m_%s<100"',proc,pt,lev,R,perf_mass,perf_mass))+0.0
    m(proc,lev)=sprintf('< ../scripts/histogram.sh ../res/lhc14-%s%s-pythia84C-%s-R%s.ntuple.gz %s 0.0 %g %g "m_%s>60" "m_%s<100"',proc,pt,lev,R,perf_shape,perf_vmax,perf_dv,perf_mass,perf_mass)

    norm_WW=count('WW'    ,levelref)
    norm_jj=count('dijets',levelref)

    plot m('WW'    ,levelref) u 2:($4/norm_WW) w l dt 1 lc 3 lw 3 t 'signal, '.levelref,\
         m('WW'    ,levelalt) u 2:($4/norm_WW) w l dt 1 lc 1 lw 3 t 'signal, '.levelalt,\
         m('dijets',levelref) u 2:($4/norm_jj) w l dt 2 lc 3 lw 3 t 'bkg, '   .levelref,\
         m('dijets',levelalt) u 2:($4/norm_jj) w l dt 2 lc 1 lw 3 t 'bkg, '   .levelalt
}

set out
