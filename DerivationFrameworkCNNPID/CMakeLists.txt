################################################################################
# Package: DerivationFrameworkCNNPID
# Author: BELFKIR Mohamed
# Email: mohamed.belfkir@cern.ch
################################################################################

# Declare the package name:
atlas_subdir( DerivationFrameworkCNNPID )

# Declare the package's dependencies:
atlas_depends_on_subdirs(
   Event/xAOD/xAODEgamma
   Calorimeter/CaloCalibHitRec
   Calorimeter/CaloClusterCorrection
   Calorimeter/CaloEvent
   Calorimeter/CaloUtils
   GaudiKernel
   LArCalorimeter/LArTools )

# External dependencies:
find_package( onnxruntime )

# Component(s) in the package:
atlas_add_library( DerivationFrameworkCNNPIDLib
   DerivationFrameworkCNNPID/*.h src/*.cxx
   PUBLIC_HEADERS DerivationFrameworkCNNPID
   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${ONNXRUNTIME_INCLUDE_DIRS} PathResolver
   LINK_LIBRARIES CaloIdentifier AthenaBaseComps CaloCalibHitRecLib 
   xAODCaloEvent xAODEgamma CaloClusterCorrectionLib
   CaloUtilsLib LArToolsLib
   DerivationFrameworkInterfacesLib
   LINK_LIBRARIES ${ROOT_LIBRARIES} ${ONNXRUNTIME_LIBRARIES} CaloEvent
   GaudiKernel PathResolver)

atlas_add_component( DerivationFrameworkCNNPID
   src/components/*.cxx
   LINK_LIBRARIES GaudiKernel DerivationFrameworkCNNPIDLib)

# Install files from the package:
atlas_install_python_modules( python/*.py )
atlas_install_data( data/*)
