// ============================================================================
//  File   : MuonSensitiveDetector.hh
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Defines the sensitive detector class used to track muon stops and
//           energy deposition events inside target materials and scoring volumes.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#ifndef MUON_SENSITIVE_DETECTOR_HH
#define MUON_SENSITIVE_DETECTOR_HH

#include "G4Step.hh"
#include "G4VSensitiveDetector.hh"
#include "globals.hh"
#include <G4String.hh>

// ============================================================================
// MuonSensitiveDetector Class Declaration
// ============================================================================
/**
 * @class MuonSensitiveDetector
 * @brief Sensitive detector that tracks muon stops and energy deposition (dE/dx)
 * within designated scoring volumes, aiding in spatial diagnostics and efficiency studies.
 *
 * This class inherits from G4VSensitiveDetector and is used to tag muon-related events,
 * such as energy loss and stopping position, inside a designated scoring volume.
 * It collaborates with `SteppingAction` and `RunAction` to accumulate diagnostic data
 * across runs and enables detailed event-based or histogram-based analysis.
 */
class MuonSensitiveDetector : public G4VSensitiveDetector
{
  public:
	/**
	 * @brief Constructor.
	 * @param name The name used to identify this sensitive detector.
	 */
	MuonSensitiveDetector(const G4String &name);

	/**
	 * @brief Destructor.
	 */
	virtual ~MuonSensitiveDetector();

	/**
	 * @brief Called at every simulation step inside a sensitive volume.
	 *
	 * This method handles hit processing such as recording deposited energy
	 * or triggering detector responses.
	 *
	 * @param step Pointer to the current step.
	 * @param history Touchable history (geometry context).
	 * @return True if the hit was processed successfully.
	 */
	virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;
};
// ============================================================================

#endif