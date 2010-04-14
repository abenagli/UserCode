import FWCore.ParameterSet.Config as cms

eventIdFilter = cms.EDFilter(
    "EventIdFilter",
    runId   = cms.int32(132959),
    eventId = cms.int32(346686)
)

