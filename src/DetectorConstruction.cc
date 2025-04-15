// ============================================================================
//  File   : DetectorConstruction.cc
//  Project: ActiveTargetSim - Muon-Catalyzed Fusion Simulation Toolkit
//  Purpose: Defines geometry and material configuration for various detector
//           setups including carbon stacks, alternating absorbers, and muon
//           production targets. Also defines a uniform magnetic field
//           to study muon steering and stopping behavior.
//
//  Author : Mohammadreza Zakeri (Zaki)
//  Email  : m.zakeri@eku.edu
//  Created: 2025-04-02
// ============================================================================

#include "DetectorConstruction.hh"
#include "MuonSensitiveDetector.hh"

#include <sstream>

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4UniformMagField.hh"

// ============================================================================
// Constructor & Destructor
// ============================================================================

/**
 * @brief Constructor
 *
 * Initializes member variables. No geometry is built here.
 */
DetectorConstruction::DetectorConstruction()
{
	// Nothing to initialize at construction
}

/**
 * @brief Destructor
 *
 * Cleans up any allocated resources (none explicitly used).
 */
DetectorConstruction::~DetectorConstruction()
{
	// Nothing to delete explicitly
}

// ============================================================================
// Public Method: SetDetectorType
// ============================================================================

/**
 * @brief Set the geometry configuration for the detector.
 *
 * @param type Name of the desired detector setup (e.g., "muonTarget").
 */
void DetectorConstruction::SetDetectorType(const G4String &type)
{
	fDetectorType = type;
}

// ============================================================================
// Public Method: Construct
// ============================================================================

/**
 * @brief Main entry point for building the detector geometry.
 *
 * Delegates geometry construction to the appropriate private method based on
 * the user-selected type (muon target, carbon stack, alternating layers, or open muon target).
 *
 * The selected geometry is controlled by the value of `fDetectorType`, which may be set
 * via macro command or code.
 *
 * Supported detector types:
 * - "muonTarget"        → Compact graphite + stacked converter layers (ConstructStackedTargetGeometry)
 * - "openMuonTarget"    → Graphite + spaced converter layers with air gaps (ConstructOpenMuonTarget)
 * - "carbonStack"       → Carbon-only layers (ConstructCarbonStack)
 * - "alternatingLayers" → Alternating graphite/tungsten (ConstructAlternatingLayers)
 *
 * @return Pointer to the top-level physical volume in the geometry hierarchy.
 */
G4VPhysicalVolume *DetectorConstruction::Construct()
{
	if (fDetectorType == "muonTarget")
	{
		return ConstructStackedTargetGeometry();
	}
	else if (fDetectorType == "openMuonTarget")
	{
		return ConstructOpenMuonTarget();
	}
	else if (fDetectorType == "carbonStack")
	{
		return ConstructCarbonStack();
	}
	else if (fDetectorType == "alternatingLayers")
	{
		return ConstructAlternatingLayers();
	}
	else
	{
		G4Exception("DetectorConstruction::Construct()", "InvalidSetup", FatalException,
					("Unknown detector type: " + fDetectorType).c_str());
		return nullptr;
	}
}

// ============================================================================
// Public Method: GetTargetNVolume
// ============================================================================

/**
 * @brief Retrieves a pointer to the i-th target logical volume.
 *
 * @param n Index of the desired target layer.
 * @return Pointer to the G4LogicalVolume of the target layer.
 */
G4LogicalVolume *DetectorConstruction::GetTargetNVolume(size_t n) const
{
	if (n < fTargetVolumes.size())
		return fTargetVolumes[n];
	else
		return nullptr;
}

// ============================================================================
// Public Method: GetNumTargetVolumes
// ============================================================================

/**
 * @brief Returns the total number of target volumes.
 *
 * @return Number of target layers in the detector.
 */
size_t DetectorConstruction::GetNumTargetVolumes() const
{
	return fTargetVolumes.size();
}

// ============================================================================
// Private Method: ConstructCarbonStack
// ============================================================================

/**
 * @brief Creates a stack of carbon plates for muon traversal studies.
 *
 * This method constructs multiple carbon plates separated by gaps. Each plate
 * is tracked for later scoring. The last plate is marked as the scoring volume.
 * A uniform magnetic field is applied in the +Z direction for guiding particle transport.
 *
 * @return Pointer to the world volume containing the geometry.
 */
