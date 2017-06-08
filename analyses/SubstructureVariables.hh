#ifndef __SUBSTRUCTURE_VARIABLES_HH__
#define __SUBSTRUCTURE_VARIABLES_HH__

#include <sstream>
#include <CmdLine.hh>
#include <fastjet/tools/Filter.hh>   // for trimming
#include <fastjet/tools/Recluster.hh>
#include <fastjet/contrib/ModifiedMassDropTagger.hh>
#include <fastjet/contrib/SoftDrop.hh>
#include <fastjet/contrib/Nsubjettiness.hh>
#include <fastjet/contrib/EnergyCorrelator.hh>

/// \class SubstructureVariables
/// A global tool to compute all the substructure observables we're
/// interested in
///
/// Usage:
///  1. initialise this object outside your event loop
///  2. in the event loop, call set_jet to compute everything (cached)
///  3. retrieve the computed values
///
/// Q: - do we really want to cache things?
///    - do we really want to compute everything by default?
///    - do we need to add support for PU subtraction??
///
/// Supported command-line options:
///   -trim.Rsub  [0.2]
///   -trim.fcut  [0.05]
///   -mmdt.zcut  [0.1]
///   -sd.beta    [2]
///   -sd.zcut    [0.05]
///
/// Internal note: the explicit variable name structure makes for
/// readibility but it would be much more compact to store that in a
/// map or an array
class SubstructureVariables{
public:
  enum groom {plain, loose, tight, trim};
  /// ctor (initialises all tools)
  SubstructureVariables(CmdLine &cmdline);

  // cache values for a given jet
  void set_jet(const fastjet::PseudoJet &jet);

  // get the results
  double m(groom g) const{ return _m_vals[g];}
  
  double tau1_beta1(groom g) const { return _tau1_beta1_vals[g];}
  double tau2_beta1(groom g) const { return _tau2_beta1_vals[g];}
  double tau1_beta2(groom g) const { return _tau1_beta2_vals[g];}
  double tau2_beta2(groom g) const { return _tau2_beta2_vals[g];}
  double tau21_beta1(groom g1, groom g2) const {
    return _tau2_beta1_vals[g1]/_tau2_beta1_vals[g2];}
  double tau21_beta1(groom g) const { return tau21_beta1(g,g);}
  double tau21_beta2(groom g1, groom g2) const {
    return _tau2_beta2_vals[g1]/_tau2_beta2_vals[g2];}
  double tau21_beta2(groom g) const { return tau21_beta2(g,g);}


  // ecfgs
  double ecfg_v1_N2_beta1(groom g) const {return _1ecf2_beta1_vals[g];}
  double ecfg_v1_N3_beta1(groom g) const {return _1ecf3_beta1_vals[g];}
  double ecfg_v2_N3_beta1(groom g) const {return _2ecf3_beta1_vals[g];}
  double ecfg_v3_N3_beta1(groom g) const {return _3ecf3_beta1_vals[g];}
  
  double ecfg_v1_N2_beta2(groom g) const {return _1ecf2_beta2_vals[g];}
  double ecfg_v1_N3_beta2(groom g) const {return _1ecf3_beta2_vals[g];}
  double ecfg_v2_N3_beta2(groom g) const {return _2ecf3_beta2_vals[g];}
  double ecfg_v3_N3_beta2(groom g) const {return _3ecf3_beta2_vals[g];}

  // ecfs based observables
  double U1_beta1(groom g) const {return _1ecf2_beta1_vals[g];}
  double U2_beta1(groom g) const {return _1ecf3_beta1_vals[g];}
  double N2_beta1(groom g1, groom g2) const {
    return _2ecf3_beta1_vals[g1] / (_1ecf2_beta1_vals[g2]*_1ecf2_beta1_vals[g2]);}
  double N2_beta1(groom g) const {return N2_beta1(g,g);}
  double M2_beta1(groom g1, groom g2) const {
    return _1ecf3_beta1_vals[g1] / _1ecf2_beta1_vals[g2];}
  double M2_beta1(groom g) const {return M2_beta1(g,g);}
  double D2_beta1(groom g1, groom g2) const {
    return _3ecf3_beta1_vals[g1]
      / (_1ecf2_beta1_vals[g2]*_1ecf2_beta1_vals[g2]*_1ecf2_beta1_vals[g2]);}
  double D2_beta1(groom g) const { return D2_beta1(g,g);}
  
