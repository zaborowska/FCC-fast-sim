#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(main42)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC QUIET)
if(NOT HEPMC_FOUND)
  message(STATUS "G4 Examples: HepMC package not found. --> example_MyPythia example disabled")  
  return()
endif()

#----------------------------------------------------------------------------
# Find Pythia8 (required package)
#
find_package(Pythia8 QUIET)

if(PYTHIA8_FOUND)
  message(STATUS "G4 Examples: Pythia8 found. --> main42 with Pythia8 enabled.")
  add_definitions(-DG4LIB_USE_PYTHIA)
else()
  set(PYTHIA8_LIBRARY "")
  set(PYTHIA8_HEPMC_LIBRARY "")
  set(PYTHIA8_HAPDF_LIBRARY "")
endif()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
include_directories(${HEPMC_INCLUDE_DIR})
include_directories(${PYTHIA8_INCLUDE_DIR})
add_executable(main42 main42.cc)
target_link_libraries(main42
                      ${HEPMC_LIBRARIES} ${PYTHIA8_LIBRARY}
                      ${PYTHIA8_HEPMC_LIBRARY} ${PYTHIA8_HAPDF_LIBRARY} gcc)


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS main42 DESTINATION bin)

