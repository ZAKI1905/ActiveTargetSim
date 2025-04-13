// ============================================================================
//  File   : SteppingAction.hh
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Declares the SteppingAction class, responsible for tracking particle
//           steps, scoring muon creation energy, stopping depth, and radial
//           stopping position within configured magnetic field environments.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Step;
class DetectorConstruction;

// ============================================================================
// SteppingAction Class Declaration
// ============================================================================
/**
 * @class SteppingAction
 * @brief Monitors and analyzes particle transport step-by-step, with emphasis on
 * muon stopping position (Z and radial), initial energy, and scoring volume entry.
 *
 * This class is invoked for every step taken by every particle. It retrieves the
 * current step's volume, particle type, energy deposition, and spatial coordinates.
 * It tracks when a muon is created, when it stops, and where it stops — both along
 * the beam axis (Z) and transversely (R) — enabling spatially resolved scoring.
 * This is critical for analyzing how field configurations affect stopping behavior.
 */
class SteppingAction : public G4UserSteppingAction
{
  public:
	/**
	 * @brief Constructor.
	 * @param detectorConstruction Pointer to the DetectorConstruction instance
	 *        to access the scoring volume geometry.
	 */
	SteppingAction(const DetectorConstruction *detectorConstruction);

	/**
	 * @brief Destructor.
	 */
	virtual ~SteppingAction();

	/**
	 * @brief Action performed at each simulation step.
	 * @param step Pointer to the current G4Step. This object contains
	 *             information about energy deposition, particle, volume, etc.
	 */
	virtual void UserSteppingAction(const G4Step *step) override;

  private:
	/// Pointer to detector construction class to access geometry definitions.
	const DetectorConstruction *fDetectorConstruction;

	/// Cached pointer to the logical volume designated for scoring.
	G4LogicalVolume *fScoringVolume = nullptr;
};
// ============================================================================

#endif