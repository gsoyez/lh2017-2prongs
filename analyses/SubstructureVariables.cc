#include "SubstructureVariables.hh"


using namespace std;
using namespace fastjet;
using namespace fastjet::contrib;

//------------------------------------------------------------------------
// SubstructureVariables implementation
//------------------------------------------------------------------------

// ctor (initialises all tools)
SubstructureVariables::SubstructureVariables(CmdLine &cmdline) {
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
  _tau1[beta1].reset(new Nsubjettiness(1, axes_kt,  measure1));
  _tau1[beta2].reset(new Nsubjettiness(2, axes_kt,  measure1));
  _tau2[beta1].reset(new Nsubjettiness(1, axes_gkt, measure2));
  _tau2[beta2].reset(new Nsubjettiness(2, axes_gkt, measure2));
  _1ecf2[beta1].reset(new EnergyCorrelatorGeneralized(1, 2, 1));
  _1ecf3[beta1].reset(new EnergyCorrelatorGeneralized(1, 3, 1));
  _2ecf3[beta1].reset(new EnergyCorrelatorGeneralized(2, 3, 1));
  _3ecf3[beta1].reset(new EnergyCorrelatorGeneralized(3, 3, 1));
  _1ecf2[beta2].reset(new EnergyCorrelatorGeneralized(1, 2, 2));
  _1ecf3[beta2].reset(new EnergyCorrelatorGeneralized(1, 3, 2));
  _2ecf3[beta2].reset(new EnergyCorrelatorGeneralized(2, 3, 2));
  _3ecf3[beta2].reset(new EnergyCorrelatorGeneralized(3, 3, 2));

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
  _compute_one_level(jet, jet.pt(), _tau1_vals[plain], _tau2_vals[plain],
		     _1ecf2_vals[plain], _1ecf3_vals[plain],
		     _2ecf3_vals[plain], _3ecf3_vals[plain]);
  _compute_one_level(sd_jet, jet.pt(), _tau1_vals[loose], _tau2_vals[loose],
		     _1ecf2_vals[loose], _1ecf3_vals[loose],
		     _2ecf3_vals[loose], _3ecf3_vals[loose]);
  _compute_one_level(mmdt_jet, jet.pt(), _tau1_vals[tight], _tau2_vals[tight],
		     _1ecf2_vals[tight], _1ecf3_vals[tight],
		     _2ecf3_vals[tight], _3ecf3_vals[tight]);

  // mass quantities and groomed zg, thetag are separate
  _zg_vals[loose] = sd_jet.structure_of<contrib::SoftDrop>().symmetry();
  _zg_vals[tight] = mmdt_jet.structure_of<contrib::SoftDrop>().symmetry();
  _thetag_vals[loose] = sd_jet.structure_of<contrib::SoftDrop>().delta_R();
  _thetag_vals[tight] = mmdt_jet.structure_of<contrib::SoftDrop>().delta_R();
  _m_vals[plain] = jet.m();
  _m_vals[loose] = sd_jet.m();
  _m_vals[tight] = mmdt_jet.m();
  _m_vals[trim]  = trimmed_jet.m();

}

void SubstructureVariables::_compute_one_level(const PseudoJet &jet, double ungroomed_pt,
                                               double (&tau1)[2], double (&tau2)[2],
					       double (&e12)[2], double (&e13)[2],
					       double (&e23)[2], double (&e33)[2]){
  tau1[beta1] = (*(_tau1[beta1]))(jet);
  tau2[beta1] = (*(_tau2[beta1]))(jet);
  
  tau1[beta2] = (*(_tau1[beta2]))(jet);
  tau2[beta2] = (*(_tau2[beta2]))(jet);
  
  // ecfgs
  double ungroomed_pt2 = ungroomed_pt*ungroomed_pt;
  double ungroomed_pt3 = ungroomed_pt2*ungroomed_pt;
  e12[beta1] = (*(_1ecf2[beta1]))(jet)*ungroomed_pt2;
  e13[beta1] = (*(_1ecf3[beta1]))(jet)*ungroomed_pt3;
  e23[beta1] = (*(_2ecf3[beta1]))(jet)*ungroomed_pt3;
  e33[beta1] = (*(_3ecf3[beta1]))(jet)*ungroomed_pt3;

  e12[beta2] = (*(_1ecf2[beta2]))(jet)*ungroomed_pt2;
  e13[beta2] = (*(_1ecf3[beta2]))(jet)*ungroomed_pt3;
  e23[beta2] = (*(_2ecf3[beta2]))(jet)*ungroomed_pt3;
  e33[beta2] = (*(_3ecf3[beta2]))(jet)*ungroomed_pt3;
}

