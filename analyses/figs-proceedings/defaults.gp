# a set of defaults to be used across the plots

default_R="0.8"
default_Ratlas="0.8"
default_Rcms="0.8"
default_pt="1000"


atlas_tag='D2_beta1_trimmed'
atlas_label='ATLAS-like [D@_2^{(1)}(trim)]'
atlas_vmax=5.0
atlas_dv  =0.05
atlas_shape='D2_beta1_trimmed'
atlas_mass='trim'

cms_tag='N2_beta1_ttt'
cms_label='CMS-like [N@_2^{(1)}(t{/Symbol \304}t/t]]'
cms_vmax=0.5
cms_dv  =0.005
cms_shape='N2_beta1_tt'
cms_mass='tight'

perf_tag='D2_beta2_tlt'
perf_label='Dichroic [D@_2^{(2)}(t{/Symbol \304}l/t)]'
perf_vmax=5.0
perf_dv  =0.05
perf_shape='D2_beta2_lt'
perf_mass='tight'

resi_tag='M2_beta2_ttt'
resi_label='Resilient [M@_2^{(2)}(t{/Symbol \304}t/t)]'
resi_vmax=0.1
resi_dv  =0.001
resi_shape='M2_beta2_tt'
resi_mass='tight'

all_title="ATLAS-like CMS-like dichroic resilient"
all_tag  =sprintf("'%s' '%s' '%s' '%s'", atlas_tag  , cms_tag  , perf_tag  , resi_tag  )
all_label=sprintf("'%s' '%s' '%s' '%s'", atlas_label, cms_label, perf_label, resi_label)
all_vmax =sprintf("'%g' '%g' '%g' '%g'", atlas_vmax , cms_vmax , perf_vmax , resi_vmax )
all_dv   =sprintf("'%g' '%g' '%g' '%g'", atlas_dv   , cms_dv   , perf_dv   , resi_dv   )
all_shape=sprintf("'%s' '%s' '%s' '%s'", atlas_shape, cms_shape, perf_shape, resi_shape)
all_mass =sprintf("'%s' '%s' '%s' '%s'", atlas_mass , cms_mass , perf_mass , resi_mass )
all_ymax ="0.05 0.05 0.08 0.05"


default_levelref='truth'
default_levelalt='parton'
default_levelref_label='truth'
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

