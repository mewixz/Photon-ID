#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import sys
from DataProcessing.DataProcessing import Data, CNNData
import math
from hep_ml import reweight

seed = 215
np.random.seed(seed) 
data = CNNData(0)

mask_sig = data.Y_Train == 1
mask_bkg = data.Y_Train == 0

PT = data.Z_Train[:,4] # to GeV

Sig_PT = PT[mask_sig]
Bkg_PT = PT[mask_bkg]

reweighter = reweight.GBReweighter(n_estimators=30, learning_rate=0.1, max_depth=3, min_samples_leaf=1000, gb_args={'subsample': 0.4})
reweighter.fit(Bkg_PT, Sig_PT)
weights = reweighter.predict_weights(Bkg_PT)

hist_settings = {'bins': 50, 'density': True, 'histtype': 'step'}
plt.figure()

fig, axs = plt.subplots(1,2,figsize=(15,15))

axs[0].hist(Sig_PT, **hist_settings)
axs[0].hist(Bkg_PT, **hist_settings)
axs[0].legend(['Signal', 'Background'], loc='best')
axs[0].set_title('Before reweighting')

axs[1].hist(Sig_PT, **hist_settings)
axs[1].hist(Bkg_PT, weights=weights, **hist_settings)
axs[1].legend(['Signal', 'Background'], loc='best')
axs[1].set_title('After reweighting')

plt.savefig('plot.png')