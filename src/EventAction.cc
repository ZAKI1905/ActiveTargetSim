// ============================================================================
//  File: EventAction.cc
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

#include "EventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
// ============================================================================
// EventAction Class Implementation
// ============================================================================
/**
 * @brief Constructor initializes the internal flag.
 */
EventAction::EventAction()
	: G4UserEventAction(), fKeepThisEvent(false)
{
}
// ----------------------------------------------------------------------------
/**
 * @brief Destructor.
 */
EventAction::~EventAction() {}
// ----------------------------------------------------------------------------
/**
 * @brief Called at the beginning of each event.
 * Resets the internal event retention flag.
 * @param event Pointer to the current event.
 */
void EventAction::BeginOfEventAction(const G4Event *)
{
	// Reset event retention flag
	fKeepThisEvent = false;
}
// ----------------------------------------------------------------------------
/**
 * @brief Called at the end of each event.
 * Instructs Geant4 to keep the event if flagged.
 * @param event Pointer to the current event.
 */
void EventAction::EndOfEventAction(const G4Event *)
{
	// Only keep the event if it was flagged
	if (fKeepThisEvent)
	{
		G4EventManager::GetEventManager()->KeepTheCurrentEvent();
	}
	// Otherwise: do NOT call it at all
}

// ----------------------------------------------------------------------------
/**
 * @brief Sets whether the current event should be retained for visualization.
 * @param keep True to keep this event, false otherwise.
 */
void EventAction::SetKeepEvent(bool keep)
{
	fKeepThisEvent = keep;
}
// ----------------------------------------------------------------------------

// ============================================================================
