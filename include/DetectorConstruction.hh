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

	/**
	 * @brief Returns the Z-position of the front face of the D-T gas region.
	 *
	 * This marks the beginning (lowest Z value) of the D-T fusion volume,
	 * used for histogram range and stopping diagnostics.
	 *
	 * @return Z-position [G4double] in global coordinates.
	 */
	G4double GetDTZStart() const { return fDTZStart; }

	/**
	 * @brief Returns the Z-position of the back face of the D-T gas region.
	 *
	 * This marks the end (highest Z value) of the D-T fusion volume,
	 * used for bounding region in scoring and plotting.
	 *
	 * @return Z-position [G4double] in global coordinates.
	 */
	G4double GetDTZEnd() const { return fDTZEnd; }

	/**
	 * @brief Returns the Z-position of the center of the D-T gas region.
	 *
	 * Useful for visualization, debugging, or positioning detectors relative
	 * to the D-T gas volume.
	 *
	 * @return Z-position [G4double] in global coordinates.
	 */
	G4double GetDTZCenter() const { return fDTZCenter; }

  private:
	// ==== Private Members ====

	/**
	 * @brief Z-start of D-T gas region (front face).
	 */
	G4double fDTZStart = 0.;

	/**
	 * @brief Z-end of D-T gas region (back face).
	 */
	G4double fDTZEnd = 0.;

	/**
	 * @brief Z-center of D-T gas region (midpoint).
	 */
	G4double fDTZCenter = 0.;

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
	 * @brief Constructs the compact muon target geometry with a graphite target and stacked tungsten converters.
	 *
	 * This configuration simulates a high-density design where the proton beam impacts a graphite target
	 * followed by closely packed tungsten converter layers. Useful for benchmarking pion production and
	 * in-place muon generation in a confined geometry. Includes optional D-T gas region with local magnetic field.
	 *
	 * @return Pointer to the constructed physical volume.
	 */
	G4VPhysicalVolume *ConstructStackedTargetGeometry();

	/**
	 * @brief Constructs an open muon target geometry with interleaved converter layers and air gaps.
	 *
	 * This configuration simulates a low-density, decay-friendly geometry where the proton beam hits a graphite
	 * target, followed by spaced tungsten plates allowing pion decay and improved muon escape. Designed to increase
	 * muon throughput to the D-T fusion region for muon-catalyzed fusion studies.
	 *
	 * @return Pointer to the constructed physical volume.
	 */
	G4VPhysicalVolume *ConstructOpenMuonTarget();
};
// ============================================================================

#endif