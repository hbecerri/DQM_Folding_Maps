
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.TFileService = cms.Service("TFileService", fileName = cms.string("Tracking_Info.root") )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(

#run 319756 Zero Bias AOD


        'root://cms-xrd-global.cern.ch//store/data/Run2018C/ZeroBias/AOD/PromptReco-v2/000/319/756/00000/6E29D2DE-878B-E811-88AE-02163E019F1A.root'      


    )
)

process.demo = cms.EDAnalyzer('DemoAnalyzer'
     , tracks = cms.untracked.InputTag('generalTracks')
#generalTracks
)


process.p = cms.Path(process.demo)
