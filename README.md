FCC-fast-sim
============

Geant 4 based fast simulation

____________________________________________________________________

1. Installation
-------------------

        mkdir build
        cd build
        source /afs/cern.ch/sw/lcg/external/geant4/10.0.p02/x86_64-slc6-gcc47-opt/CMake-setup.sh
        export PYTHIA8DATA=/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt67b/pythia8/186/x86_64-slc6-gcc47-opt/xmldoc
        export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.21/x86_64-slc6-gcc47-opt/root/
        cmake -DHEPMC_ROOT_DIR='/afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc47-opt/' -DPYTHIA8='/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt67b/pythia8/186/x86_64-slc6-gcc47-opt/' -DGCCXML='/afs/cern.ch/sw/lcg/external/gccxml/0.9.0_20120309p3/x86_64-slc6-gcc47-opt/bin/gccxml' -DCMAKE_INSTALL_PREFIX=. ..
        make

2. Event generator - Pythia 8
-------------------

Detail of Pythia processes and beam/target, energy need to be specified in Geant macro:

        /generator/select pythia8
        /generator/pythia8/read "HardQCD:all = on"
        /generator/pythia8/read "PhaseSpace:pTHatMin = 20."
        /generator/pythia8/init 2212 2212 14000.

3. Running an example
-------------------

3.1 Without GUI

        ./examplePar01 ../simple.gdml examplePar01.in
        ./examplePar01 ../FCCdetector.gdml examplePar01.in
        
3.1 With GUI

        ./examplePar01 ../simple.gdml
        ./examplePar01 ../FCCdetector.gdml
