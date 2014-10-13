#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Pythia8ToHepMCroot)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)
#----------------------------------------------------------------------------
# Find Pythia8 (required package)
#
find_package(Pythia8 REQUIRED)

if(PYTHIA8_FOUND)
  message(STATUS "Pythia8 found.")
  add_definitions(-DG4LIB_USE_PYTHIA)
endif()
#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC REQUIRED)
if(HEPMC_FOUND)
  message(STATUS "HepMC package found.")
endif()

include_directories(${PROJECT_SOURCE_DIR}/.
                ${HEPMC_INCLUDE_DIR}
                ${ROOT_INCLUDE_DIR}
                ${PYTHIA8_INCLUDE_DIR}
)
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.h)

ROOT_GENERATE_DICTIONARY(HepMC HepMCClasses.h  LINKDEF HepMCLinkDef.h )
add_library(HepMCdict SHARED ${sources} HepMC.cxx)
target_link_libraries(HepMCdict   ${HEPMC_LIBRARIES} ${ROOT_LIBRARIES})


add_executable(main main.cxx ${sources} ${headers})
target_link_libraries(main ${ROOT_LIBRARIES} ${HEPMC_LIBRARIES} ${PYTHIA8_LIBRARY} ${PYTHIA8_HEPMC_LIBRARY}  ${PYTHIA8_HAPDF_LIBRARY}
                     HepMCdict )

install(TARGETS main DESTINATION ${PROJECT_SOURCE_DIR}/build)
