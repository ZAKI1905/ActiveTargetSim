// ============================================================================
//  File   : PrimaryGeneratorAction.hh
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Defines the primary event generator for injecting particles such as
//           protons or muons into the simulation. Responsible for initializing
//           particle type, energy, position, and direction, tailored to muon production
//           and moderation studies.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;

// ============================================================================
// PrimaryGeneratorAction Class Declaration
// ============================================================================
/**
 * @class PrimaryGeneratorAction
 * @brief Configures the initial state of primary particles injected into the simulation volume.
 *
 * This class sets up and uses a G4ParticleGun to inject primaries—such as protons for
 * muon production or muons for moderation studies—into the world volume. It defines
 * the initial conditions of the simulation, including position, momentum direction,
 * particle type, and kinetic energy. Called at the start of each event.
 */
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
	/**
	 * @brief Constructor.
	 *
	 * Initializes the G4ParticleGun and sets default particle properties
	 * such as type, energy, position, and direction.
	 */
	PrimaryGeneratorAction();

	/**
	 * @brief Destructor.
	 */
	virtual ~PrimaryGeneratorAction();

	/**
	 * @brief Generates the primary particle(s) at the start of each event.
	 *
	 * This method is automatically invoked by GEANT4 during the event loop
	 * and defines the primary particle kinematics.
	 *
	 * @param event Pointer to the current G4Event object.
	 */
	virtual void GeneratePrimaries(G4Event *event) override;

  private:
	/**
	 * @brief Pointer to the G4ParticleGun instance used to define and launch primary particles per event.
	 */
	G4ParticleGun *fParticleGun;
};
// ============================================================================

#endif