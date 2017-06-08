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
  

  //TODO: EnergyCorrelator _ecf1_beta1, _ecf2_beta1, _ecf3_beta1, _ecf1_beta2, _ecf2_beta2, _ecf3_beta2;

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
  _compute_one_level(jet,      _m_plain, _tau1_beta1_plain, _tau2_beta1_plain, _tau1_beta2_plain, _tau2_beta2_plain);
  _compute_one_level(sd_jet,   _m_loose, _tau1_beta1_loose, _tau2_beta1_loose, _tau1_beta2_loose, _tau2_beta2_loose);
  _compute_one_level(mmdt_jet, _m_tight, _tau1_beta1_tight, _tau2_beta1_tight, _tau1_beta2_tight, _tau2_beta2_tight);
  //TODO: add ecfs

  // trimmed quantities are sepatare
  _m_trim  = trimmed_jet.m();

}

void SubstructureVariables::_compute_one_level(const PseudoJet &jet,
                                               double &m,
                                               double &tau1_beta1, double &tau2_beta1,
                                               double &tau1_beta2, double &tau2_beta2){
  //TODO: add ecfs
  m = jet.m();
  tau1_beta1 = (*_tau1_beta1)(jet);
  tau2_beta1 = (*_tau2_beta1)(jet);
  tau1_beta2 = (*_tau1_beta2)(jet);
  tau2_beta2 = (*_tau2_beta2)(jet);
}

