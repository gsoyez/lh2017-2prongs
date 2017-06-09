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
///   -shape.beta [1]
///
/// Internal note: the explicit variable name structure makes for
/// readibility but it would be much more compact to store that in a
/// map or an array
class SubstructureVariables{
public:
  enum groom {loose, tight, plain, trim};
  enum beta {beta1, beta2};
  /// ctor (initialises all tools)
  SubstructureVariables(CmdLine &cmdline);

  // cache values for a given jet
  void set_jet(const fastjet::PseudoJet &jet);

  // get the results
  double m(groom g) const{ return _m_vals[g];}
  
  double tau1(groom g, beta b) const { return _tau1_vals[g][b];}
  double tau2(groom g, beta b) const { return _tau2_vals[g][b];}
  double tau21(groom g1, groom g2, beta b) const {
    return _tau2_vals[b][g1]/_tau2_vals[b][g2];}
  double tau21(groom g, beta b) const { return tau21(g,g,b);}

  // ecfgs
  double ecfg_v1_N2(groom g, beta b) const {return _1ecf2_vals[g][b];}
  double ecfg_v1_N3(groom g, beta b) const {return _1ecf3_vals[g][b];}
  double ecfg_v2_N3(groom g, beta b) const {return _2ecf3_vals[g][b];}
  double ecfg_v3_N3(groom g, beta b) const {return _3ecf3_vals[g][b];}

  // groomed energy fraction, angle
  double zg(groom g) const {return _zg_vals[g];}
  double thetag(groom g) const {return _thetag_vals[g];}
  
  // ecfs based observables
  double U1(groom g, beta b) const {return _1ecf2_vals[g][b];}
  double U2(groom g, beta b) const {return _1ecf3_vals[g][b];}
  double N2(groom g1, groom g2, beta b) const {
    return _2ecf3_vals[g2][b] / (_1ecf2_vals[g2][b]*_1ecf2_vals[g2][b]);}
  double N2(groom g, beta b) const {return N2(g,g,b);}
  double M2(groom g1, groom g2, beta b) const {
    return _1ecf3_vals[g2][b] / _1ecf2_vals[g2][b];}
  double M2(groom g, beta b) const {return M2(g,g,b);}
  double D2(groom g1, groom g2, beta b) const {
    return _3ecf3_vals[g2][b]
      / (_1ecf2_vals[g2][b]*_1ecf2_vals[g2][b]*_1ecf2_vals[g2][b]);}
  double D2(groom g, beta b) const { return D2(g,g,b);}
  
protected:
  //----------------------------------------------------------------------
  // cached results
  double _m_vals[4];                   ///< masses
  
  // N-subjettiness
  double _tau1_vals[3][2]; ///< tau1(beta=1,2)
  double _tau2_vals[3][2]; ///< tau2(beta=1,2)

  // ecfgs
  double _1ecf2_vals[3][2]; ///< 1ecfg2(beta=1,2)
  double _1ecf3_vals[3][2]; ///< 1ecfg3(beta=1,2)
  double _2ecf3_vals[3][2]; ///< 2ecfg3(beta=1,2)
  double _3ecf3_vals[3][2]; ///< 3ecfg3(beta=1,2)

  // z_g and theta_g
  double _zg_vals[2];
  double _thetag_vals[2];
  
  //----------------------------------------------------------------------
  // substructure tools
  
  /// grooming tools (note that we're systematically using a shared ptr)
  fastjet::SharedPtr<fastjet::Filter> _trimmer;
  fastjet::SharedPtr<fastjet::contrib::ModifiedMassDropTagger> _mmdt;
  fastjet::SharedPtr<fastjet::contrib::SoftDrop> _sd;

  /// shape tools
  fastjet::SharedPtr<fastjet::contrib::Nsubjettiness> _tau1[2], _tau2[2];
  fastjet::SharedPtr<fastjet::contrib::EnergyCorrelatorGeneralized>
    _1ecf2[2], _1ecf3[2], _2ecf3[2], _3ecf3[2];

  /// underlying needs
  fastjet::SharedPtr<fastjet::Recluster> _recluster_ca;

  //----------------------------------------------------------------------
  // helpers
  void _compute_one_level(const fastjet::PseudoJet &jet, 
                          double (&tau1)[2], double (&tau2)[2],
			  double (&e12)[2], double (&e13)[2],
			  double (&e23)[2], double (&e33)[2]);

  std::ostringstream _description;
};



#endif //  __SUBSTRUCTURE_VARIABLES_HH__
