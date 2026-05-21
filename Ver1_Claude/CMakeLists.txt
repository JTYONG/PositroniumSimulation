cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(PositroniumSim VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Geant4 ─────────────────────────────────────────────────────────────────────
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

# ── ROOT ───────────────────────────────────────────────────────────────────────
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree)
include(${ROOT_USE_FILE})

# ── Sources ────────────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cc
    src/PsSimHit.cc
    src/PsSimDetectorConstruction.cc
    src/PsSimDetectorMessenger.cc
    src/PsSimSensitiveDetector.cc
    src/PsFormationProcess.cc
    src/PsSimFormationPhysics.cc
    src/PsFormationMessenger.cc
    src/PsSimPhysicsList.cc
    src/PsSimPrimaryGeneratorAction.cc
    src/PsSimRunAction.cc
    src/PsSimEventAction.cc
    src/PsSimOutputMessenger.cc
    src/PsSimActionInitialization.cc
)

# ── Executable ─────────────────────────────────────────────────────────────────
add_executable(PsSim ${SOURCES})

target_include_directories(PsSim
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${Geant4_INCLUDE_DIRS}
        ${ROOT_INCLUDE_DIRS}
)

target_link_libraries(PsSim
    PRIVATE
        ${Geant4_LIBRARIES}
        ROOT::Core
        ROOT::RIO
        ROOT::Tree
)

# ── Copy macro to build directory ──────────────────────────────────────────────
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sim.mac
               ${CMAKE_CURRENT_BINARY_DIR}/sim.mac COPYONLY)

# ── Create output directories in build tree ────────────────────────────────────
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/result_cache/root_file)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/result_cache/png)
