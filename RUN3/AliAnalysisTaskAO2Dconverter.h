/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskAO2Dconverter_H
#define AliAnalysisTaskAO2Dconverter_H

#include "AliAnalysisFilter.h"
#include "AliAnalysisTaskSE.h"
#include "AliEventCuts.h"

#include <TString.h>

#include "TClass.h"

#include <Rtypes.h>

class AliESDEvent;

class AliAnalysisTaskAO2Dconverter : public AliAnalysisTaskSE
{
public:
  AliAnalysisTaskAO2Dconverter() = default;
  AliAnalysisTaskAO2Dconverter(const char *name);
  virtual ~AliAnalysisTaskAO2Dconverter();

  AliAnalysisTaskAO2Dconverter(const AliAnalysisTaskAO2Dconverter &) = default;
  AliAnalysisTaskAO2Dconverter &operator=(const AliAnalysisTaskAO2Dconverter &) = delete;

  void SetUseEventCuts(Bool_t useEventCuts=kTRUE) { fUseEventCuts = useEventCuts;}
  Bool_t GetUseEventCuts() const {return fUseEventCuts;}

  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *option);

  void SetNumberOfEventsPerCluster(int n) { fNumberOfEventsPerCluster = n; }

  static AliAnalysisTaskAO2Dconverter* AddTask(TString suffix = "");
  enum TreeIndex { // Index of the output trees
    kEvents = 0,
    kTracks,
    kCalo,
    kCaloTrigger,
    kMuon,
    kMuonCls,
    kZdc,
    kVzero,
    kV0s,
    kCascades,
    kTOF,
    kKinematics,
    kTrees
  };
  enum TaskModes { // Flag for the task operation mode
    kStandard = 0,
    kMC
  };
  enum MCGeneratorID { // Generator type
    kAliGenEventHeader = 0,
    kAliGenCocktailEventHeader,
    kAliGenDPMjetEventHeader,
    kAliGenEpos3EventHeader,
    kAliGenEposEventHeader,
    kAliGenEventHeaderTunedPbPb,
    kAliGenGeVSimEventHeader,
    kAliGenHepMCEventHeader,
    kAliGenHerwigEventHeader,
    kAliGenHijingEventHeader,
    kAliGenPythiaEventHeader,
    kAliGenToyEventHeader,
    kGenerators
  };
  static const TClass* Generator[kGenerators]; // Generators

  TTree* CreateTree(TreeIndex t);
  void PostTree(TreeIndex t);
  void EnableTree(TreeIndex t) { fTreeStatus[t] = kTRUE; };
  void DisableTree(TreeIndex t) { fTreeStatus[t] = kFALSE; };
  static const TString TreeName[kTrees];  //! Names of the TTree containers
  static const TString TreeTitle[kTrees]; //! Titles of the TTree containers

  void Prune(TString p) { fPruneList = p; }; // Setter of the pruning list
  void SetMCMode() { fTaskMode = kMC; };     // Setter of the MC running mode

  AliAnalysisFilter fTrackFilter; // Standard track filter object
