// ============================================================================
//  File   : TrackingAction.hh
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Declares the TrackingAction class, which processes logic
//           related to particle tracking. Logs muon creation information,
//           and records stopping positions for physics analysis and histogramming.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-04
// ============================================================================

#ifndef TRACKING_ACTION_HH
#define TRACKING_ACTION_HH

#include "G4UserTrackingAction.hh"
#include "globals.hh"

// ============================================================================
// TrackingAction Class Declaration
// ============================================================================
/**
 * @brief Tracks muon creation and stopping diagnostics for analysis and histogramming.
 *
 * This class hooks into the GEANT4 tracking system and executes user-defined
 * logic at the beginning and end of a particle’s trajectory. In this project,
 * it is used to print and log muon creation information — including energy,
 * position, and process — and to record final muon stopping coordinates (Z, radial).
 * Histogram entries are filled for subsequent ROOT analysis.
 */
class G4Track;

class TrackingAction : public G4UserTrackingAction
{
  public:
	/// Constructor
	TrackingAction();

	/// Destructor
	virtual ~TrackingAction();

	/**
	 * @brief Called before a new particle track is processed.
	 *
	 * Checks if the particle is a muon and, if so, prints useful debug info.
	 *
	 * @param track Pointer to the GEANT4 track object.
	 */
	virtual void PreUserTrackingAction(const G4Track *track) override;

	/**
	 * @brief Called after a particle’s track has ended.
	 *
	 * Common use cases include checking if a muon has stopped, printing diagnostics,
	 * or recording final positions for analysis.
	 *
	 * @param track Pointer to the GEANT4 track that just completed.
	 */
	virtual void PostUserTrackingAction(const G4Track *track) override;
};
// ============================================================================

#endif