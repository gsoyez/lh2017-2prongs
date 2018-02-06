reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic

call 'defaults.gp'


eS=default_Sref
mmin=default_mmin
mmax=default_mmax

levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel 'resilience'
set xrange [0:7]

set ylabel 'performance'
set yrange [0:7]

set grid dt 3 lw 1

set label 11 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)    right at graph 0.97,0.185
set label 12 sprintf('{/*0.8 %g<m<%g GeV}',mmin,mmax) right at graph 0.97,0.14
set label 13 '{/*0.8 Pythia8(M13)}'                   right at graph 0.97,0.085
set label 14 '{/*0.8 anti-k_t(0.8)}'                  right at graph 0.97,0.04

#          +   x   *   sqr     o       tr     dia
groomers="ppp lll lpl lpp ttt tlt tpt tll tpl tpp trimmed"
#labs='"tight{/Symbol \304}loose/tight" "loose{/Symbol \304}loose/loose" "tight{/Symbol \304}tight/tight" "trim{/Symbol \304}trim/trim" "tight{/Symbol \304}plain/plain"'

shapes="tau21_beta1 tau21_beta2 N2_beta1 N2_beta2 D2_beta1 D2_beta2 M2_beta1 M2_beta2"
#slabs='"{/Symbol t}_{21}" "N_2" "D_2" "M_2"'

# let us hardcode the column numbers for truth v. parton, ATLAS, CMS and pileup
merge="< grep -v '^#' ../quality-measures/qualities-R0.8.new"

mtruth(pt)=merge.' | awk "{if (\$2=='.pt.'){print \$1,\$9,\$10,\$11}}"  | ../quality-measures/optimal-line-new'
mpu50 (pt)=merge.' | awk "{if (\$2=='.pt.'){print \$1,\$12,\$13,\$14}}" | ../quality-measures/optimal-line-new'
matlas(pt)=merge.' | awk "{if (\$2=='.pt.'){print \$1,\$15,\$16,\$17}}" | ../quality-measures/optimal-line-new'
mcms  (pt)=merge.' | awk "{if (\$2=='.pt.'){print \$1,\$18,\$19,\$20}}" | ../quality-measures/optimal-line-new'

set macros

set key spacing 1.2
set title reflabel.' v. '.altlabel 


# We'll place the labels by hand, so proceed pt by pt


#------------------------------------------------------------------------
# pt=500
#------------------------------------------------------------------------

pt=500
set out 'optimal-pt'.pt.'.pdf'
set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96

x0=0.9
y0=6.3
dx=0.43
dy=0.34

set label 100 '{/*0.8 N@_2^{(1)}[t{/Symbol \304}p/t]'             center at x0+ 0*dx, y0- 0*dy
set label 101 '{/*0.8 N@_2^{(1)}[t{/Symbol \304}p/p]'             center at x0+ 1*dx, y0- 1*dy
set label 102 '{/*0.8 N@_2^{(2)}[l{/Symbol \304}l/l]'             center at x0+ 2*dx, y0- 2*dy
set label 103 '{/*0.8 D@_2^{(2)}[l{/Symbol \304}l/l]'             center at x0+ 3*dx, y0- 3*dy
set label 104 '{/*0.8 D@_2^{(2)}[t{/Symbol \304}l/l]'             center at x0+ 4*dx, y0- 4*dy
set label 105 '{/*0.8 D@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+ 5*dx, y0- 5*dy
set label 106 '{/*0.8 N@_2^{(2)}[t{/Symbol \304}l/l]'             center at x0+ 6*dx, y0- 6*dy
set label 107 '{/*0.8 N@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+ 7*dx, y0- 7*dy
set label 108 '{/*0.8 {/Symol t}@_{21}^{(2)}[t{/Symbol \304}l/l]' center at x0+ 8*dx, y0- 8*dy
set label 109 '{/*0.8 {/Symol t}@_{21}^{(2)}[t{/Symbol \304}l/t]' center at x0+ 9*dx, y0- 9*dy
set label 110 '{/*0.8 D@_2^{(1)}[t{/Symbol \304}l/t]'             center at x0+10*dx, y0-10*dy
set label 111 '{/*0.8 M@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+11*dx, y0-11*dy
set label 112 '{/*0.8 M@_2^{(2)}[l{/Symbol \304}l/l]'             center at x0+12*dx, y0-12*dy

x0=0.55
y0=6.1
dx=0.41
dy=0.33

