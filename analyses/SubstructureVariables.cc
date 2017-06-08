#include "SubstructureVariables.hh"


using namespace std;
using namespace fastjet;
using namespace fastjet::contrib;

//------------------------------------------------------------------------
// SubstructureVariables implementation
//------------------------------------------------------------------------

// ctor (initialises all tools)
SubstructureVariables::SubstructureVariables(CmdLine &cmdline){
  _description << "# Substructure variables configuration" << endl;

  // trimming
  double Rtrim = cmdline.value("-trim.Rsub", 0.2);
  double ftrim = cmdline.value("-trim.fcut", 0.05);
  _trimmer.reset(new Filter(JetDefinition(kt_algorithm, Rtrim), SelectorPtFractionMin(ftrim)));
  _description << "# Trimming: " << _trimmer->description() << endl;

  // mmdt
  double mmdt_zcut = cmdline.value("-mmdt.zcut", 0.1);
  _mmdt.reset(new ModifiedMassDropTagger(mmdt_zcut));
  _mmdt->set_grooming_mode(true); // avoids nasty side effects
  _description << "# mMDT: " << _mmdt->description() << endl;

  // SoftDrop
  double sd_beta = cmdline.value("-sd.beta", 2.0);
  double sd_zcut = cmdline.value("-sd.zcut", 0.05);
  double sd_R0   = cmdline.value("-R", 1.0); //TODO: make sure we have a decent/synchronised default R
  _sd.reset(new SoftDrop(sd_beta, sd_zcut, sd_R0));
  _description << "# SoftDrop: " << _sd->description() << endl;

  /// shape tools
  contrib::WTA_KT_Axes  axes_kt;
  contrib::GenKT_Axes   axes_gkt(0.5);
  contrib::UnnormalizedMeasure measure1(1);
  contrib::UnnormalizedMeasure measure2(2);
  _tau1_beta1.reset(new Nsubjettiness(1, axes_kt,  measure1));
  _tau1_beta2.reset(new Nsubjettiness(1, axes_gkt, measure2));
  _tau2_beta1.reset(new Nsubjettiness(2, axes_kt,  measure1));
  _tau2_beta2.reset(new Nsubjettiness(2, axes_gkt, measure2));
  
  _1ecf2_beta1.reset(new EnergyCorrelatorGeneralized(1, 2, 1.0));
  _1ecf3_beta1.reset(new EnergyCorrelatorGeneralized(1, 3, 1.0));
  _2ecf3_beta1.reset(new EnergyCorrelatorGeneralized(2, 3, 1.0));
  _3ecf3_beta1.reset(new EnergyCorrelatorGeneralized(3, 3, 1.0));

  _1ecf2_beta2.reset(new EnergyCorrelatorGeneralized(1, 2, 2.0));
  _1ecf3_beta2.reset(new EnergyCorrelatorGeneralized(1, 3, 2.0));
  _2ecf3_beta2.reset(new EnergyCorrelatorGeneralized(2, 3, 2.0));
  _3ecf3_beta2.reset(new EnergyCorrelatorGeneralized(3, 3, 2.0));

  // additional tools:
  // mMDT andSD will need a C/A reclustering
  // TODO: decide whether or not we need to inherit the recombiner?
  _recluster_ca.reset(new Recluster(JetDefinition(cambridge_algorithm, JetDefinition::max_allowable_R), true));
  


}

//------------------------------------------------------------------------
void SubstructureVariables::set_jet(const PseudoJet &jet){
  // C/A reclustering (avoids doing it twice, once for mmdt, one for sd)
  PseudoJet ca_jet = (*_recluster_ca)(jet);
  
  // apply the various groomers
  PseudoJet trimmed_jet = (*_trimmer)(jet);
  PseudoJet sd_jet      = (*_sd)(ca_jet);
  PseudoJet mmdt_jet    = (*_mmdt)(ca_jet);

  // all quantities for the plain jet
  _compute_one_level(jet,      _m_plain,
		     _tau1_beta1_plain, _tau2_beta1_plain,
		     _tau1_beta2_plain, _tau2_beta2_plain,
		     _1ecf2_beta1_plain, _1ecf3_beta1_plain,
		     _2ecf3_beta1_plain, _3ecf3_beta1_plain,
		     _1ecf2_beta2_plain, _1ecf3_beta2_plain,
		     _2ecf3_beta2_plain, _3ecf3_beta2_plain);
  _compute_one_level(sd_jet,   _m_loose,
		     _tau1_beta1_loose, _tau2_beta1_loose,
		     _tau1_beta2_loose, _tau2_beta2_loose,
		     _1ecf2_beta1_loose, _1ecf3_beta1_loose,
		     _2ecf3_beta1_loose, _3ecf3_beta1_loose,
		     _1ecf2_beta2_loose, _1ecf3_beta2_loose,
		     _2ecf3_beta2_loose, _3ecf3_beta2_loose);
  _compute_one_level(mmdt_jet, _m_tight,
		     _tau1_beta1_tight, _tau2_beta1_tight,
		     _tau1_beta2_tight, _tau2_beta2_tight,
		     _1ecf2_beta1_tight, _1ecf3_beta1_tight,
		     _2ecf3_beta1_tight, _3ecf3_beta1_tight,
		     _1ecf2_beta2_tight, _1ecf3_beta2_tight,
		     _2ecf3_beta2_tight, _3ecf3_beta2_tight);

  // trimmed quantities are sepatare
  _m_trim  = trimmed_jet.m();

}

void SubstructureVariables::_compute_one_level(const PseudoJet &jet,
                                               double &m,
                                               double &tau1_beta1, double &tau2_beta1,
                                               double &tau1_beta2, double &tau2_beta2,
					       double &e12_beta1, double& e13_beta1,
					       double &e23_beta1, double& e33_beta1,
					       double &e12_beta2, double& e13_beta2,
					       double &e23_beta2, double& e33_beta2){
  m = jet.m();
  tau1_beta1 = (*_tau1_beta1)(jet);
  tau2_beta1 = (*_tau2_beta1)(jet);
  tau1_beta2 = (*_tau1_beta2)(jet);
  tau2_beta2 = (*_tau2_beta2)(jet);
  // ecfgs
  e12_beta1 = (*_1ecf2_beta1)(jet);
  e13_beta1 = (*_1ecf3_beta1)(jet);
  e23_beta1 = (*_2ecf3_beta1)(jet);
  e33_beta1 = (*_3ecf3_beta1)(jet);
  e12_beta2 = (*_1ecf2_beta2)(jet);
  e13_beta2 = (*_1ecf3_beta2)(jet);
  e23_beta2 = (*_2ecf3_beta2)(jet);
  e33_beta2 = (*_3ecf3_beta2)(jet);
}

