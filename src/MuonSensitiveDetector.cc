// ============================================================================
//  File   : MuonSensitiveDetector.cc
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Implements a sensitive detector to track energy deposition (dE)
//           from particles in scoring volumes. Can be extended to log or process
//           muon-specific interactions and diagnostics.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#include "MuonSensitiveDetector.hh"

#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4ios.hh"

// ============================================================================
// Constructor / Destructor
// ============================================================================

/**
 * @brief Constructs the MuonSensitiveDetector and sets its name.
 *
 * @param name Name of the sensitive detector (used for registration).
 */
MuonSensitiveDetector::MuonSensitiveDetector(const G4String &name)
	: G4VSensitiveDetector(name)
{
	// No initialization required at this stage
}

/**
 * @brief Destructor for MuonSensitiveDetector.
 */
MuonSensitiveDetector::~MuonSensitiveDetector()
{
	// Nothing to clean up
}

// ============================================================================
// ProcessHits
// ============================================================================

/**
 * @brief Processes energy deposition (hits) in the scoring volume.
 *
 * Called every time a particle takes a step within a sensitive detector
 * volume. Currently this is a placeholder for processing energy deposition
 * or identifying specific particle types (e.g., muons) for scoring or
 * diagnostics.
 *
 * Logging is disabled by default, but can be re-enabled for debugging or
 * histogramming.
 *
 *
 * @param step     Pointer to the current G4Step object.
 * @param history  Pointer to the touchable history (not used).
 * @return true if hit was processed.
 */
G4bool MuonSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
	// Future extension: Accumulate energy deposition, count particle types,
	// or track muon energy loss per volume.
	G4double edep = step->GetTotalEnergyDeposit();

	// if (edep > 0)
	// {
	// 	G4String particle = step->GetTrack()->GetDefinition()->GetParticleName();
	// 	G4cout << "[MuonSD] " << particle << " deposited "
	// 		   << edep / MeV << " MeV" << G4endl;
	// }

	return true;
}

// ============================================================================