set arrow 100 from 1.07105, 3.54045 to x0+ 0*dx, y0- 0*dy size 0.15,15 filled
set arrow 101 from 1.12377, 3.46669 to x0+ 1*dx, y0- 1*dy size 0.15,15 filled
set arrow 102 from 1.21644, 3.41189 to x0+ 2*dx, y0- 2*dy size 0.15,15 filled
set arrow 103 from 1.60209, 3.40638 to x0+ 3*dx, y0- 3*dy size 0.15,15 filled
set arrow 104 from 1.86308, 3.36739 to x0+ 4*dx, y0- 4*dy size 0.15,15 filled
set arrow 105 from 1.86459, 3.36614 to x0+ 5*dx, y0- 5*dy size 0.15,15 filled
set arrow 106 from 1.87673, 3.31491 to x0+ 6*dx, y0- 6*dy size 0.15,15 filled
set arrow 107 from 1.88395, 3.31481 to x0+ 7*dx, y0- 7*dy size 0.15,15 filled
set arrow 108 from 2.09476, 3.10492 to x0+ 8*dx, y0- 8*dy size 0.15,15 filled
set arrow 109 from 2.11459, 3.10085 to x0+ 9*dx, y0- 9*dy size 0.15,15 filled
set arrow 110 from 2.43067, 2.81317 to x0+10*dx, y0-10*dy size 0.15,15 filled
set arrow 111 from 2.80932, 2.6873  to x0+11*dx, y0-11*dy size 0.15,15 filled
set arrow 112 from 4.25584, 1.74356 to x0+12*dx, y0-12*dy size 0.15,15 filled


plot mtruth(pt) i 1 u 2:3 w l         lc rgb "#aaaa00" lw 3 t 'optimal line',\
     mtruth(pt) i 0 u 2:3 w p pt 13 ps 0.7 lc 1 t 'all studied',\
     mtruth(pt).' | grep "^D2_beta1_trim"' u 2:3 w p pt  5 ps 1.0 lc rgb "#000000" t 'ATLAS-like',\
     mtruth(pt).' | grep "^N2_beta1_ttt"'  u 2:3 w p pt  9 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
     mtruth(pt).' | grep "^D2_beta2_tlt"'  u 2:3 w p pt  7 ps 1.0 lc rgb "#0000ff" t 'LHDD@_2^{(2)}'

unset for [i=100:112] label i
unset for [i=100:112] arrow i

#------------------------------------------------------------------------
# pt=1000
#------------------------------------------------------------------------
pt=1000
set out 'optimal-pt'.pt.'.pdf'
set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96

x0=0.9
y0=6.1
dx=0.75
dy=0.4

set label 100 '{/*0.8 N@_2^{(1)}[l{/Symbol \304}p/l]'             center at x0+0*dx, y0-0*dy
set label 101 '{/*0.8 D@_2^{(2)}[l{/Symbol \304}l/l]'             center at x0+1*dx, y0-1*dy
set label 102 '{/*0.8 D@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+2*dx, y0-2*dy
set label 103 '{/*0.8 N@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+3*dx, y0-3*dy
set label 104 '{/*0.8 {/Symol t}@_{21}^{(2)}[t{/Symbol \304}l/t]' center at x0+4*dx, y0-4*dy
set label 105 '{/*0.8 M@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+5*dx, y0-5*dy
set label 106 '{/*0.8 M@_2^{(2)}[t{/Symbol \304}p/p]'             center at x0+6*dx, y0-6*dy
set label 107 '{/*0.8 M@_2^{(2)}[l{/Symbol \304}l/l]'             center at x0+7*dx, y0-7*dy

x0=0.73
y0=5.9
dx=0.74
dy=0.4

set arrow 100 from  0.795503, 4.50552 to x0+ 0*dx, y0- 0*dy size 0.15,15 filled
set arrow 101 from  1.72168,  4.45555 to x0+ 1*dx, y0- 1*dy size 0.15,15 filled
set arrow 102 from  2.11554,  4.37685 to x0+ 2*dx, y0- 2*dy size 0.15,15 filled
set arrow 103 from  2.60566,  3.92174 to x0+ 3*dx, y0- 3*dy size 0.15,15 filled
set arrow 104 from  2.83651,  3.69951 to x0+ 4*dx, y0- 4*dy size 0.15,15 filled
set arrow 105 from  3.8368,   3.15139 to x0+ 5*dx, y0- 5*dy size 0.15,15 filled
set arrow 106 from  4.53103,  2.8563  to x0+ 6*dx, y0- 6*dy size 0.15,15 filled
set arrow 107 from  5.61176,  1.68161 to x0+ 7*dx, y0- 7*dy size 0.15,15 filled

plot mtruth(pt) i 1 u 2:3 w l         lc rgb "#aaaa00" lw 3 t 'optimal line',\
     mtruth(pt) i 0 u 2:3 w p pt 13 ps 0.7 lc 1 t 'all studied',\
     mtruth(pt).' | grep "^D2_beta1_trim"' u 2:3 w p pt  5 ps 1.0 lc rgb "#000000" t 'ATLAS-like',\
     mtruth(pt).' | grep "^N2_beta1_ttt"'  u 2:3 w p pt  9 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
     mtruth(pt).' | grep "^D2_beta2_tlt"'  u 2:3 w p pt  7 ps 1.0 lc rgb "#0000ff" t 'LHDD@_2^{(2)}'
