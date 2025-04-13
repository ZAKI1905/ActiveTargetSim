// ============================================================================
//  File: DetectorConstruction.hh
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Defines detector geometry classes for simulating particle transport
//           through muon production, moderation, and magnetic field steering setups.
//
//  Author: Mohammadreza Zakeri (Zaki)
//  Email : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================
#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4LogicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <string>
#include <vector>

// ============================================================================
// DetectorConstruction Class Declaration
// ============================================================================
/**
 * @class DetectorConstruction
 * @brief A modular geometry builder for GEANT4 simulations focused on muon-target interaction studies,
 * including magnetic field configurations for analyzing muon stopping behavior and confinement.
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
	DetectorConstruction();
	virtual ~DetectorConstruction();

	/**
	 * @brief Constructs the physical volumes of the detector.
	 * @return Pointer to the constructed physical volume.
	 */
	virtual G4VPhysicalVolume *Construct() override;

	// ==== Public Methods ====

	/**
	 * @brief Sets the type of detector to be constructed.
	 * @param type The type of detector (e.g., "carbonStack", "alternatingLayers", "muonTarget").
	 */
	void SetDetectorType(const G4String &type);

	/**
	 * @brief Access to the scoring volume.
	 * @return Pointer to the logical volume used for scoring.
	 */
	G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

	/**
	 * @brief Accessor for the target volume corresponding to a given index.
	 * @param n Index of the requested target volume (e.g., plate or layer index).
	 * @return Pointer to the logical volume of the nth target, or nullptr if out of range.
	 */
	G4LogicalVolume *GetTargetNVolume(size_t n) const;

	/**
	 * @brief Get the number of individual target layers or plates in the current detector configuration.
	 * @return Total number of target volumes (e.g., carbon plates, converters, etc.).
	 */
	size_t GetNumTargetVolumes() const;

  private:
	// ==== Private Members ====

	G4LogicalVolume *fScoringVolume = nullptr;
	G4String fDetectorType = "carbonStack"; // default

	// Target layers or absorbers for muon interaction and diagnostics
	std::vector<G4LogicalVolume *> fTargetVolumes;

	// List of physical volumes for each layer/plate
	// Corresponding placements used for Z-position diagnostics
	std::vector<G4VPhysicalVolume *> fTargetPlacements;

	/**
	 * @brief Constructs a carbon stack target configuration.
	 * @return Pointer to the constructed physical volume for the carbon stack.
	 * This method defines a detector layout for analyzing muon production and stopping behavior.
	 * Includes optional magnetic field configuration for trajectory steering and spatial diagnostics.
	 */
	G4VPhysicalVolume *ConstructCarbonStack();

	/**
	 * @brief Constructs an alternating layers target configuration.
	 * @return Pointer to the constructed physical volume for alternating layers.
	 * This method defines a detector layout for analyzing muon production and stopping behavior.
	 * Includes optional magnetic field configuration for trajectory steering and spatial diagnostics.
	 */
	G4VPhysicalVolume *ConstructAlternatingLayers();

	/**
	 * @brief Constructs a muon target configuration.
	 * @return Pointer to the constructed physical volume for the muon target.
	 * This method defines a detector layout for analyzing muon production and stopping behavior.
	 * Includes optional magnetic field configuration for trajectory steering and spatial diagnostics.
	 */
	G4VPhysicalVolume *ConstructMuonTarget();
};
// ============================================================================

#endif