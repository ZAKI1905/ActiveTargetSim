# ActiveTargetSim: Muon Production and Stopping Target Simulation

**Author:** Mohammadreza Zakeri (Zaki)  
**Contact:** m.zakeri@eku.edu  
**Created:** April 2025  
**Simulation Framework:** [Geant4](https://geant4.web.cern.ch/)

---

## Overview

**ActiveTargetSim** is a Geant4-based simulation toolkit designed to study the generation, transport, and stopping behavior of muons in a layered target geometry. The simulation enables controlled testing of muon-producing targets and potential moderation schemes, supporting future work in muon-catalyzed fusion and related areas of applied particle physics.

The code allows for flexible target geometries, including:

- **Thin graphite targets** for primary proton interactions
- **Tungsten converter stacks** to produce and absorb muons
- Optional **carbon stack** and **alternating moderator-absorber configurations**

The simulation tracks:

- Muon creation energy
- Stopping location (in space and in material)
- Radial displacement from beam axis
- Target layer index of final muon stop

---

## Key Features

- Configurable geometry:
  - Thin graphite target for primary proton collisions
  - Stacked tungsten converter layers
  - Alternate configurations include carbon stacks and graphite-tungsten sandwich targets
- Uniform magnetic field aligned with beam axis (z-direction)
- ROOT-based histogramming of muon kinematic and spatial observables:
  - Creation energy
  - Stopping z-position
  - Radial stop distance
  - Target layer index of final stop
- G4SmoothTrajectory-based visualization
- Debug-mode output for muon creation/stopping diagnostics

---

## Detector Geometry (Figure 1)

![Detector Geometry](figs/muon_target_geom.png)

**Figure 1**: _Perspective view of the detector geometry in the ActiveTargetSim simulation. The five grey slabs represent tungsten converter layers used for muon production and stopping analysis. The thin brown slab upstream (rightmost slab in image) is the primary graphite target where incoming protons interact to produce secondary particles including muons._

---

## Output Histograms

Generated using ROOT via Geant4’s G4AnalysisManager. Example plots below were created from 1000 events.

| Histogram              | Description                                         |
| ---------------------- | --------------------------------------------------- |
| **MuonEnergy.pdf**     | Distribution of muon creation energies (in MeV)     |
| **MuonStopZ.pdf**      | Longitudinal stopping positions of muons (in mm)    |
| **MuonStopRadius.pdf** | Radial stopping distance from beam axis (in mm)     |
| **MuonStopTarget.pdf** | Histogram of which target index the muon stopped in |

Example:

![Muon Creation Energy](Hists/MuonEnergy.png)

**Figure 2**: Histogram of muon creation energies.

---

## How to Build

Ensure that you have Geant4 (with data libraries and environment variables properly set) and optionally Graphviz (for Doxygen diagrams) installed.

```bash
mkdir build && cd build
cmake ..
make
```

---

## How to Run

### Interactive Mode (with Visualization)

```bash
./active_target_sim
```

The simulation uses the vis.mac file to render geometry and track visuals.

### Batch Mode

```bash
./active_target_sim run.mac
```

---

## Generating Documentation

If Doxygen is installed, you can generate HTML and PDF documentation with:

```bash
doxygen Doxyfile
```

Output will be available in the doc/html and doc/latex directories:

    •	HTML output: doc/html/index.html
    •	LaTeX output: doc/latex/refman.pdf

Make sure Graphviz is installed for diagram support.

---

## Physics Motivation

This simulation explores the production and moderation of muons via primary proton beams. A graphite target initiates pion production, and pions decay into muons. High-Z materials like tungsten are used to efficiently capture and stop muons due to their short radiation length and high density. The configuration also allows insertion of magnetic fields to steer particles, paving the way for future optimization or staged moderation.

Though designed generically, the setup is relevant to R&D efforts in muon-related technologies such as:
• Muon beam production
• Particle stopping optimization
• Muon-catalyzed fusion system design (future use)

---

## Future Directions

This simulation serves as a foundation for exploring detector layouts and beamline optimization in muon-related applications. While the code is not specific to muon-catalyzed fusion, its modularity supports extensions toward that direction.

---

## License

This project is open for academic and research use. If you use this code in your work, please cite this repository or contact the author.
