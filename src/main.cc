// ============================================================================
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  File   : main.cc
//  Purpose: Defines the main entry point for initializing the simulation,
//           setting up geometry, physics processes, user actions, and
//           interactive/batch visualization modes.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "QGSP_BERT.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

/**
 * @brief Entry point of the ActiveTargetSim simulation.
 *
 * Handles the full setup of the simulation, including:
 * - Geometry and detector configuration
 * - Physics processes
 * - Primary particle generation
 * - Visualization engine
 * - Interactive or batch execution
 *
 * @param argc Argument count
 * @param argv Argument values (used to check for interactive mode)
 * @return Exit code
 */
int main(int argc, char **argv)
{
	// =========================================================================
	// UI Setup
	// =========================================================================
	G4UIExecutive *ui = nullptr;
	if (argc == 1)
	{
		// No command-line arguments provided: assume interactive session
		ui = new G4UIExecutive(argc, argv);
	}

	// =========================================================================
	// Run Manager
	// =========================================================================
	auto *runManager = new G4RunManager();

	// =========================================================================
	// Detector Setup (includes magnetic field and scoring volumes)
	// =========================================================================
	auto *detector = new DetectorConstruction();
	detector->SetDetectorType("muonTarget"); // Choose detector geometry variant (e.g., carbonStack, alternatingLayers, muonTarget)
	runManager->SetUserInitialization(detector);

	// =========================================================================
	// Physics List
	// =========================================================================
	// QGSP_BERT handles EM interactions and hadronic cascades, including muon-nuclear effects
	runManager->SetUserInitialization(new QGSP_BERT());

	// =========================================================================
	// Register User Actions
	// =========================================================================
	// Primary generator (defines the particle beam)
	runManager->SetUserAction(new PrimaryGeneratorAction());

	// Actions at the start and end of each simulation run
	runManager->SetUserAction(new RunAction());

	// Step-level user actions (e.g., scoring)
	runManager->SetUserAction(new SteppingAction(detector));

	// Register user-defined tracking action (e.g., muon birth info)
	runManager->SetUserAction(new TrackingAction());

	// =========================================================================
	// Visualization Engine
	// =========================================================================
	auto *visManager = new G4VisExecutive();
	visManager->Initialize();

	// =========================================================================
	// Command Manager
	// =========================================================================
	auto *UImanager = G4UImanager::GetUIpointer();

	// =========================================================================
	// Execution Mode
	// =========================================================================
	if (ui)
	{
		// ----- Interactive Mode -----
		UImanager->ApplyCommand("/control/execute vis.mac"); // Load viewer & draw geometry
		// UImanager->ApplyCommand("/control/execute vis_geometry.mac"); // Load viewer & draw geometry
		ui->SessionStart(); // Start command-line session
		delete ui;
	}
	else
	{
		// ----- Batch Mode -----
		UImanager->ApplyCommand("/run/beamOn 10"); // Default batch execution (adjustable via script)
	}

	// =========================================================================
	// Cleanup
	// =========================================================================
	delete visManager;
	delete runManager;

	return 0;
}