# plot mtruth(pt) i 0 u 2:3 w p pt 7 ps 0.7 lc 1 t 'all studied',\
#      mtruth(pt) i 1 u 2:3 w l        lc 3 lw 3 t 'optimal line',\
#      mtruth(pt).' | grep "^D2_beta1_trim"' u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'ATLAS-like',\
#      mtruth(pt).' | grep "^N2_beta1_ttt"'  u 2:3 w p pt  5 ps 1.2 lc rgb "#00cc00" t 'CMS-like'

unset for [i=100:107] label i
unset for [i=100:107] arrow i


#------------------------------------------------------------------------
# pt=2000
#------------------------------------------------------------------------
pt=2000
set out 'optimal-pt'.pt.'.pdf'
set label 22 'p_t>'.pt.' GeV' at graph 0.03,0.96

x0=2.7
y0=6.4
dx=0.45
dy=0.4

set label 100 '{/*0.8 D@_2^{(2)}[l{/Symbol \304}l/l]'             center at x0+0*dx, y0-0*dy
set label 101 '{/*0.8 D@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+1*dx, y0-1*dy
set label 102 '{/*0.8 N@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+2*dx, y0-2*dy
set label 103 '{/*0.8 {/Symol t}@_{21}^{(2)}[t{/Symbol \304}l/t]' center at x0+3*dx, y0-3*dy
set label 104 '{/*0.8 D@_2^{(1)}[t{/Symbol \304}l/l]'             center at x0+4*dx, y0-4*dy
set label 105 '{/*0.8 {/Symol t}@_{21}^{(2)}[t{/Symbol \304}l/l]' center at x0+5*dx, y0-5*dy
set label 106 '{/*0.8 M@_2^{(2)}[t{/Symbol \304}l/t]'             center at x0+6*dx, y0-6*dy
set label 107 '{/*0.8 M@_2^{(2)}[trim]'                           center at x0+7*dx, y0-7*dy
set label 108 '{/*0.8 M@_2^{(2)}[l{/Symbol \304}l/l]'             center at x0+8*dx, y0-8*dy

x0=2.12
y0=6.38
dx=0.45
dy=0.41

set arrow 100 from 1.3971,  6.24331 to x0+ 0*dx, y0- 0*dy size 0.15,15 filled
set arrow 101 from 1.7356,  6.08782 to x0+ 1*dx, y0- 1*dy size 0.15,15 filled
set arrow 102 from 2.25504, 5.11219 to x0+ 2*dx, y0- 2*dy size 0.15,15 filled
set arrow 103 from 2.57466, 4.72884 to x0+ 3*dx, y0- 3*dy size 0.15,15 filled
set arrow 104 from 2.67913, 4.38069 to x0+ 4*dx, y0- 4*dy size 0.15,15 filled
set arrow 105 from 2.72306, 4.30354 to x0+ 5*dx, y0- 5*dy size 0.15,15 filled
set arrow 106 from 3.81088, 3.66542 to x0+ 6*dx, y0- 6*dy size 0.15,15 filled
set arrow 107 from 4.68965, 2.75525 to x0+ 7*dx+0.1, y0- 7*dy-0.1 size 0.15,15 filled
set arrow 108 from 6.24708, 2.52901 to x0+ 8*dx+0.5, y0- 8*dy-0.13 size 0.15,15 filled

plot mtruth(pt) i 1 u 2:3 w l         lc rgb "#aaaa00" lw 3 t 'optimal line',\
     mtruth(pt) i 0 u 2:3 w p pt 13 ps 0.7 lc 1 t 'all studied',\
     mtruth(pt).' | grep "^D2_beta1_trim"' u 2:3 w p pt  5 ps 1.0 lc rgb "#000000" t 'ATLAS-like',\
     mtruth(pt).' | grep "^N2_beta1_ttt"'  u 2:3 w p pt  9 ps 1.2 lc rgb "#00cc00" t 'CMS-like',\
     mtruth(pt).' | grep "^D2_beta2_tlt"'  u 2:3 w p pt  7 ps 1.0 lc rgb "#0000ff" t 'LHDD@_2^{(2)}'
# plot mtruth(pt) i 0 u 2:3 w p pt 7 ps 0.7 lc 1 t 'all studied',\
#      mtruth(pt) i 1 u 2:3 w l        lc 3 lw 3 t 'optimal line',\
#      mtruth(pt).' | grep "^D2_beta1_trim"' u 2:3 w p pt 13 ps 1.2 lc rgb "#000000" t 'ATLAS-like',\
#      mtruth(pt).' | grep "^N2_beta1_ttt"'  u 2:3 w p pt  5 ps 1.2 lc rgb "#00cc00" t 'CMS-like'

unset for [i=100:108] label i
unset for [i=100:108] arrow i

set out