private:
  Bool_t fUseEventCuts = kFALSE;         //! Use or not event cuts
  AliEventCuts fEventCuts;      //! Standard event cuts
  AliESDEvent *fESD = nullptr;  //! input event
  TList *fOutputList = nullptr; //! output list

  // Output TTree
  TTree* fTree[kTrees] = { nullptr }; //! Array with all the output trees
  void Prune();                       // Function to perform tree pruning
  void FillTree(TreeIndex t);         // Function to fill the trees (only the active ones)

  // Task configuration variables
  TString fPruneList = "";                // Names of the branches that will not be saved to output file
  Bool_t fTreeStatus[kTrees] = { kTRUE }; // Status of the trees i.e. kTRUE (enabled) or kFALSE (disabled)
  int fNumberOfEventsPerCluster = 1000;   // Maximum basket size of the trees

  TaskModes fTaskMode = kStandard; // Running mode of the task. Useful to set for e.g. MC mode

  // Data structures

  struct {
    // Event data

    ULong64_t fEventId = 0u;    /// Event (collision) unique id. Contains peiod, orbit and bunch crossing numbers
    // Primary vertex position
    Float_t  fX = -999.f;       /// Primary vertex x coordinate
    Float_t  fY = -999.f;       /// Primary vertex y coordinate
    Float_t  fZ = -999.f;       /// Primary vertex z coordinate
    // Primary vertex covariance matrix
    Float_t  fCovXX = 999.f;    /// cov[0]
    Float_t  fCovXY = 0.f;      /// cov[1]
    Float_t  fCovXZ = 0.f;      /// cov[2]
    Float_t  fCovYY = 999.f;    /// cov[3]
    Float_t  fCovYZ = 0.f;      /// cov[4]
    Float_t  fCovZZ = 999.f;    /// cov[5]
    // Quality parameters
    Float_t  fChi2;             /// Chi2 of the vertex
    UInt_t   fN;                /// Number of contributors

    // The calculation of event time certainly will be modified in Run3
    // The prototype below can be switched on request
    Float_t fEventTime = -999.f;    /// Event time (t0) obtained with different methods (best, T0, T0-TOF, ...)
    Float_t fEventTimeRes = -999.f; /// Resolution on the event time (t0) obtained with different methods (best, T0, T0-TOF, ...)
    UChar_t fEventTimeMask = 0u;    /// Mask with the method used to compute the event time (0x1=T0-TOF,0x2=T0A,0x3=TOC) for each momentum bins

  } vtx; /// structure to keep the primary vertex (avoid name conflicts)

  // PH: The MC informafion has to be stored separately!
#ifdef USE_MC
  // MC information on the event
  Short_t fGeneratorID = 0u; /// Generator ID used for the MC
  Float_t fMCVtxX = -999.f;  /// Primary vertex x coordinate from MC
  Float_t fMCVtxY = -999.f;  /// Primary vertex y coordinate from MC
  Float_t fMCVtxZ = -999.f;  /// Primary vertex z coordinate from MC
#endif

  struct {
    // Track data

    Int_t   fCollisionID;    /// The index of the collision vertex in the TF, to which the track is attached

    // Coordinate system parameters
    Float_t fX = -999.f;     /// X coordinate for the point of parametrisation
    Float_t fAlpha = -999.f; /// Local <--> global coor.system rotation angle

    // Track parameters
    Float_t fY = -999.f;          /// fP[0] local Y-coordinate of a track (cm)
    Float_t fZ = -999.f;          /// fP[1] local Z-coordinate of a track (cm)
    Float_t fSnp = -999.f;        /// fP[2] local sine of the track momentum azimuthal angle
    Float_t fTgl = -999.f;        /// fP[3] tangent of the track momentum dip angle
    Float_t fSigned1Pt = -999.f;  /// fP[4] 1/pt (1/(GeV/c))

    // Covariance matrix
    Float_t fCYY = -999.f;       /// fC[0]
    Float_t fCZY = -999.f;       /// fC[1]
    Float_t fCZZ = -999.f;       /// fC[2]
    Float_t fCSnpY = -999.f;     /// fC[3]
    Float_t fCSnpZ = -999.f;     /// fC[4]
    Float_t fCSnpSnp = -999.f;   /// fC[5]
    Float_t fCTglY = -999.f;     /// fC[6]
    Float_t fCTglZ = -999.f;     /// fC[7]
    Float_t fCTglSnp = -999.f;   /// fC[8]
    Float_t fCTglTgl = -999.f;   /// fC[9]
    Float_t fC1PtY = -999.f;     /// fC[10]
    Float_t fC1PtZ = -999.f;     /// fC[11]
    Float_t fC1PtSnp = -999.f;   /// fC[12]
    Float_t fC1PtTgl = -999.f;   /// fC[13]
    Float_t fC1Pt21Pt2 = -999.f; /// fC[14]

    // Additional track parameters
    Float_t fTPCinnerP = -999.f; /// Full momentum at the inner wall of TPC for dE/dx PID

    // Track quality parameters
    ULong64_t fFlags = 0u;       /// Reconstruction status flags

    // Clusters
    UChar_t fITSClusterMap = 0u; /// ITS map of clusters, one bit per a layer
    UShort_t fTPCncls = 0u;      /// number of clusters assigned in the TPC
    UChar_t fTRDntracklets = 0u; /// number of TRD tracklets used for tracking/PID (TRD/TOF pattern)

    // Chi2
    Float_t fITSchi2Ncl = -999.f; /// chi2/Ncl ITS
    Float_t fTPCchi2Ncl = -999.f; /// chi2/Ncl TPC
    Float_t fTRDchi2 = -999.f;    /// chi2 TRD match (?)
    Float_t fTOFchi2 = -999.f;    /// chi2 TOF match (?)

    // PID
    Float_t fTPCsignal = -999.f; /// dE/dX TPC
    Float_t fTRDsignal = -999.f; /// dE/dX TRD
    Float_t fTOFsignal = -999.f; /// TOFsignal
    Float_t fLength = -999.f;    /// Int.Lenght @ TOF
  } tracks;

