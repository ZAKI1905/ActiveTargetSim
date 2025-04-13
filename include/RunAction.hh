// ============================================================================
//  File   : RunAction.hh
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Declares the RunAction class that manages run-level lifecycle actions,
//           including initialization and output of ROOT histograms for muon creation
//           energies, stopping depths, and radial distributions in magnetic fields.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "TFile.h"
#include "TH1D.h"

// ============================================================================
// RunAction Class Declaration
// ============================================================================
/**
 * @class RunAction
 * @brief Manages run-level initialization, histogram setup, and ROOT file output for
 * muon transport and stopping analysis.
 *
 * This class is responsible for initializing ROOT histograms at the start of a run
 * and writing them to disk at the end. It records per-event observables such as
 * muon creation energy, stopping Z-position, and radial stopping distance in a
 * magnetic field configuration.
 */
class RunAction : public G4UserRunAction
{
  public:
	/**
	 * @brief Constructor.
	 *
	 * Initializes ROOT file and histogram for energy deposition scoring.
	 */
	RunAction();

	/**
	 * @brief Destructor.
	 *
	 * Closes the ROOT file and cleans up resources.
	 */
	virtual ~RunAction();

	/**
	 * @brief Called at the beginning of each run.
	 *
	 * Initializes histogram and opens ROOT file for output.
	 *
	 * @param run Pointer to the current G4Run object.
	 */
	virtual void BeginOfRunAction(const G4Run *run) override;

	/**
	 * @brief Called at the end of each run.
	 *
	 * Writes histogram data to ROOT file and closes the file.
	 *
	 * @param run Pointer to the completed G4Run object.
	 */
	virtual void EndOfRunAction(const G4Run *run) override;

	/**
	 * @brief Accessor for the energy histogram.
	 * @return Pointer to the ROOT TH1D histogram object.
	 */
	TH1D *GetEnergyHistogram() const { return fEnergyHist; }

	/**
	 * @brief Returns a pointer to the histogram tracking muon radial stopping distance.
	 * @return Pointer to the 1D ROOT histogram for transverse stop positions (in mm).
	 */
	TH1D *GetMuonStoppingHistogram() const { return fMuonStoppingHist; }

  private:
	/// Pointer to the ROOT output file
	TFile *fRootFile = nullptr;

	/// Histogram for recording initial muon kinetic energies (MeV)
	TH1D *fEnergyHist = nullptr;

	/// Histogram for muon radial stopping distances (in mm)
	TH1D *fMuonStoppingHist = nullptr;
};
// ============================================================================

#endif