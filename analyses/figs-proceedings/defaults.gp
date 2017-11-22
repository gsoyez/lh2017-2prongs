# a set of defaults to be used across the plots

default_R="1.0"
default_Ratlas="1.0"
default_Rcms="0.8"
default_pt="1000"

resi_tag='M2_beta2_ttt'
resi_label='M@_2^{(2)}[ttt]'
resi_vmax=0.1
resi_dv  =0.001
resi_shape='M2_beta2_tt'
resi_mass='tight'

perf_tag='D2_beta2_tlt'
perf_label='D@_2^{(2)}[tlt]'
perf_vmax=5.0
perf_dv  =0.05
perf_shape='D2_beta2_lt'
perf_mass='tight'

atlas_tag='D2_beta1_trimmed'
atlas_label='D@_2^{(1)}[trim]'
atlas_vmax=5.0
atlas_dv  =0.05
atlas_shape='D2_beta1_trimmed'
atlas_mass='trim'

cms_tag='N2_beta1_ttt'
cms_label='N@_2^{(1)}[tt]'
cms_vmax=0.5
cms_dv  =0.005
cms_shape='N2_beta1_tt'
cms_mass='tight'

default_levelref='truth'
default_levelalt='parton'
default_levelref_label='particle'
default_levelalt_label='parton'

default_Sref=0.4
default_mmin=65.0
default_mmax=105.0

# implementation of our quality figures
#
# performance: just the signal significance for the reference level
performance(Sref, Bref, Salt, Balt)=(Bref>0) ? Sref/sqrt(Bref) : 1/0
#
# resilience: essentially the inverse relative distance moved on a lin-lin efficiency plot
resilience(Sref, Bref, Salt, Balt)=0.5/sqrt(((Sref-Salt)/(Sref+Salt))**2+((Bref-Balt)/(Bref+Balt))**2)

# alternative: use a lin-log distance
#resilience(Sref, Bref, Salt, Balt)=1.0/sqrt((Sref-Salt)**2+log(Balt/Bref)**2)