#ifdef USE_MC
  // Track labels
  Int_t fLabel = -1;           /// Track label
  Int_t fTOFLabel[3] = { -1 }; /// Label of the track matched to TOF

  // MC information
  Int_t fPdgCode = -99999;    /// PDG code of the particle
  Int_t fMother[2] = { 0 };   /// Indices of the mother particles
  Int_t fDaughter[2] = { 0 }; /// Indices of the daughter particles

  Float_t fPx = -999.f; /// x component of momentum
  Float_t fPy = -999.f; /// y component of momentum
  Float_t fPz = -999.f; /// z component of momentum

  Float_t fVx = -999.f; /// x of production vertex
  Float_t fVy = -999.f; /// y of production vertex
  Float_t fVz = -999.f; /// z of production vertex
  Float_t fVt = -999.f; /// t of production vertex
#endif

  // To test the compilation uncoment the line below
  // #define USE_TOF_CLUST 1
#ifdef USE_TOF_CLUST
  struct {
    // TOF clusters
    // PH: Do we store the TOF information per track?
    Int_t fTOFChannel = -1;        /// Index of the matched channel
    Short_t fTOFncls = -1;         /// Number of matchable clusters of the track
    Float_t fDx = -999.f;          /// Residual along x
    Float_t fDz = -999.f;          /// Residual along z
    Float_t fToT = -999.f;         /// ToT
    Float_t fLengthRatio = -999.f; /// Ratio of the integrated track length @ TOF to the cluster with respect to the matched cluster
  } tofClusters;