G4VPhysicalVolume *DetectorConstruction::ConstructCarbonStack()
{
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *carbon = nist->FindOrBuildMaterial("G4_C");

	if (!carbon)
	{
		G4Exception("DetectorConstruction::ConstructCarbonStack()", "InvalidMaterial",
					FatalException, "Material G4_C not found!");
	}

	G4double worldSize = 1.0 * m;
	auto solidWorld = new G4Box("World", worldSize / 2, worldSize / 2, worldSize / 2);
	auto logicWorld = new G4LogicalVolume(solidWorld, air, "World");
	auto physWorld = new G4PVPlacement(nullptr, {}, logicWorld, "World", nullptr, false, 0);

	const int nPlates = 5;
	G4double plateThickness = 2.0 * mm;
	G4double gap = 10.0 * mm;
	G4double totalLength = nPlates * plateThickness + (nPlates - 1) * gap;
	G4double startZ = -totalLength / 2 + plateThickness / 2;

	for (int i = 0; i < nPlates; ++i)
	{
		G4double posZ = startZ + i * (plateThickness + gap);
		G4String name = "Plate_" + std::to_string(i);

		auto solid = new G4Box(name, worldSize / 4, worldSize / 4, plateThickness / 2);
		auto logic = new G4LogicalVolume(solid, carbon, name);
		auto phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, posZ), logic, name, logicWorld, false, i);
		fTargetPlacements.push_back(phys);

		fTargetVolumes.push_back(logic);

		if (i == nPlates - 1)
		{
			fScoringVolume = logic;
		}
	}

	if (fScoringVolume)
	{
		auto sdManager = G4SDManager::GetSDMpointer();
		auto muonSD = new MuonSensitiveDetector("MuonSD");
		sdManager->AddNewDetector(muonSD);
		fScoringVolume->SetSensitiveDetector(muonSD);
	}

	// Apply uniform magnetic field along Z to simulate muon guidance or confinement
	auto field = new G4UniformMagField(G4ThreeVector(0., 0., 1.0 * tesla));
	auto fieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
	fieldManager->SetDetectorField(field);
	fieldManager->CreateChordFinder(field);

	G4cout << "=== Target Layer Summary ===" << G4endl;
	for (size_t i = 0; i < fTargetVolumes.size(); ++i)
	{
		auto *logic = fTargetVolumes[i];
		auto *phys = fTargetPlacements[i]; // matches 1:1 with logic
		G4ThreeVector pos = phys->GetTranslation();
		G4cout << "Target " << i
			   << " | Z = " << pos.z() / mm << " mm"
			   << " | Material = " << logic->GetMaterial()->GetName()
			   << G4endl;
	}

	return physWorld;
}

// ============================================================================
// Private Method: ConstructAlternatingLayers
// ============================================================================

/**
 * @brief Builds alternating layers of tungsten (absorber) and graphite (moderator).
 *
 * Designed to simulate cascaded slowing down or absorption. The last graphite
 * layer is used as the scoring volume.
 * A uniform magnetic field is applied in the +Z direction for guiding particle transport.
 *
 * @return Pointer to the world volume.
 */
