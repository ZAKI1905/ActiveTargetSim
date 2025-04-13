cmake_minimum_required(VERSION 3.16)

# Automatically detect the macOS SDK path.
execute_process(
    COMMAND xcrun --sdk macosx --show-sdk-path
    OUTPUT_VARIABLE MACOS_SDK_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set the sysroot to the detected path. This forces CMake to use that SDK.
set(CMAKE_OSX_SYSROOT "${MACOS_SDK_PATH}" CACHE STRING "macOS SDK path" FORCE)

# Use Homebrew LLVM compilers consistently.
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

# Set the project name and version.
project(ActiveTargetSim)

# Find Geant4 and include its settings.
find_package(Geant4 REQUIRED COMPONENTS ui_all vis_all)
include(${Geant4_USE_FILE})

# Find ROOT and include its settings.
find_package(ROOT REQUIRED COMPONENTS Core Hist Tree)
include(${ROOT_USE_FILE}) # This ensures ROOT_INCLUDE_DIRS and flags are set.

# Define the executable and its source files.
add_executable(active_target_sim
    src/main.cc
    src/DetectorConstruction.cc
    src/MuonSensitiveDetector.cc
    src/PrimaryGeneratorAction.cc
    src/RunAction.cc
    src/SteppingAction.cc
    src/TrackingAction.cc
)

# Include your headers.
target_include_directories(active_target_sim PUBLIC include)

# Link against Geant4 and ROOT libraries.
target_link_libraries(active_target_sim
    ${Geant4_LIBRARIES}
    ${ROOT_LIBRARIES}
)