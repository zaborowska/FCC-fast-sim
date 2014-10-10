#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(4Reflex)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)
if(NOT GCCXML)
   message(FATAL_ERROR " requires GCCXML")
endif()
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

REFLEX_GENERATE_DICTIONARY(HepMCDict HepMCClasses.h  SELECTION selection.xml )
add_library(myHepMC SHARED ${sources} HepMCDict.cpp)
set(libsuffix .so)
set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
set_target_properties(myHepMC PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
target_link_libraries(myHepMC   ${HEPMC_LIBRARIES} ${ROOT_LIBRARIES} Cintex Reflex)


add_executable(main main.cxx ${sources} ${headers})
target_link_libraries(main ${ROOT_LIBRARIES} ${HEPMC_LIBRARIES} ${PYTHIA8_LIBRARY} ${PYTHIA8_HEPMC_LIBRARY}  ${PYTHIA8_HAPDF_LIBRARY}
                     myHepMC Cintex Reflex )

install(TARGETS main DESTINATION ${PROJECT_SOURCE_DIR}/build)