G4VPhysicalVolume *DetectorConstruction::ConstructAlternatingLayers()
{
	auto nist = G4NistManager::Instance();
	auto tungsten = nist->FindOrBuildMaterial("G4_W");
	auto graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
	auto air = nist->FindOrBuildMaterial("G4_AIR");

	G4double worldSize = 30 * cm;
	auto solidWorld = new G4Box("World", worldSize / 2, worldSize / 2, worldSize / 2);
	auto logicWorld = new G4LogicalVolume(solidWorld, air, "WorldLV");
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	auto physWorld = new G4PVPlacement(0, {}, logicWorld, "World", nullptr, false, 0);

	const G4int nLayers = 5;
	G4double tungstenThickness = 1.0 * mm;
	G4double graphiteThickness = 2.0 * mm;
	G4double zPos = -nLayers * (tungstenThickness + graphiteThickness) / 2;

	for (int i = 0; i < nLayers; ++i)
	{
		auto solidW = new G4Box("Tungsten", 5 * cm, 5 * cm, tungstenThickness / 2);
		auto logicW = new G4LogicalVolume(solidW, tungsten, "TungstenLV");
		new G4PVPlacement(0, G4ThreeVector(0, 0, zPos + tungstenThickness / 2),
						  logicW, "Tungsten", logicWorld, false, i);
		logicW->SetVisAttributes(new G4VisAttributes(G4Colour::Grey()));
		fTargetVolumes.push_back(logicW);
		zPos += tungstenThickness;

		auto solidC = new G4Box("Graphite", 5 * cm, 5 * cm, graphiteThickness / 2);
		auto logicC = new G4LogicalVolume(solidC, graphite, "GraphiteLV");
		new G4PVPlacement(0, G4ThreeVector(0, 0, zPos + graphiteThickness / 2),
						  logicC, "Graphite", logicWorld, false, i + 100);
		logicC->SetVisAttributes(new G4VisAttributes(G4Colour::Brown()));
		fTargetVolumes.push_back(logicC);
		zPos += graphiteThickness;

		if (i == nLayers - 1)
		{
			fScoringVolume = logicC;
		}
	}

	if (fScoringVolume)
	{
		auto sdManager = G4SDManager::GetSDMpointer();
		auto muonSD = new MuonSensitiveDetector("MuonSD");
		sdManager->AddNewDetector(muonSD);
		fScoringVolume->SetSensitiveDetector(muonSD);
	}

	// ------------------------------
	// Add D-T Gas Region with Local Magnetic Field
	// ------------------------------

	// Define elements
	G4Element *elD = new G4Element("Deuterium", "D", 1., 2.014 * g / mole);
	G4Element *elT = new G4Element("Tritium", "T", 1., 3.016 * g / mole);

	// Define D-T gas material
	G4double DT_density = 0.000179 * g / cm3;
	G4double DT_temp = 293.15 * kelvin;
	G4double DT_pressure = 1.0 * atmosphere;

	G4Material *DTGas = new G4Material("DTGas", DT_density, 2, kStateGas, DT_temp, DT_pressure);
	DTGas->AddElement(elD, 1);
	DTGas->AddElement(elT, 1);

	// Geometry and placement
	G4double DT_thickness = 10.0 * cm;
	G4double DT_width = 5.0 * cm;
	G4double DT_height = 5.0 * cm;
	G4double DT_zPos = zPos + DT_thickness / 2.0; // zPos from the end of tungsten/graphite loop

	auto solidDT = new G4Box("DTGasBox", DT_width / 2.0, DT_height / 2.0, DT_thickness / 2.0);
	auto logicDT = new G4LogicalVolume(solidDT, DTGas, "DTGasLogical");

	G4cout << "[DEBUG] D-T gas region placed at Z = " << DT_zPos / mm << " mm" << G4endl;

	new G4PVPlacement(0, G4ThreeVector(0, 0, DT_zPos), logicDT, "DTGasPhysical", logicWorld, false, 0);
	logicDT->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 1.0, 1.0))); // Cyan

	// Local magnetic field inside D-T gas
	auto localField = new G4UniformMagField(G4ThreeVector(0., 0., 1.0 * tesla));
	auto localFieldManager = new G4FieldManager();
	localFieldManager->SetDetectorField(localField);
	localFieldManager->CreateChordFinder(localField);

	// Assign field ONLY to D-T gas
	logicDT->SetFieldManager(localFieldManager, true);

	// (optional) make this the scoring volume if desired
	// fScoringVolume = logicDT;

	return physWorld;
}

// ============================================================================
// Private Method: ConstructStackedTargetGeometry
// ============================================================================

/**
 * @brief Creates a compact muon production geometry: a thin graphite target followed by closely stacked tungsten layers.
 *
 * This layout is suitable for studying pion creation and muon stopping in a dense converter stack. It places the D-T gas region
 * downstream of the converter and applies a localized magnetic field to guide muons. This configuration emphasizes compactness
 * over muon escape efficiency.
 *
 * @return Pointer to the constructed physical volume.
 */
