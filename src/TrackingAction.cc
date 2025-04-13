// ============================================================================
//  File   : TrackingAction.cc
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Implements muon tracking logic to log particle name,
//           creation energy, vertex position, and stopping location.
//           Also fills a ROOT histogram of muon stopping Z positions.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-04
// ============================================================================

#include "TrackingAction.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "RunAction.hh"

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
TrackingAction::TrackingAction()
{
}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
TrackingAction::~TrackingAction()
{
}

// ----------------------------------------------------------------------------
/**
 * @brief Called at the start of every new particle track.
 *
 * Filters for muons (mu+ or mu-) and prints their:
 *  - Particle type and track ID
 *  - Kinetic energy at creation
 *  - Vertex position
 *  - Creator process name (e.g., "Decay")
 *
 * Helps identify where and how muons are created in the detector setup.
 */
// ----------------------------------------------------------------------------
void TrackingAction::PreUserTrackingAction(const G4Track *track)
{
	const G4String &name = track->GetDefinition()->GetParticleName();

	if (name == "mu+" || name == "mu-")
	{
		const G4ThreeVector &pos = track->GetVertexPosition();
		G4double energy = track->GetKineticEnergy();
		const G4VProcess *creator = track->GetCreatorProcess();

		G4cout << "[MuonCreated] "
			   << name << " | Track ID: " << track->GetTrackID()
			   << " | Energy: " << energy / MeV << " MeV"
			   << " | Position: (" << pos.x() / mm << ", " << pos.y() / mm << ", " << pos.z() / mm << ") mm"
			   << " | Created by: " << (creator ? creator->GetProcessName() : "Primary")
			   << G4endl;
	}
}

// ----------------------------------------------------------------------------
/**
 * @brief Called after a particle's track is terminated.
 *
 * If the particle is a muon, this logs its stopping Z position
 * and fills the ROOT histogram via RunAction.
 */
void TrackingAction::PostUserTrackingAction(const G4Track *track)
{
	const G4String &name = track->GetDefinition()->GetParticleName();

	if (name == "mu+" || name == "mu-")
	{
		G4ThreeVector stopPos = track->GetPosition();
		G4double z = stopPos.z();

		// Optional debug print
		G4cout << "[MuonStopped] " << name
			   << " | Track ID: " << track->GetTrackID()
			   << " | Stopped at Z = " << z / mm << " mm" << G4endl;

		// Fill histogram
		// Note: redundant with SteppingAction histogram H1(1), but this is cleaner for debugging
		auto runAction = static_cast<const RunAction *>(G4RunManager::GetRunManager()->GetUserRunAction());

		if (runAction && runAction->GetMuonStoppingHistogram())
		{
			runAction->GetMuonStoppingHistogram()->Fill(z / mm);
		}
	}
}
// ----------------------------------------------------------------------------

// ============================================================================
