FCC-fast-sim
============

Geant 4 based fast simulation

____________________________________________________________________

1. Installation
-------------------

        mkdir build
        cd build
        source /afs/cern.ch/sw/lcg/external/geant4/10.1/x86_64-slc6-gcc49-opt/CMake-setup.sh
        source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.25/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh
        export PYTHIA8DATA=/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt67b/pythia8/186/x86_64-slc6-gcc47-opt/xmldoc
        export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.25/x86_64-slc6-gcc48-opt/root/
        cmake -DHEPMC_ROOT_DIR='/afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc47-opt/' \
              -DPYTHIA8='/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt67b/pythia8/186/x86_64-slc6-gcc47-opt/'  \
              -DDOXYGEN_EXECUTABLE='/afs/cern.ch/sw/lcg/external/doxygen/1.8.2/x86_64-slc6-gcc48-opt/bin/doxygen' \
              -DCMAKE_CXX_COMPILER='/afs/cern.ch/sw/lcg/contrib/gcc/4.9/x86_64-slc6-gcc49-opt/bin/g++' \
              -DCMAKE_INSTALL_PREFIX=. ..
        make


2. Running an example
-------------------

Usage: ./FCCfastsim
		<intput gdml filename : mandatory>
		<output root filename : optional (default "DefaultOutput")>
		<do smearing : optional (default true)>
		<settings macro : optional (if not ->GUI)>


2.1 Without GUI

        ./FCCfastsim ../gdml/simple.gdml Output true pythia.in

2.2 With GUI

        ./FCCfastsim ../gdml/simple.gdml Output true


3. Macro with settings: event generator
-------------------

3.1. Simple particle gun (default)

     gun.in:

          /generator/select particleGun
          /gun/particle e-
          /gun/direction 0.1 0.1 0.05
          /gun/energy 10 GeV
          /run/setCut 10 m
          /run/beamOn 1000

3.2. Pythia 8

Detail of Pythia processes, beam/target (PDG code) ans energy (in GeV) need to be specified in Geant macro:

       pythia.in :

          /generator/select pythia8
          /generator/pythia8/read "HardQCD:all = on"
          /generator/pythia8/read "PhaseSpace:pTHatMin = 20."
          /generator/pythia8/init 2212 2212 14000.

3.3. HepMC event read from ROOT file

       root.in:

          /generator/select hepmcRoot
          /generator/hepmcRoot/verbose 1
          /generator/hepmcRoot/open PythiaInHepMC.root
          /run/beamOn 1



4. Geometry description: GDML
-------------------

Geometry is read from the GDML file. All GDMLs are stored in gdml/ directory:

FCCSD.gdml - Complex detector with various EM calorimeters, hadron calorimeters, muon detectors (all sensitive detectors)

FCCOnedetector.gdl - Only one EM calorimeter (sensitive detector)

simple.gdml - Simple box as a detector


5. Choosing the parametrisation type
-------------------

The details concerning the detectors and fast simulation models are described in the file:

   src/FCCFastSim/FCCFastSimGeometry.cc

Possible parametrisation types:

   FCCDetectorParametrisation::eCMS
   FCCDetectorParametrisation::eATLAS
   FCCDetectorParametrisation::eALEPH
   FCCDetectorParametrisation::eATLFAST

Resolution and efficiency for a given parametrisation type can be found in:

   src/FCCFastSim/FCCDetectorParametrisation.cc