#endif

  struct {
    // Calorimeter data (EMCAL & PHOS)

    Int_t   fCollisionID;         /// The index of the collision vertex in the TF, to which the track is attached

    Short_t fCellNumber = -1;     /// Cell absolute Id. number
    Float_t fAmplitude = -999.f;  /// Cell amplitude (= energy!)
    Float_t fTime = -999.f;       /// Cell time
    Char_t fCellType = -1;        /// EMCAL: High Gain: 0 / Low Gain: 1 / TRU: 2 / LEDmon 3 (see DataFromatsEMCAL/Constants.h)
    Char_t fType = -1;            /// Cell type (-1 is undefined, 0 is PHOS, 1 is EMCAL)
  } calo;
  
  struct {
    // Calorimeter trigger data (EMCAL & PHOS)
    Int_t   fCollisionID;         /// The index of the collision vertex in the TF, to which the track is attached
    Short_t fFastorAbsID = - 1;   /// FastOR absolute ID
    Float_t fL0Amplitude = -1.f;  /// L0 amplitude (ADC) := Peak Amplitude
    Float_t fL0Time = -1.f;       /// L0 time
    Int_t fL1TimeSum = -1;        /// L1 amplitude (ADC) := Integral over L0 time samples
    Char_t fNL0Times = -1;        /// Number of L0 times
    Int_t fTriggerBits = 0;       /// Online trigger bits
    Char_t fType = -1;            /// Calorimeter type (-1 is undefined, 0 is PHOS, 1 is EMCAL)
  } calotrigger;

  struct {
    // MUON track data

    Int_t   fCollisionID;            /// The index of the collision vertex, to which the muon is attached

    /// Parameters at vertex
    Float_t fInverseBendingMomentum; ///< Inverse bending momentum (GeV/c ** -1) times the charge 
    Float_t fThetaX;                 ///< Angle of track at vertex in X direction (rad)
    Float_t fThetaY;                 ///< Angle of track at vertex in Y direction (rad)
    Float_t fZ;                      ///< Z coordinate (cm)
    Float_t fBendingCoor;            ///< bending coordinate (cm)
    Float_t fNonBendingCoor;         ///< non bending coordinate (cm)

    /// Reduced covariance matrix of UNCORRECTED track parameters, ordered as follow:      <pre>
    /// [0] =  <X,X>
    /// [1] =<X,ThetaX>  [2] =<ThetaX,ThetaX>
    /// [3] =  <X,Y>     [4] =  <Y,ThetaX>     [5] =  <Y,Y>
    /// [6] =<X,ThetaY>  [7] =<ThetaX,ThetaY>  [8] =<Y,ThetaY>  [9] =<ThetaY,ThetaY>
    /// [10]=<X,InvP_yz> [11]=<ThetaX,InvP_yz> [12]=<Y,InvP_yz> [13]=<ThetaY,InvP_yz> [14]=<InvP_yz,InvP_yz>  </pre>
    Float_t fCovariances[15]; ///< \brief reduced covariance matrix of parameters AT FIRST CHAMBER

    /// Global tracking info
    Float_t fChi2;                ///< chi2 in the MUON track fit
    Float_t fChi2MatchTrigger;    ///< chi2 of trigger/track matching
  } muons;

  struct {
    // Muon clister data
    
    Int_t   fMuTrackID; /// The index of the track to which the clusters are attached
    Float_t fX;         ///< cluster X position
    Float_t fY;         ///< cluster Y position
    Float_t fZ;         ///< cluster Z position
    Float_t fErrX;      ///< transverse position errors
    Float_t fErrY;      ///< transverse position errors
    Float_t fCharge;    ///< cluster charge
    Float_t fChi2;      ///< cluster chi2
  } mucls;

  struct {
    // ZDC: it is not clear what is the minimal set of information (PH)

    Int_t     fCollisionID;          /// The index of the collision vertex

    Float_t   fZEM1Energy;   	     ///< E in ZEM1
    Float_t   fZEM2Energy;	     ///< E in ZEM2

    Float_t   fZNCTowerEnergy[5];    ///< E in 5 ZNC sectors - high gain chain
    Float_t   fZNATowerEnergy[5];    ///< E in 5 ZNA sectors - high gain chain
    Float_t   fZPCTowerEnergy[5];    ///< E in 5 ZPC sectors - high gain chain
    Float_t   fZPATowerEnergy[5];    ///< E in 5 ZPA sectors - high gain chain

    Float_t   fZNCTowerEnergyLR[5];  ///< E in 5 ZNC sectors - low gain chain
    Float_t   fZNATowerEnergyLR[5];  ///< E in 5 ZNA sectors - low gain chain
    Float_t   fZPCTowerEnergyLR[5];  ///< E in 5 ZPC sectors - low gain chain
    Float_t   fZPATowerEnergyLR[5];  ///< E in 5 ZPA sectors - low gain chain

    Float_t   fZDCTDCCorrected[32][4]; /// ZDC TDC data in ns corrected 4 phase shift

    UChar_t   fFired;                  /// Bits: 0 - ZNA, 1 - ZNC, 2 - ZPA, 3 - ZPC, 4 - ZEM1, 5 - ZEM2
  } zdc;

  struct {
    /// VZERO as proxy for FIT

    Int_t   fCollisionID;      /// The index of the collision vertex

    Float_t fAdc[64];          ///  adc for each channel
    Float_t fTime[64];         ///  time for each channel
    Float_t fWidth[64];        ///  time width for each channel
    ULong64_t fBBFlag;         ///  BB Flags from Online V0 Electronics
    ULong64_t fBGFlag;         ///  BG Flags from Online V0 Electronics
  } vzero;

  struct {
    /// V0s (Ks, Lambda)

    Int_t fPosTrackID; // Positive track ID
    Int_t fNegTrackID; // Negative track ID
  } v0s;

  struct {
    /// Cascades

    Int_t fV0ID; // V0 ID
    Int_t fBachelorID; // Bachelor track ID
  } cascs;

  /// Offsets to convert the IDs within one collision to global IDs
  Int_t fOffsetMuTrackID = 0; ///! Offset of MUON track IDs (used in the clusters)
  Int_t fOffsetTrackID = 0;   ///! Offset of track IDs (used in V0s)
  Int_t fOffsetV0ID = 0;      ///! Offset of track IDs (used in cascades)

  ClassDef(AliAnalysisTaskAO2Dconverter, 3);
};

#endif
