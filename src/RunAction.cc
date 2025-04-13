// ============================================================================
//  File   : RunAction.cc
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Manages run-level actions including histogram setup, ROOT file I/O,
//           and diagnostics for muon creation, stopping location, and radial distribution.
//           Supports quantitative analysis of muon production and confinement behavior.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

// ============================================================================
// Constructor / Destructor
// ============================================================================

/**
 * @brief Constructor
 *
 * Creates a ROOT file and initializes the histogram to track energy deposition
 * in the detector's scoring volume.
 * Initializes histograms for energy, position, target ID, and radial distribution.
 */
RunAction::RunAction()
{
}

/**
 * @brief Destructor
 *
 * Cleans up ROOT resources by deleting the histogram and closing the file.
 */
RunAction::~RunAction()
{
}

// ============================================================================
// Run Lifecycle Methods
// ============================================================================

/**
 *  @brief Initializes histograms and opens the ROOT output file.
 *
 * Executed at the start of each simulation run.
 *
 * Histograms created:
 * - Muon creation energy
 * - Z-position of muon stopping
 * - Index of the target volume where muon stopped
 * - Radial stopping distance from beam axis
 *
 * @param run Pointer to the current G4Run (unused here).
 */
void RunAction::BeginOfRunAction(const G4Run *)
{
	G4cout << "### Run started ###" << G4endl;

	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetDefaultFileType("root"); // or "csv", "hdf5", "xml"
	analysisManager->OpenFile("muon_output");

	// Create histograms for muon diagnostics
	analysisManager->CreateH1("MuonEnergy", "Muon Creation Energy (MeV)", 100, 0., 200.);		  // Initial kinetic energy
	analysisManager->CreateH1("MuonStopZ", "Muon Stopping Z Position (mm)", 100, -150., 150.);	  // Longitudinal stop position
	analysisManager->CreateH1("MuonStopTarget", "Muon Stopped in Target Layer (int)", 10, 0, 10); // Target index
	analysisManager->CreateH1("MuonStopRadius", "Muon radial stop distance [mm]", 100, 0., 50.0); // sqrt(x^2 + y^2) at stop
}

/**
 * @brief Finalizes histogram data output.
 *
 * Writes data to ROOT file and closes it while retaining histogram memory
 * for post-run visualization in Geant4 (/vis/plot) or offline analysis.
 *
 * @param run Pointer to the current G4Run (unused here).
 */
void RunAction::EndOfRunAction(const G4Run *)
{
	G4cout << "### Run ended, saving ROOT output... ###" << G4endl;

	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();

	// This tells Geant4:
	//  - Close the file
	//  - Do NOT reset the histograms
	//  - So they can still be plotted in-session (/vis/plot) or exported to ROOT
	analysisManager->CloseFile(false);
}

// ============================================================================