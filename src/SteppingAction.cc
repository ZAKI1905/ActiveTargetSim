// ============================================================================
//  File   : SteppingAction.cc
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Handles per-step particle tracking and scoring logic within the
//           simulation. Specifically records:
//           - Muon creation energy
//           - Z-position and radial distance at stop
//           - Target layer index where the muon stops
//           Data is stored in ROOT histograms for offline analysis.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ios.hh"

// ============================================================================
// Constructor / Destructor
// ============================================================================

/**
 * @brief Called at every step of every particle in the simulation.
 *
 * Tracks muons to:
 *  - Log their kinetic energy at creation
 *  - Record their Z-position and radial stopping distance
 *  - Identify which target volume the muon stopped in
 *
 * Fills four histograms:
 *  - H1(0): Muon creation energy [MeV]
 *  - H1(1): Muon stopping Z position [mm]
 *  - H1(2): Index of target volume where muon stopped
 *  - H1(3): Radial distance from beam axis at muon stop [mm]
 *
 * Additional debug output includes:
 *  - Track ID, particle type, volume name and material, stop location
 *
 * @param step Pointer to the current G4Step object representing the simulation step.
 */
SteppingAction::SteppingAction(const DetectorConstruction *detectorConstruction)
	: fDetectorConstruction(detectorConstruction)
{
}
// ----------------------------------------------------------------------------
/**
 * @brief Destructor
 *
 * No custom cleanup is needed for this class.
 */
SteppingAction::~SteppingAction() {}

// ============================================================================
// User Stepping Action
// ============================================================================
/**
 * @brief Called at every step of every particle in the simulation.
 *
 * Tracks muons to:
 *  - Log their kinetic energy at creation (step 1)
 *  - Log their Z position and stopping volume when they come to rest (step 2)
 *  - Identify which target volume and material the muon stopped in
 *
 * Fills three histograms:
 *  - H1(0): Muon creation energy [MeV]
 *  - H1(1): Muon stopping Z position [mm]
 *  - H1(2): Target volume index where muon stopped (integer index)
 *
 * Additional diagnostic output (via G4cout) includes:
 *  - Track ID, particle type, Z stop position
 *  - Logical volume name and material
 *
 * @param step Pointer to the current G4Step object representing the simulation step.
 */
void SteppingAction::UserSteppingAction(const G4Step *step)
{
	const G4Track *track = step->GetTrack();
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	G4ParticleDefinition *particle = track->GetDefinition();

	// Tracking pions
	// if (particle->GetParticleName() == "pi+" || particle->GetParticleName() == "pi-")
	// {
	// 	G4cout << "[PionCreated] " << particle->GetParticleName()
	// 		   << " at Z = " << track->GetPosition().z() / mm << " mm" << G4endl;
	// }

	// Only care about muons
	if (particle->GetParticleName() != "mu+" && particle->GetParticleName() != "mu-")
		return;

	auto baseAction = const_cast<G4UserEventAction *>(
		G4RunManager::GetRunManager()->GetUserEventAction());

	auto eventAction = dynamic_cast<EventAction *>(baseAction);
	if (eventAction)
	{
		eventAction->SetKeepEvent(true);
	}

	// Case 1: muon was just created
	if (track->GetCurrentStepNumber() == 1)
	{
		// G4cout << "[DEBUG] About to fill histogram for track ID "
		// 	   << track->GetTrackID() << ", particle: " << track->GetParticleDefinition()
		// 	   << ", step length: " << step->GetStepLength()
		// 	   << ", volume: " << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
		// 	   << G4endl;

		G4double energy = track->GetKineticEnergy();
		// G4cout << "[DEBUG] FillH1 ID=0, energy=" << energy << G4endl;
		analysisManager->FillH1(0, energy / MeV); // Histogram 0: MuonEnergy
	}

	// Case 2: muon is about to stop (track status is fStopAndKill)
	if (track->GetTrackStatus() == fStopAndKill)
	{
		G4ThreeVector pos = track->GetPosition();
		G4double r = std::sqrt(pos.x() * pos.x() + pos.y() * pos.y());
		analysisManager->FillH1(3, r / mm); // Histogram ID 3: radial distance from beamline
		// G4cout << "[DEBUG] FillH1 ID=3, rStop=" << r / mm << " mm" << G4endl;

		G4double zStop = pos.z();
		// G4cout << "[DEBUG] FillH1 ID=1, zStop=" << zStop << G4endl;
		analysisManager->FillH1(1, zStop / mm); // Histogram 1: MuonStopZ

		// Get volume muon stopped in
		G4LogicalVolume *vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

		// Map volume to integer index
		int targetIndex = -1;
		size_t numTargets = fDetectorConstruction->GetNumTargetVolumes();
		for (size_t i = 0; i < numTargets; ++i)
		{
			// G4cout << "[DEBUG] Stopped in volume: " << vol->GetName() << G4endl;
			auto targetVol = fDetectorConstruction->GetTargetNVolume(i);
			// G4cout << "[DEBUG] Target " << i << " volume pointer: " << targetVol
			// 	   << ", name: " << targetVol->GetName() << G4endl;

			if (vol == targetVol)
			{
				targetIndex = static_cast<int>(i);
				break;
			}
		}

		// Log which volume the muon stopped in
		G4String material = vol->GetMaterial()->GetName();
		G4String volName = vol->GetName();

		G4cout << "[MuonStopped] " << particle->GetParticleName()
			   << " | Track ID: " << track->GetTrackID()
			   << " | Z = " << zStop / mm << " mm"
			   << " | Volume: " << volName
			   << " | Material: " << material << G4endl;

		// If stopped in a known tungsten target
		if (targetIndex >= 0)
		{
			// G4cout << "[DEBUG] FillH1 ID=2, targetIndex=" << targetIndex << G4endl;
			analysisManager->FillH1(2, targetIndex); // Histogram 2: MuonStopTarget
		}

		// NEW: If stopped in D-T gas region
		if (volName == "DTGasLogical")
		{
			G4cout << "[MuonStopped-DT] " << particle->GetParticleName()
				   << " | Z = " << zStop / mm << " mm"
				   << " | R = " << r / mm << " mm" << G4endl;

			analysisManager->FillH1(4, zStop / mm); // Histogram 4: MuonStopZ in D-T
			analysisManager->FillH1(5, r / mm);		// Histogram 5: MuonStopR in D-T
		}
	}
}
// ============================================================================