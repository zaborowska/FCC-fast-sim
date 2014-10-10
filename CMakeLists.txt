#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Par01)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC REQUIRED)

#----------------------------------------------------------------------------
# Find Pythia8 (required package)
#
find_package(Pythia8 REQUIRED)
if(PYTHIA8_FOUND)
  message(STATUS "G4 Examples: Pythia8 -- found.") 
  add_definitions(-DG4LIB_USE_PYTHIA8) 
else()
  message(STATUS "G4 Examples: Pythia8 -- NOT found.") 
  set(PYTHIA8_LIBRARY "")
  set(PYTHIA8_HEPMC_LIBRARY "")
  set(PYTHIA8_HAPDF_LIBRARY "")
endif()
#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# P01 requires GCCXML 
#
if(NOT GCCXML)
   message(FATAL_ERROR "P01 requires GCCXML")
endif() 
#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${HEPMC_INCLUDE_DIR}
                    ${PYTHIA8_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Generate dictionaries, add ROOT libraries properties
#
REFLEX_GENERATE_DICTIONARY(HepMCDict include/HepMCClasses.hh  SELECTION xml/HepMC_selection.xml )
add_library(myHepMC SHARED ${sources} HepMCDict.cpp)
set(libsuffix .so)
set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
set_target_properties(myHepMC PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
target_link_libraries(myHepMC ${HEPMC_LIBRARIES} ${ROOT_LIBRARIES} Cintex Reflex)

REFLEX_GENERATE_DICTIONARY(ExP01Classes include/ExP01Classes.hh  SELECTION xml/hits_selection.xml )
add_library(ExP01ClassesDict SHARED ${sources} ExP01Classes.cpp)
set(libsuffix .so)
set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
set_target_properties(ExP01ClassesDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
target_link_libraries(ExP01ClassesDict ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Cintex Reflex)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(examplePar01 examplePar01.cc ${sources} ${headers})
target_link_libraries(examplePar01 ${Geant4_LIBRARIES}
                      ${HEPMC_LIBRARIES}
                       ${PYTHIA8_LIBRARY}
                       ${PYTHIA8_HEPMC_LIBRARY} ${PYTHIA8_HAPDF_LIBRARY}
                       myHepMC ExP01ClassesDict ${ROOT_LIBRARIES} Cintex Reflex)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build Par01. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(Par01_SCRIPTS
    pythia.in root.in ascii.in vis.mac PythiaInHepMC.root
  )

foreach(_script ${Par01_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS examplePar01 DESTINATION bin)


