/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

// AliFlowEventCuts:
// An event cut class
// origin: Mikolaj Krzewicki (mikolaj.krzewicki@cern.ch)

#ifndef ALIFLOWEVENTCUTS_H
#define ALIFLOWEVENTCUTS_H

#include <float.h>
#include <limits.h>
#include "TNamed.h"

class AliVEvent;
#include "AliFlowTrackCuts.h"

class AliFlowEventCuts : public TNamed {

 public:
  enum refMultMethod { kTPConly, kSPDtracklets, kV0, kSPD1clusters };

  AliFlowEventCuts();
  AliFlowEventCuts(const char* name, const char* title = "AliFlowEventCuts");
  AliFlowEventCuts(const AliFlowEventCuts& someCuts);
  AliFlowEventCuts& operator=(const AliFlowEventCuts& someCuts);
  virtual  ~AliFlowEventCuts();
  
  virtual Bool_t IsSelected(TObject* obj);

  Bool_t PassesCuts(AliVEvent* event);
  
  static AliFlowEventCuts* StandardCuts();
  
  void SetNumberOfTracksMax(Int_t value) {fNumberOfTracksMax=value;fCutNumberOfTracks=kTRUE;}
  void SetNumberOfTracksMin(Int_t value) {fNumberOfTracksMin=value;fCutNumberOfTracks=kTRUE;}
  void SetNumberOfTracksRange(Int_t min, Int_t max) {fNumberOfTracksMin=min;fNumberOfTracksMax=max;fCutNumberOfTracks=kTRUE;}
  void SetRefMultMax(Int_t value) {fRefMultMax=value;fCutRefMult=kTRUE;}
  void SetRefMultMin(Int_t value) {fRefMultMin=value;fCutRefMult=kTRUE;}
  void SetRefMultRange(Int_t min, Int_t max) {fRefMultMin=min;fRefMultMax=max;fCutRefMult=kTRUE;}
  void SetPrimaryVertexXrange(Double_t min, Double_t max)
       {fCutPrimaryVertexX=kTRUE; fPrimaryVertexXmin=min; fPrimaryVertexXmax=max;}
  void SetPrimaryVertexYrange(Double_t min, Double_t max)
       {fCutPrimaryVertexY=kTRUE; fPrimaryVertexYmin=min; fPrimaryVertexYmax=max;}
  void SetPrimaryVertexZrange(Double_t min, Double_t max)
       {fCutPrimaryVertexZ=kTRUE; fPrimaryVertexZmin=min; fPrimaryVertexZmax=max;}
  void SetNContributorsRange(Int_t min, Int_t max=INT_MAX) 
       {fCutNContributors=kTRUE; fNContributorsMin=min; fNContributorsMax=max;}
  void SetMeanPtRange(Double_t min, Double_t max) {fCutMeanPt=kTRUE; fMeanPtMax=max; fMeanPtMin=min;}
  void SetCutSPDvertexerAnomaly(Bool_t b) {fCutSPDvertexerAnomaly=b;}

  Int_t GetNumberOfTracksMax() const {return fNumberOfTracksMax;}
  Int_t GetNumberOfTracksMin() const {return fNumberOfTracksMin;}
  Int_t GetRefMultMax() const {return fRefMultMax;}
  Int_t GetRefMultMin() const {return fRefMultMin;}
  void SetRefMultMethod(refMultMethod m) {fRefMultMethod=m;}
  refMultMethod GetRefMultMethod() const {return fRefMultMethod;}
  void SetRefMultCuts( AliFlowTrackCuts* cuts ) {fRefMultCuts=static_cast<AliFlowTrackCuts*>(cuts->Clone());}
  void SetMeanPtCuts( AliFlowTrackCuts* cuts ) {fMeanPtCuts=static_cast<AliFlowTrackCuts*>(cuts->Clone());}
  AliFlowTrackCuts* GetRefMultCuts() const {return fRefMultCuts;}

  Int_t RefMult(AliVEvent* event);
  //Int_t GetRefMult() {return fRefMult;}
  Int_t GetReferenceMultiplicity(AliVEvent* event) {return RefMult(event);}
  const char* CentrMethName(refMultMethod) const;
  void SetCentralityPercentileRange(Float_t min, Float_t max){ fCentralityPercentileMin=min;
                                                               fCentralityPercentileMax=max;
                                                               fCutCentralityPercentile=kTRUE; }
  void SetCentralityPercentileMethod( refMultMethod m) {fRefMultMethod=m;}

 private:
  Bool_t fCutNumberOfTracks;//cut on # of tracks
  Int_t fNumberOfTracksMax;  //limits
  Int_t fNumberOfTracksMin;  //limits
  Bool_t fCutRefMult; //cut on refmult
  refMultMethod fRefMultMethod; //how do we calculate refmult?
  Int_t fRefMultMax; //max refmult
  Int_t fRefMultMin; //min refmult
  AliFlowTrackCuts* fRefMultCuts; //cuts
  AliFlowTrackCuts* fMeanPtCuts; //mean pt cuts
  Bool_t fCutPrimaryVertexX; //cut on x of prim vtx
  Double_t fPrimaryVertexXmax; //max x prim vtx
  Double_t fPrimaryVertexXmin; //min x prim vtx
  Bool_t fCutPrimaryVertexY; //cut on y of prim vtx
  Double_t fPrimaryVertexYmax; //max y prim vtx
  Double_t fPrimaryVertexYmin; //min y prim vtx
  Bool_t fCutPrimaryVertexZ; //cut on z of prim vtx
  Double_t fPrimaryVertexZmax; //max z prim vtx
  Double_t fPrimaryVertexZmin; //min z prim vtx
  Bool_t fCutNContributors; //cut on number of contributors
  Int_t fNContributorsMax; //maximal number of contrib
  Int_t fNContributorsMin; //minimal number of contrib
  Bool_t fCutMeanPt; //cut on mean pt
  Double_t fMeanPtMax; //max mean pt
  Double_t fMeanPtMin; //min mean pt
  Bool_t fCutSPDvertexerAnomaly; //cut on the spd vertexer anomaly
  Bool_t fCutCentralityPercentile; //cut on centrality perc. from AliESDCentrality
  refMultMethod fCentralityPercentileMethod; //where to get the percentile from
  Float_t fCentralityPercentileMax; // max centr. perc
  Float_t fCentralityPercentileMin; // min centr. perc



  ClassDef(AliFlowEventCuts,2)
};

#endif