  double U1_beta2(groom g) const {return _1ecf2_beta2_vals[g];}
  double U2_beta2(groom g) const {return _1ecf3_beta2_vals[g];}
  double N2_beta2(groom g1, groom g2) const {
    return _2ecf3_beta2_vals[g1] / (_1ecf2_beta2_vals[g2]*_1ecf2_beta2_vals[g2]);}
  double N2_beta2(groom g) const { return N2_beta2(g,g);}
  double M2_beta2(groom g1, groom g2) const {
    return _1ecf3_beta2_vals[g1] / _1ecf2_beta2_vals[g2];}
  double M2_beta2(groom g) const { return M2_beta2(g,g);}
  double D2_beta2(groom g1, groom g2) const {
    return _3ecf3_beta2_vals[g1]
      / (_1ecf2_beta2_vals[g2]*_1ecf2_beta2_vals[g2]*_1ecf2_beta2_vals[g2]);}
  double D2_beta2(groom g) const { return D2_beta2(g,g);}
  
protected:
  //----------------------------------------------------------------------
  // cached results
  double _m_vals[4];                   ///< masses
  
  // N-subjettiness
  double _tau1_beta1_vals[3]; ///< tau1(beta=1)
  double _tau2_beta1_vals[3]; ///< tau2(beta=1)
  double _tau1_beta2_vals[3]; ///< tau1(beta=2)
  double _tau2_beta2_vals[3]; ///< tau2(beta=2)

  // ecfgs
  double _1ecf2_beta1_vals[3]; ///< 1ecfg2(beta=1)
  double _1ecf3_beta1_vals[3]; ///< 1ecfg3(beta=1)
  double _2ecf3_beta1_vals[3]; ///< 2ecfg3(beta=1)
  double _3ecf3_beta1_vals[3]; ///< 3ecfg3(beta=1)

  double _1ecf2_beta2_vals[3]; ///< 1ecfg2(beta=2)
  double _1ecf3_beta2_vals[3]; ///< 1ecfg3(beta=2)
  double _2ecf3_beta2_vals[3]; ///< 2ecfg3(beta=2)
  double _3ecf3_beta2_vals[3]; ///< 3ecfg3(beta=2)
  
  //----------------------------------------------------------------------
  // substructure tools
  
  /// grooming tools (note that we're systematically using a shared ptr)
  fastjet::SharedPtr<fastjet::Filter> _trimmer;
  fastjet::SharedPtr<fastjet::contrib::ModifiedMassDropTagger> _mmdt;
  fastjet::SharedPtr<fastjet::contrib::SoftDrop> _sd;

  /// shape tools
  fastjet::SharedPtr<fastjet::contrib::Nsubjettiness> _tau1_beta1, _tau1_beta2,
    _tau2_beta1, _tau2_beta2;
  fastjet::SharedPtr<fastjet::contrib::EnergyCorrelatorGeneralized>
    _1ecf2_beta1, _1ecf3_beta1, _2ecf3_beta1, _3ecf3_beta1,
    _1ecf2_beta2, _1ecf3_beta2, _2ecf3_beta2, _3ecf3_beta2;

  /// underlying needs
  fastjet::SharedPtr<fastjet::Recluster> _recluster_ca;

  //----------------------------------------------------------------------
  // helpers
  void _compute_one_level(const fastjet::PseudoJet &jet,
                          double &m,
                          double &tau1_beta1, double &tau2_beta1,
                          double &tau1_beta2, double &tau2_beta2,
			  double &e12_beta1, double& e13_beta1,
			  double &e23_beta1, double& e33_beta1,
			  double &e12_beta2, double& e13_beta2,
			  double &e23_beta2, double& e33_beta2);

  std::ostringstream _description;
};



#endif //  __SUBSTRUCTURE_VARIABLES_HH__
