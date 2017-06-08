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
  /// ctor (initialises all tools)
  SubstructureVariables(CmdLine &cmdline);

  // cache values for a given jet
  void set_jet(const fastjet::PseudoJet &jet);

  // get the results
  double m_plain() const{ return _m_plain;}
  double m_loose() const{ return _m_loose;}
  double m_tight() const{ return _m_tight;}
  double m_trim () const{ return _m_trim ;}
  
  double tau1_beta1_plain () const { return _tau1_beta1_plain;}
  double tau1_beta1_loose () const { return _tau1_beta1_loose;}
  double tau1_beta1_tight () const { return _tau1_beta1_tight;}
  double tau2_beta1_plain () const { return _tau2_beta1_plain;}
  double tau2_beta1_loose () const { return _tau2_beta1_loose;}
  double tau2_beta1_tight () const { return _tau2_beta1_tight;}
  double tau21_beta1_plain() const { return _tau2_beta1_plain/_tau2_beta1_plain;}
  double tau21_beta1_loose() const { return _tau2_beta1_loose/_tau2_beta1_loose;}
  double tau21_beta1_tight() const { return _tau2_beta1_tight/_tau2_beta1_tight;}
  
  double tau1_beta2_plain () const { return _tau1_beta2_plain;}
  double tau1_beta2_loose () const { return _tau1_beta2_loose;}
  double tau1_beta2_tight () const { return _tau1_beta2_tight;}
  double tau2_beta2_plain () const { return _tau2_beta2_plain;}
  double tau2_beta2_loose () const { return _tau2_beta2_loose;}
  double tau2_beta2_tight () const { return _tau2_beta2_tight;}
  double tau21_beta2_plain() const { return _tau2_beta2_plain/_tau2_beta2_plain;}
  double tau21_beta2_loose() const { return _tau2_beta2_loose/_tau2_beta2_loose;}
  double tau21_beta2_tight() const { return _tau2_beta2_tight/_tau2_beta2_tight;}

  //TODO: add ecfs

protected:
  //----------------------------------------------------------------------
  // cached results 
  double _m_plain, _m_loose, _m_tight, _m_trim;                   ///< masses
  double _tau1_beta1_plain, _tau1_beta1_loose, _tau1_beta1_tight; ///< tau1(beta=1)
  double _tau2_beta1_plain, _tau2_beta1_loose, _tau2_beta1_tight; ///< tau2(beta=1)
  double _tau1_beta2_plain, _tau1_beta2_loose, _tau1_beta2_tight; ///< tau1(beta=2)
  double _tau2_beta2_plain, _tau2_beta2_loose, _tau2_beta2_tight; ///< tau2(beta=2)
  //TODO: add ecfs

  //----------------------------------------------------------------------
  // substructure tools
  
  /// grooming tools (note that we're systematically using a shared ptr)
  fastjet::SharedPtr<fastjet::Filter> _trimmer;
  fastjet::SharedPtr<fastjet::contrib::ModifiedMassDropTagger> _mmdt;
  fastjet::SharedPtr<fastjet::contrib::SoftDrop> _sd;

  /// shape tools
  fastjet::SharedPtr<fastjet::contrib::Nsubjettiness> _tau1_beta1, _tau1_beta2, _tau2_beta1, _tau2_beta2;
  fastjet::SharedPtr<fastjet::contrib::EnergyCorrelator> _ecf1_beta1, _ecf2_beta1, _ecf3_beta1, _ecf1_beta2, _ecf2_beta2, _ecf3_beta2;

  /// underlying needs
  fastjet::SharedPtr<fastjet::Recluster> _recluster_ca;

  //----------------------------------------------------------------------
  // helpers
  void _compute_one_level(const fastjet::PseudoJet &jet,
                          double &m,
                          double &tau1_beta1, double &tau2_beta1,
                          double &tau1_beta2, double &tau2_beta2);
  //TODO: add ecfs

  std::ostringstream _description;
};



#endif //  __SUBSTRUCTURE_VARIABLES_HH__
