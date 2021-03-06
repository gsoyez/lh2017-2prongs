reset
set term pdfcairo enhanced size 9cm,9cm
set colors classic

call 'defaults.gp'

eS=0.4

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

#          +   x   *   sqr     o       tr     dia
groomers="ppp lpl lll lpp tpp tpt tlt tll ttt tpl trimmed"
shapes="D2_beta1 D2_beta2 N2_beta1 N2_beta2 tau21_beta1 tau21_beta2 M2_beta1 M2_beta2"

# x = (a-b)/b = a/b -1 => a/b = x+1
# ---> y = 2 (a-b)/(a+b) = 2x/(x+2)
resilience ="1.0/sqrt((2.0*column('Bcorrection_'.level)/(column('Bcorrection_'.level)+2.0))**2+(2*column('Scorrection_'.level)/(column('Scorrection_'.level)+2.0))**2)"
#resilience ="1.0/sqrt(column('Bcorrection_'.level)**2+column('Scorrection_'.level)**2)"
performance="column('significance_'.levelref)"

set macros

mycolour(i)=(i==0) ? 'rgb "#ff0000"' : ((i==1) ? 'rgb "#00dd00"' : ((i==2) ? 'rgb "#0000ff"' : 'rgb "#ffffff"'))
mydt(i)=(i-1)%2
mypoint(ig)=(ig<=9) ? ig : ig+2


set style line 1 dt 1 lc rgb "#000000" lw 2 pt 12 # D2
set style line 2 dt 2 lc rgb "#000000" lw 2 pt 13 # D2
set style line 3 dt 1 lc rgb "#00cc00" lw 2 pt 4  # N2
set style line 4 dt 2 lc rgb "#00cc00" lw 2 pt 5  # N2
set style line 5 dt 1 lc rgb "#0000ff" lw 2 pt 6  # tau21
set style line 6 dt 2 lc rgb "#0000ff" lw 2 pt 7  # tau21
set style line 7 dt 1 lc rgb "#ff0000" lw 2 pt 8  # M2
set style line 8 dt 2 lc rgb "#ff0000" lw 2 pt 9  # M2

set title reflabel.' v. '.altlabel 

set out 'global-scan.pdf'

set label 103 '{/*0.8 60<m<100}'                     right at graph 0.97,0.92
set label 101 sprintf('{/*0.8 {/Symbol e}_S=%g',eS)  right at graph 0.97,0.88

m(shape,R)=sprintf('< grep -m1 "^#columns:" ../quality-measures/qualities-R'.R.'.res | sed "s/#columns: //"; grep %s ../quality-measures/qualities-R'.R.'.res',shape)

set label 102 '{/*0.8 Pythia8(4C), anti-k_t(0.8 or 1.0)}' right at graph 0.97,0.96
plot for [is=1:words(shapes)] for [ig=1:words(groomers)] \
     m(word(shapes,is).'_'.word(groomers,ig),'0.8') u (@resilience):(@performance):((0.001*$2)**0.5*0.6) \
     w linesp ls is pt mypoint(ig) ps variable not,\
     for [is=1:words(shapes)] for [ig=1:words(groomers)] \
     m(word(shapes,is).'_'.word(groomers,ig),'1.0') u (@resilience):(@performance):((0.001*$2)**0.5*0.6) \
     w linesp ls is pt mypoint(ig) ps variable not

set label 102 '{/*0.8 Pythia8(4C), anti-k_t(0.8)}' right at graph 0.97,0.96
plot for [is=1:words(shapes)] for [ig=1:words(groomers)] \
     m(word(shapes,is).'_'.word(groomers,ig),'0.8') u (@resilience):(@performance):((0.001*$2)**0.5*0.6) \
     w linesp ls is pt mypoint(ig) ps variable not

set label 102 '{/*0.8 Pythia8(4C), anti-k_t(1)}' right at graph 0.97,0.96
plot for [is=1:words(shapes)] for [ig=1:words(groomers)] \
     m(word(shapes,is).'_'.word(groomers,ig),'1.0') u (@resilience):(@performance):((0.001*$2)**0.5*0.6) \
     w linesp ls is pt mypoint(ig) ps variable not




# examples of performant but not too resilient :
#   solid-blue crosses         i.e. D2,beta=2 with lll
#   solid-blue filled-diamonds i.e. D2,beta=2 with trimmed
#   dashed-black stars         i.e. N2,beta=1 with lpl
# example of resilient  but not too performant:
#   solid-green empty squares  i.e. M2,beta=2 with ttt

set out
