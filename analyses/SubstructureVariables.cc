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
  double shape_beta = cmdline.value("-shape.beta", 1.0);
  contrib::WTA_KT_Axes  axes_kt;
  contrib::GenKT_Axes   axes_gkt(0.5);
  contrib::UnnormalizedMeasure measure1(1);
  contrib::UnnormalizedMeasure measure2(2);
  _tau1.reset(new Nsubjettiness(shape_beta, axes_kt,  measure1));
  _tau1.reset(new Nsubjettiness(shape_beta, axes_gkt, measure2));
  _1ecf2.reset(new EnergyCorrelatorGeneralized(1, 2, shape_beta));
  _1ecf3.reset(new EnergyCorrelatorGeneralized(1, 3, shape_beta));
  _2ecf3.reset(new EnergyCorrelatorGeneralized(2, 3, shape_beta));
  _3ecf3.reset(new EnergyCorrelatorGeneralized(3, 3, shape_beta));

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
  _compute_one_level(jet,      _m_vals[plain],
		     _tau1_vals[plain], _tau2_vals[plain],
		     _1ecf2_vals[plain], _1ecf3_vals[plain],
		     _2ecf3_vals[plain], _3ecf3_vals[plain]);
  _compute_one_level(sd_jet,   _m_vals[loose],
		     _tau1_vals[loose], _tau2_vals[loose],
		     _1ecf2_vals[loose], _1ecf3_vals[loose],
		     _2ecf3_vals[loose], _3ecf3_vals[loose]);
  _compute_one_level(mmdt_jet, _m_vals[tight],
		     _tau1_vals[tight], _tau2_vals[tight],
		     _1ecf2_vals[tight], _1ecf3_vals[tight],
		     _2ecf3_vals[tight], _3ecf3_vals[tight]);

  // trimmed quantities are sepatare
  _m_vals[trim]  = trimmed_jet.m();

}

void SubstructureVariables::_compute_one_level(const PseudoJet &jet,
                                               double &m,
                                               double &tau1, double &tau2,
					       double &e12, double& e13,
					       double &e23, double& e33){
  m = jet.m();
  tau1 = (*_tau1)(jet);
  tau2 = (*_tau2)(jet);
  // ecfgs
  e12 = (*_1ecf2)(jet);
  e13 = (*_1ecf3)(jet);
  e23 = (*_2ecf3)(jet);
  e33 = (*_3ecf3)(jet);
}

