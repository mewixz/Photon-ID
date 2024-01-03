# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

from egammaRec.Factories import ToolFactory
from egammaRec import egammaKeys
from DerivationFrameworkCNNPID.DerivationFrameworkCNNPIDConf import DerivationFramework__PhotonID

##################
# PhotonID
##################
PhotonID = ToolFactory( DerivationFramework__PhotonID,
                                SGKey_photons = egammaKeys.outputPhotonKey(),
				SGKey_caloCells = egammaKeys.caloCellKey(),
				N_eta = 7,
				N_phi = 11,
				CNN_model_name="DerivationFrameworkCNNPID/MixtureNN_CNN_CNNn_256_DNNn_128_lr_0.0001_dr_0.08_bt_32.onnx")
