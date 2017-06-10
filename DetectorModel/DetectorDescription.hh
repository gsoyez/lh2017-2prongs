// -*- c++ -*-
#ifndef DETECTORMODEL_DETECTORDESCRIPTION_H
#define DETECTORMODEL_DETECTORDESCRIPTION_H

#include "DetectorTags.h"
#include "DetectorGeometry.h"

#include "TowerGrid.h"

#include <string>

namespace Detector {
  ///@brief Detector descriptions
  namespace Description {
    ///@brief ATLAS experiment
    namespace ATLASExperiment {
      ///@name Detector idenfication and overall parameters
      ///@{
      static const std::string experimentName            = "ATLAS";             ///<@brief Name
      static const Detector::ExperimentTag experimentTag = Detector::ATLAS;     ///<@brief Tag 
      static const Detector::RecoMode recoMode           = Detector::CaloTower; ///<@brief Signal type
      static const double solenoidField                  = 2.0;    /* Tesla */  ///<@brief Solonoid field 
      static const double cavityRadius                   = 1150.;  /* mm */     ///<@brief Inner cavavity radius
      static const Detector::Geometry::Circle cavityEnvelope = Detector::Geometry::Circle(Detector::Geometry::Point2d(0.,0.),cavityRadius);
      ///@}
      ///@name Tracking performance and coverage
      ///@{
      static const double trkPtResoA                     = 5e-04;               ///<@brief @f$ p_{\rm T} @f$ resolution parameter @f$ a @f$ 
      static const double trkPtResoB                     = 0.01;                ///<@brief @f$ p_{\rm T} @f$ resolution parameter @f$ b @f$
      static const double trkPtMin                       = 0.5;    /* GeV */    ///<@brief Minimum reconstructed track @f$ p_{\rm T}^{\rm trk} > p_{\rm T,min} @f$
      static const double trkPtMax                       = 140.;   /* GeV */    ///<@brief Maximum reconstructed track @f$ p_{\rm T}^{\rm trk} < p_{\rm T,max} @f$
      static const double trkEtaMin                      = -2.5;                ///<@brief Tracking coverage @f$ \eta_{\rm trk} > \eta_{\rm min} @f$
      static const double trkEtaMax                      = 2.5;                 ///<@brief Tracking coverage @f$ \eta_{\rm trk} < \eta_{\rm max} @f$
      ///@}
      ///@name EMC performance, coverage and signal collection
      ///@{
      static const double emcResoA                       = 0.1;                 ///<@brief Energy resolution parameter @f$ a @f$ (stochastic term)
      static const double emcResoB                       = 0.;                  ///<@brief Energy resolution parameter @f$ b @f$ (noise term)
      static const double emcResoC                       = 0.007;               ///<@brief Energy resolution parameter @f$ c @f$ (constant term)
      static const double emcPtMin                       = 0.5;    /* GeV */    ///<@brief Minimum tower @f$ p_{\rm T} @f$ for EM particles
      static const double emcPtMax                       = 10000.; /* GeV */    ///<@brief Maximum tower @f$ p_{\rm T} @f$ for EM particles
      static const double emcEtaMin                      = -2.5;                ///<@brief EMC coverage lower limit
      static const double emcEtaMax                      = 2.5;                 ///<@brief EMC coverage upper limit
      static const int    emcEtaBin                      = 200;                 ///<@brief EMC tower granularity in @f$ \eta @f$
      static const int    emcPhiBin                      = 256;                 ///<@brief EMC tower granularity in @f$ \phi @f$
      static const TowerGrid emcTowers                   = TowerGrid(emcEtaBin,emcEtaMin,emcEtaMax,emcPhiBin); ///<@brief EMC tower grid
      ///@}
      ///@name HAC performance, coverage and signal collection
      ///@{
      static const double hacResoA                       = 0.5;                 ///<@brief Energy resolution parameter @f$ a @f$ (stochastic term)
      static const double hacResoB                       = 0.;                  ///<@brief Energy resolution parameter @f$ b @f$ (noise term)
      static const double hacResoC                       = 0.03;                ///<@brief Energy resolution parameter @f$ c @f$ (constant term)
      static const double hacPtMin                       = 0.5;    /* GeV */    ///<@brief Minimum tower @f$ p_{\rm T} @f$ for HAD particles
      static const double hacPtMax                       = 10000.; /* GeV */    ///<@brief Maximum tower @f$ p_{\rm T} @f$ for HAD particles
      static const double hacEtaMin                      = -4.9;                ///<@brief HAC coverage lower limit
      static const double hacEtaMax                      = 4.9;                 ///<@brief HAC coverage upper limit
      static const int    hacEtaBin                      = 98;                  ///<@brief HAC tower granularity in @f$ \eta @f$
      static const int    hacPhiBin                      = 64;                  ///<@brief HAC tower granularity in @f$ \phi @f$
      static const TowerGrid hacTowers   = TowerGrid(hacEtaBin,hacEtaMin,hacEtaMax,hacPhiBin);
      ///@}
      ///@name Muon spectrometer coverage and acceptance
      ///@{
      static const double muoPtMin                       = 1.0;    /* GeV */    ///<@brief Minimum muon @f$ p_{\rm T} @f%
      static const double muoPtMax                       = 2000.;  /* GeV */    ///<@brief Maximum muon @f$ p_{\rm T} @f%
      static const double muoEtaMin                      = -2.7;                ///<@brief Muon coverage lower limit
      static const double muoEtaMax                      = 2.7;                 ///<@brief Muon coverage upper limit
      ///@}
      ///@name Overall experimental acceptance 
      ///@{
      static const double expEtaMin                      = hacEtaMin;           ///<@brief Absolute acceptance lower bound
      static const double expEtaMax                      = hacEtaMax;           ///<@brief Absolute acceptance upper bound
      ///@}
    } // ATLAS
    ///@brief CMS experiment
    namespace CMSExperiment {
      ///@name Detector idenfication and overall parameters
      ///@{
      static const std::string experimentName            = "CMS";
      static const Detector::ExperimentTag experimentTag = Detector::CMS;
      static const Detector::RecoMode recoMode           = Detector::ParticleFlowExcl; 
      static const double solenoidField                  = 4.0;    /* Tesla */
      static const double cavityRadius                   = 1290.;  /* mm */
      static const Detector::Geometry::Circle cavityEnvelope = Detector::Geometry::Circle(Detector::Geometry::Point2d(0.,0.),cavityRadius);
      ///@}
      ///@name Tracking performance and coverage
      ///@{
      static const double trkPtResoA                     = 1.5e-04;
      static const double trkPtResoB                     = 0.005;
      static const double trkPtMin                       = 1.0;    /* GeV */
      static const double trkPtMax                       = 300.;   /* GeV */
      static const double trkEtaMin                      = -2.5;                ///<@brief Tracking coverage @f$ \eta_{\rm trk} > \eta_{\rm min} @f$
      static const double trkEtaMax                      = 2.5;                 ///<@brief Tracking coverage @f$ \eta_{\rm trk} < \eta_{\rm max} @f$
      ///@}
      ///@name EMC performance, coverage and signal collection
      ///@{
      static const double emcResoA       = 0.03;
      static const double emcResoB       = 0.;
      static const double emcResoC       = 0.005;
      static const double emcPtMin       = 0.5;    /* GeV */
      static const double emcPtMax       = 10000.; /* GeV */
      static const double emcEtaMin      = -2.5;
      static const double emcEtaMax      = 2.5;
      static const int    emcEtaBin      = 200;
      static const int    emcPhiBin      = 256;
      static const TowerGrid emcTowers   = TowerGrid(emcEtaBin,emcEtaMin,emcEtaMax,emcPhiBin);
      ///@}
      ///@name HAC performance, coverage and signal collection
      ///@{
      static const double hacResoA       = 1.0;
      static const double hacResoB       = 0.;
      static const double hacResoC       = 0.05;
      static const double hacPtMin       = 0.5;    /* GeV */
      static const double hacPtMax       = 10000.; /* GeV */
      static const double hacEtaMin      = -4.9;
      static const double hacEtaMax      = 4.9;
      static const int    hacEtaBin      = 98;
      static const int    hacPhiBin      = 64;
      static const TowerGrid hacTowers   = TowerGrid(hacEtaBin,hacEtaMin,hacEtaMax,hacPhiBin);
      ///@}
      ///@name Muon spectrometer coverage and acceptance
      ///@{
      static const double muoPtMin       = 1.0;    /* GeV */
      static const double muoPtMax       = 2000.;  /* GeV */
      static const double muoEtaMin      = -2.7;
      static const double muoEtaMax      = 2.7;
      ///@}
      ///@name Overall experimental acceptance 
      ///@{
      static const double expEtaMin      = hacEtaMin;
      static const double expEtaMax      = hacEtaMax;
      ///@} 
    } // CMS
  } // Description
} // Detector
#endif
