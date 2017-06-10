// -*- c++ -*-
#ifndef DETECTORMODEL_DETECTORTAGS_H
#define DETECTORMODEL_DETECTORTAGS_H

namespace Detector {
  enum ExperimentTag      { ATLAS       = 0x01, CMS       = 0x02, Generic          = 0x04,                                                   UnknownExperiment = 0x00 };
  enum DetectorTag        { Calorimeter = 0x10, Tracking  = 0x01, EmCalorimeter    = 0x11, HadCalorimeter   = 0x12, MuonSpectrometer = 0x02, UnknownDetector = 0x00   };
  enum RecoMode           { CaloTower   = 0x01, RecoTrack = 0x02, ParticleFlowExcl = 0x04, ParticleFlowIncl = 0x03, RecoMuon = 0x08,         UnknownMode = 0x00       };
}
#endif
