// ============================================================================
//  File: EventAction.hh
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose:
//    Defines the EventAction class used to control per-event behavior in the
//    simulation. Specifically, it enables selective retention of events for
//    visualization in the Geant4 GUI viewer, allowing only events that meet
//    certain criteria (e.g., muon production) to be kept.
//
//  Author: Mohammadreza Zakeri (Zaki)
//  Email : m.zakeri@eku.edu
//  Created: 2025-04-15
// ============================================================================

#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

/**
 * @class EventAction
 * @brief Handles logic to selectively retain Geant4 events for visualization.
 *
 * This class allows user control over which events are kept for visualization
 * in the Geant4 GUI viewer (limited to 100 events by default). In this project,
 * we use it to keep only events where at least one muon was produced.
 */
class EventAction : public G4UserEventAction
{
  public:
	/**
	 * @brief Constructor initializes the internal flag.
	 */
	EventAction();

	/**
	 * @brief Destructor.
	 */
	virtual ~EventAction();

	/**
	 * @brief Called at the beginning of each event.
	 * Resets the internal event retention flag.
	 * @param event Pointer to the current event.
	 */
	virtual void BeginOfEventAction(const G4Event *event);

	/**
	 * @brief Called at the end of each event.
	 * Instructs Geant4 to keep the event if flagged.
	 * @param event Pointer to the current event.
	 */
	virtual void EndOfEventAction(const G4Event *event);

	/**
	 * @brief Sets whether the current event should be retained for visualization.
	 * @param keep True to keep this event, false otherwise.
	 */
	void SetKeepEvent(bool keep);

  private:
	/// Flag indicating whether this event should be retained.
	bool fKeepThisEvent;
};
// ============================================================================

#endif