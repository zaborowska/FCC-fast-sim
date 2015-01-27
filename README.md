FCC-fast-sim
============

Geant 4 based fast simulation

____________________________________________________________________

1. Installation
-------------------

        mkdir build
        cd build
        source /afs/cern.ch/sw/lcg/contrib/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
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


2. Documentation
-------------------

	cd build
	make doc

Documentation can be found in

	build/doc/html/index.html


3. Running an example
-------------------

Usage: ./FCCfastsim
		<intput gdml filename : mandatory>
		<output root filename : optional (default "DefaultOutput")>
		<do smearing : optional (default true)>
		<settings macro : optional (if not ->GUI)>


3.1 Without GUI

        ./FCCfastsim ../gdml/simple.gdml Output true pythia.in

3.2 With GUI

        ./FCCfastsim ../gdml/simple.gdml Output true


4. Macro with settings: event generator
-------------------

4.1. Simple particle gun (default)

     gun.in:

          /generator/select particleGun
          /gun/particle e-
          /gun/direction 0.1 0.1 0.05
          /gun/energy 10 GeV
          /run/setCut 10 m
          /run/beamOn 1000

4.2. Pythia 8

Detail of Pythia processes, beam/target (PDG code) ans energy (in GeV) need to be specified in Geant macro:

       pythia.in :

          /generator/select pythia8
          /generator/pythia8/read "HardQCD:all = on"
          /generator/pythia8/read "PhaseSpace:pTHatMin = 20."
          /generator/pythia8/init 2212 2212 14000.

4.3. HepMC event read from ROOT file

       root.in:

          /generator/select hepmcRoot
          /generator/hepmcRoot/verbose 1
          /generator/hepmcRoot/open PythiaInHepMC.root
          /run/beamOn 1



5. Geometry description: GDML
-------------------

Geometry is read from the GDML file. All GDMLs are stored in gdml/ directory:

FCCFullDetector.gdml - Complex detector with various EM calorimeters, hadron calorimeters, muon detectors (all sensitive detectors)

FCCOnedetector.gdl - Only one EM calorimeter (sensitive detector)

simple.gdml - Simple box as a detector


6. Details on code
-------------------

6.1. Detector geometry
- - - - - - - -

Path to the geometry file is given as a parameter to application. It should be a GDML file.
However, the fast simulation models are attached to the detectors based on the auxiliary information.
Each <volume> item inside <structure> ... </structure> block can contain auxiliary tag:

     <auxiliary auxtype=”FastSimModel” auxvalue=”tracker*”/>

auxtype is the key, 'FastSimModel' should be used;
auxvalue is the value that should contain the fast simulation model that is attached to the volume:

'Tracker' -> for FCCFastSimModelTracker
'ECal' -> for FCCFastSimModelEMCal
'HCal' -> for FCCFastSimModelHCal

e.g. TrackerBarrel, ECalBarrel, ECalEndCap


6.2. Magnetic field
- - - - - - - -

Uniform magnetic field is defined in lines 122-127 of

    src/FCCFastSim/FCCFastSimGeometry.cc.


6.3. Parametrisation
- - - - - - - -

The parametrisation type (what resolutions of the smearing should be used) is given to the fast simulation models in the constructor. It can alter for each detector and can be changed in lines 85, 94 and 104 of

   src/FCCFastSim/FCCFastSimGeometry.cc

Possible parametrisation types:

   FCCDetectorParametrisation::eCMS
   FCCDetectorParametrisation::eATLAS
   FCCDetectorParametrisation::eALEPH
   FCCDetectorParametrisation::eATLFAST

The last one depends on the input parametrisation files (data/*txt), the others are defined in

   src/FCCFastSim/FCCDetectorParametrisation.cc

The class contains also the information about the detector efficiency.