G4VPhysicalVolume *DetectorConstruction::ConstructStackedTargetGeometry()
{
	auto nist = G4NistManager::Instance();
	auto air = nist->FindOrBuildMaterial("G4_AIR");
	auto graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
	auto tungsten = nist->FindOrBuildMaterial("G4_W");

	G4double worldSize = 50 * cm;
	auto solidWorld = new G4Box("World", worldSize / 2, worldSize / 2, worldSize / 2);
	auto logicWorld = new G4LogicalVolume(solidWorld, air, "WorldLV");
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	auto physWorld = new G4PVPlacement(0, {}, logicWorld, "World", nullptr, false, 0);

	G4double targetX = 5 * cm;
	G4double targetY = 5 * cm;
	G4double targetThickness = 1.0 * mm;
	G4double converterThickness = 3.0 * mm;
	const G4int numConverters = 5;

	auto solidTarget = new G4Box("ProtonTarget", targetX / 2, targetY / 2, targetThickness / 2);
	auto logicTarget = new G4LogicalVolume(solidTarget, graphite, "ProtonTargetLV");
	new G4PVPlacement(0, G4ThreeVector(0, 0, -10 * cm), logicTarget, "ProtonTarget", logicWorld, false, 0);
	logicTarget->SetVisAttributes(new G4VisAttributes(G4Colour::Brown()));

	fScoringVolume = logicTarget;

	G4double startZ = -10 * cm + targetThickness + 1.0 * mm;
	for (G4int i = 0; i < numConverters; ++i)
	{
		G4String name = "Converter_" + std::to_string(i);
		auto solidConv = new G4Box(name, targetX / 2, targetY / 2, converterThickness / 2);
		auto logicConv = new G4LogicalVolume(solidConv, tungsten, name + "_LV");

		G4double zPos = startZ + i * (converterThickness + 1.0 * mm);
		new G4PVPlacement(0, G4ThreeVector(0, 0, zPos), logicConv, name, logicWorld, false, i + 1);
		logicConv->SetVisAttributes(new G4VisAttributes(G4Colour::Grey()));
		fTargetVolumes.push_back(logicConv);
	}

	if (fScoringVolume)
	{
		auto sdManager = G4SDManager::GetSDMpointer();
		auto muonSD = new MuonSensitiveDetector("MuonSD");
		sdManager->AddNewDetector(muonSD);
		fScoringVolume->SetSensitiveDetector(muonSD);
	}

	// ------------------------------
	// Add D-T Gas Region with Local Magnetic Field
	// ------------------------------

	// Define elements
	G4Element *elD = new G4Element("Deuterium", "D", 1., 2.014 * g / mole);
	G4Element *elT = new G4Element("Tritium", "T", 1., 3.016 * g / mole);

	// Define D-T gas material
	G4double DT_density = 0.000179 * g / cm3;
	G4double DT_temp = 293.15 * kelvin;
	G4double DT_pressure = 1.0 * atmosphere;

	G4Material *DTGas = new G4Material("DTGas", DT_density, 2, kStateGas, DT_temp, DT_pressure);
	DTGas->AddElement(elD, 1);
	DTGas->AddElement(elT, 1);

	// Dimensions
	G4double DT_thickness = 10.0 * cm;
	G4double DT_width = targetX;
	G4double DT_height = targetY;

	// Calculate last converter Z position
	G4double lastConverterZ = startZ + (numConverters - 1) * (converterThickness + 1.0 * mm);
	G4double DT_zPos = lastConverterZ + converterThickness / 2 + DT_thickness / 2 + 1.0 * mm;

	fDTZCenter = DT_zPos;					  //  Z-center of D-T gas region (midpoint).
	fDTZStart = DT_zPos - DT_thickness / 2.0; // Z-start of D-T gas region (front face).
	fDTZEnd = DT_zPos + DT_thickness / 2.0;	  // Z-end of D-T gas region (back face).

	auto solidDT = new G4Box("DTGasBox", DT_width / 2.0, DT_height / 2.0, DT_thickness / 2.0);
	auto logicDT = new G4LogicalVolume(solidDT, DTGas, "DTGasLogical");

	new G4PVPlacement(0, G4ThreeVector(0, 0, DT_zPos), logicDT, "DTGasPhysical", logicWorld, false, 0);
	logicDT->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 1.0, 1.0))); // Cyan

	// Local magnetic field inside D-T gas
	auto localField = new G4UniformMagField(G4ThreeVector(0., 0., 1.0 * tesla));
	auto localFieldManager = new G4FieldManager();
	localFieldManager->SetDetectorField(localField);
	localFieldManager->CreateChordFinder(localField);

	// Assign field ONLY to D-T gas
	logicDT->SetFieldManager(localFieldManager, true);

	// Debug output (optional)
	G4cout << "[DEBUG] D-T gas region placed at Z = " << DT_zPos / mm << " mm" << G4endl;

	return physWorld;
}

// ============================================================================
// Private Method: ConstructOpenMuonTarget
// ============================================================================
/**
 * @brief Creates an open muon production geometry: graphite target followed by a gradient stack of tungsten converters.
 *
 * This version implements six tungsten plates with decreasing thickness:
 *   {2.0, 0.5, 0.3, 0.3, 0.2, 0.2} mm, each separated by 5 mm air gaps.
 * The D-T gas region is placed 10 mm downstream of the last plate and is embedded in a uniform magnetic field.
 * This layout aims to optimize pion slowing early, while allowing downstream muon creation and escape toward the D-T region.
 *
 * @return Pointer to the constructed physical world volume.
 */
