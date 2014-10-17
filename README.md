FCC-fast-sim
============

Geant 4 based fast simulation

____________________________________________________________________

1. Installation
-------------------

        mkdir build
        cd build
        source /afs/cern.ch/sw/lcg/external/geant4/10.0.p02/x86_64-slc6-gcc47-opt/CMake-setup.sh
        source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.21/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh
        export PYTHIA8DATA=/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt67b/pythia8/186/x86_64-slc6-gcc47-opt/xmldoc
        export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.21/x86_64-slc6-gcc47-opt/root/
        cmake -DHEPMC_ROOT_DIR='/afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc47-opt/' -DPYTHIA8='/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt67b/pythia8/186/x86_64-slc6-gcc47-opt/' -DCMAKE_INSTALL_PREFIX=. ..
        make

2. Choosing the primary generator
-------------------

2.1. Pythia 8

Detail of Pythia processes and beam/target, energy need to be specified in Geant macro:

       pythia.in :

        /generator/select pythia8
        /generator/pythia8/read "HardQCD:all = on"
        /generator/pythia8/read "PhaseSpace:pTHatMin = 20."
        /generator/pythia8/init 2212 2212 14000.

2.2. HepMC event from ROOT file

       root.in:

          /generator/select hepmcRoot
          /generator/hepmcRoot/verbose 1
          /generator/hepmcRoot/open PythiaInHepMC.root
          /run/beamOn 1


2.3. Simple particle gun (default)

     gun.in


3. Geometry
-------------------

Geometry is read from the GDML file. All GDMLs are stored in gdml/ directory:

FCCSD.gdml - Complex detector with various EM calorimeters, hadron calorimeters, muon detectors (all sensitive detectors)

FCCOnedetector.gdl - Only one EM calorimeter (sensitive detector)

simple.gdml - Simple box as a detector


4. Running an example
-------------------

4.1 Without GUI

    For a simple geometry (box):

        ./FCCfastsim ../gdml/simple.gdml pythia.in

    For a complex geometry (with sensitive detectors):

        ./FCCfastsim ../gdml/FCCSD.gdml root.in

4.2 With GUI

    For a simple geometry (box):

        ./FCCfastsim ../gdml/simple.gdml

    For a complex geometry (with sensitive detectors):

        ./FCCfastsim ../gdml/FCCSD.gdml