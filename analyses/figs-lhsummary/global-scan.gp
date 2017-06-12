reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic
set out 'global-scan.pdf'

call 'defaults.gp'

eS=0.4

R=default_R
levelref=default_levelref
levelalt=default_levelalt
level=levelref.'_'.levelalt

reflabel=default_levelref_label
altlabel=default_levelalt_label

set xlabel 'resilience'
set xrange [0:5]

set ylabel 'performance'
set yrange [0:7]

set grid dt 3 lw 1

set label 102 '{/*0.8 Pythia8(4C), anti-k_t('.R.')}' right at graph 0.97,0.96
set label 103 '{/*0.8 60<m<100}'                     right at graph 0.97,0.92
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)  right at graph 0.97,0.88

#          +   x   *   sqr     o       tr     dia
groomers="ppp lll lpl lpp ttt tlt tpt tll tpl tpp trimmed"
#labs='"tight{/Symbol \304}loose/tight" "loose{/Symbol \304}loose/loose" "tight{/Symbol \304}tight/tight" "trim{/Symbol \304}trim/trim" "tight{/Symbol \304}plain/plain"'

shapes="tau21_beta1 tau21_beta2 N2_beta1 N2_beta2 D2_beta1 D2_beta2 M2_beta1 M2_beta2"
#slabs='"{/Symbol t}_{21}" "N_2" "D_2" "M_2"'

m(shape)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.res',shape)

resilience ="1.0/sqrt(column('Bcorrection_'.level)**2+column('Scorrection_'.level)**2)"
performance="column('significance_'.levelref)"

set macros

mycolour(i)=(i==0) ? 'rgb "#ff0000"' : ((i==1) ? 'rgb "#00dd00"' : ((i==2) ? 'rgb "#0000ff"' : 'rgb "#ffffff"'))
mydt(i)=(i-1)%2
mypoint(ig)=(ig<=9) ? ig : ig+2

set style line 1 dt 2 lc rgb "#ff0000" lw 2
set style line 2 dt 1 lc rgb "#ff0000" lw 2
set style line 3 dt 2 lc rgb "#000000" lw 2
set style line 4 dt 1 lc rgb "#000000" lw 2
set style line 5 dt 2 lc rgb "#0000ff" lw 2
set style line 6 dt 1 lc rgb "#0000ff" lw 2
set style line 7 dt 2 lc rgb "#00dd00" lw 2
set style line 8 dt 1 lc rgb "#00dd00" lw 2

set title reflabel.' v. '.altlabel 

plot for [is=1:words(shapes)] for [ig=1:words(groomers)] \
     m(word(shapes,is).'_'.word(groomers,ig)) u (@resilience):(@performance):((0.001*$2)**0.5*0.6) \
     w linesp ls is pt mypoint(ig) ps variable not


# examples of performant but not too resilient :
#   solid-blue crosses         i.e. D2,beta=2 with lll
#   solid-blue filled-diamonds i.e. D2,beta=2 with trimmed
#   dashed-black stars         i.e. N2,beta=1 with lpl
# example of resilient  but not too performant:
#   solid-green empty squares  i.e. M2,beta=2 with ttt

set out