G4VPhysicalVolume *DetectorConstruction::ConstructOpenMuonTarget()
{
	auto nist = G4NistManager::Instance();
	auto air = nist->FindOrBuildMaterial("G4_AIR");
	auto graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
	auto tungsten = nist->FindOrBuildMaterial("G4_W");

	G4double worldSize = 60 * cm;
	auto solidWorld = new G4Box("World", worldSize / 2, worldSize / 2, worldSize / 2);
	auto logicWorld = new G4LogicalVolume(solidWorld, air, "WorldLV");
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	auto physWorld = new G4PVPlacement(0, {}, logicWorld, "World", nullptr, false, 0);

	// Target dimensions
	G4double targetX = 5 * cm;
	G4double targetY = 5 * cm;
	G4double targetThickness = 1.0 * mm;
	G4double gap = 5.0 * mm;

	// Converter thicknesses in gradient stack
	std::vector<G4double> thicknesses = {
		1.0 * mm, 0.5 * mm, 0.5 * mm
		// ,  0.5 * mm, 0.2 * mm, 0.1 * mm
	};

	// --- Graphite proton target ---
	auto solidTarget = new G4Box("ProtonTarget", targetX / 2, targetY / 2, targetThickness / 2);
	auto logicTarget = new G4LogicalVolume(solidTarget, graphite, "ProtonTargetLV");
	new G4PVPlacement(0, G4ThreeVector(0, 0, -10 * cm), logicTarget, "ProtonTarget", logicWorld, false, 0);
	logicTarget->SetVisAttributes(new G4VisAttributes(G4Colour::Brown()));
	fScoringVolume = logicTarget;

	// --- Gradient tungsten stack ---
	G4double zPos = -10 * cm + targetThickness + 1.0 * mm;
	for (size_t i = 0; i < thicknesses.size(); ++i)
	{
		G4double t = thicknesses[i];
		G4String name = "Converter_" + std::to_string(i);

		auto solidConv = new G4Box(name, targetX / 2, targetY / 2, t / 2);
		auto logicConv = new G4LogicalVolume(solidConv, tungsten, name + "_LV");

		zPos += t / 2.0;
		new G4PVPlacement(0, G4ThreeVector(0, 0, zPos), logicConv, name, logicWorld, false, i + 1);
		logicConv->SetVisAttributes(new G4VisAttributes(G4Colour::Grey()));
		fTargetVolumes.push_back(logicConv);

		zPos += t / 2.0 + gap;
	}

	// --- D-T Gas Region ---
	G4Element *elD = new G4Element("Deuterium", "D", 1., 2.014 * g / mole);
	G4Element *elT = new G4Element("Tritium", "T", 1., 3.016 * g / mole);
	G4double DT_density = 0.000179 * g / cm3;
	G4Material *DTGas = new G4Material("DTGas", DT_density, 2, kStateGas, 293.15 * kelvin, 1.0 * atmosphere);
	DTGas->AddElement(elD, 1);
	DTGas->AddElement(elT, 1);

	G4double DT_thickness = 10.0 * cm;
	G4double DT_width = 10.0 * cm;
	G4double DT_height = 10.0 * cm;
	G4double DT_zPos = zPos + 10.0 * mm + DT_thickness / 2.0; // Increased post-converter gap

	fDTZCenter = DT_zPos;
	fDTZStart = DT_zPos - DT_thickness / 2.0;
	fDTZEnd = DT_zPos + DT_thickness / 2.0;

	auto solidDT = new G4Box("DTGasBox", DT_width / 2.0, DT_height / 2.0, DT_thickness / 2.0);
	auto logicDT = new G4LogicalVolume(solidDT, DTGas, "DTGasLogical");
	new G4PVPlacement(0, G4ThreeVector(0, 0, DT_zPos), logicDT, "DTGasPhysical", logicWorld, false, 0);
	logicDT->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 1.0, 1.0))); // Cyan

	// Magnetic field in D-T region
	auto localField = new G4UniformMagField(G4ThreeVector(0., 0., 1.0 * tesla));
	auto localFieldManager = new G4FieldManager();
	localFieldManager->SetDetectorField(localField);
	localFieldManager->CreateChordFinder(localField);
	logicDT->SetFieldManager(localFieldManager, true);

	G4cout << "[DEBUG] D-T gas region (gradient converter geometry) placed at Z = " << DT_zPos / mm << " mm" << G4endl;

	// --- Sensitive Detector ---
	if (fScoringVolume)
	{
		auto sdManager = G4SDManager::GetSDMpointer();
		auto muonSD = new MuonSensitiveDetector("MuonSD");
		sdManager->AddNewDetector(muonSD);
		fScoringVolume->SetSensitiveDetector(muonSD);
	}

	return physWorld;
}
// ============================================================================