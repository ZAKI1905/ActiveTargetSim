// ============================================================================
//  File   : PrimaryGeneratorAction.cc
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Defines and configures the primary particle generator.
//           Responsible for injecting protons (or other primaries) with
//           defined energy, position, and direction into the simulation world.
//           Currently set to emit 1 GeV protons from -15 cm along the +z-axis.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

// ============================================================================
// Constructor / Destructor
// ============================================================================
/**
 * @brief Constructor
 *
 * Initializes a G4ParticleGun to generate single protons with defined
 * energy (1 GeV), position (-15 cm along z), and direction (+z).
 */
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	G4int n_particle = 1; // Generate one particle per event
	fParticleGun = new G4ParticleGun(n_particle);

	// Define particle type: proton
	G4ParticleDefinition *particle =
		G4ParticleTable::GetParticleTable()->FindParticle("proton");
	fParticleGun->SetParticleDefinition(particle);

	// Set particle initial position (15 cm upstream)
	fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -15. * cm));

	// Future: This can be generalized to sample from a beam profile or phase-space distribution.

	// Set initial direction of motion (along +z)
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

	// Set kinetic energy to 1 GeV
	fParticleGun->SetParticleEnergy(1.0 * GeV);
}

// ----------------------------------------------------------------------------
/**
 * @brief Destructor
 *
 * Releases memory used by the particle gun.
 */
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
}

// ============================================================================
// GeneratePrimaries
// ============================================================================

/**
 * @brief Injects the primary particle into the current event.
 *
 * Called by the Geant4 framework at the beginning of each event to define
 * the primary vertex and particle. This method triggers the configured
 * G4ParticleGun.
 *
 * @param anEvent Pointer to the current event.
 */
void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

// ============================================================================