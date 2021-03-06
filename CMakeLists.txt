#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(FCCfastsim)

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
  add_definitions(-DG4LIB_USE_PYTHIA8)
endif()

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# add a target to generate API documentation with Doxygen
#
find_package(Doxygen)

if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/FCCDoc/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Generating API documentation with Doxygen" VERBATIM
                      )
endif(DOXYGEN_FOUND)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include/FCCMain
                    ${PROJECT_SOURCE_DIR}/include/FCCGenerator
                    ${PROJECT_SOURCE_DIR}/include/FCCFastSim
                    ${PROJECT_SOURCE_DIR}/include/FCCFastSim/Atlfast
                    ${PROJECT_SOURCE_DIR}/include/FCCInfo
                    ${Geant4_INCLUDE_DIR}
                    ${HEPMC_INCLUDE_DIR}
                    ${PYTHIA8_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*/*.cc ${PROJECT_SOURCE_DIR}/src/*/*/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*/*.hh ${PROJECT_SOURCE_DIR}/include/*/*/*.hh)

#----------------------------------------------------------------------------
# Generate dictionary for input/output via HepMC ROOT files
#
ROOT_GENERATE_DICTIONARY(HepMC include/FCCGenerator/FCCHepMCClasses.hh  LINKDEF include/FCCGenerator/FCCHepMCLinkDef.h )
add_library(HepMCdict SHARED ${sources} HepMC.cxx)
target_link_libraries(HepMCdict ${HEPMC_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(FCCfastsim ${sources} ${headers})
target_link_libraries(FCCfastsim ${Geant4_LIBRARIES}
                      ${HEPMC_LIBRARIES}
                       ${PYTHIA8_LIBRARY}
                       ${PYTHIA8_HEPMC_LIBRARY} ${PYTHIA8_HAPDF_LIBRARY}
                       HepMCdict ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory
#
set(FCCfastsim_SCRIPTS
    pythia.in root.in vis.mac gun.in data/Atlfast_MuonResParam_CSC.dat
    data/Atlfast_PionResParam_DC1_NewUnits.dat data/Atlfast_ElectronBremParam_CSC.dat
    data/Atlfast_ElectronResParam_CSC.dat
  )

foreach(_script ${FCCfastsim_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS FCCfastsim DESTINATION bin)


