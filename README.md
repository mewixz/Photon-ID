# CNN-Photon-ID
Contains the code used to develop the Convolutional photonID using images from the Electromagnetic Calorimeter layers (sampling 1, 2 and 3). 

              <img width="514" alt="CNN" src="https://raw.githubusercontent.com/mobelfki/CNN-Photon-ID/main/.github/images/CNN.png">

- The CNN is designed to extract information from each sampling then combine them. The model is implemented using Keras functional APIs and TensorFlow as a backend

              <img width="514" alt="CNN_model" src="https://raw.githubusercontent.com/mobelfki/CNN-Photon-ID/main/.github/images/CNN_model.png" >
# Model and Training      
- The model is defined in `Train/Models.models` as `CNNHybridClassifier` and trained using `Train/train.py` code :

          ./train --args 
          
Arguments are the number of epochs, learning rate, batch size, Number of CNN neurons, Number of DNN neurons...
# Data Processing
- Training data are images from ATLAS Electromagnetic calorimeter and handled by `Train/DataProcessing` class. 
- The data are Monte Carlo simulation produced using `ZllyAthDerivation` and processed using `NTUP` code to produce root flat ntuple which are then converted to NumPy arrays using `Array` code.
- `CellsToImage` is a code which converte the NumPy cells vector to NumPy images for training, An example of training images is shown below:

              <img width="514" alt="Image_CNN" src="https://raw.githubusercontent.com/mobelfki/CNN-Photon-ID/main/.github/images/Image_CNN.png" >
# Validation
- After Training the model is validated using `Validation` code, the model is validated on an out-of-sample photon generated from a different process used in training to remove any potential bias
# ONNX and Integration in Athena
- After validation, the mode is converted to ONNX format developed by Microsoft using the code `Train/toonnx.py`
- `DerivationFrameworkCNNPID` is a framework that integrates and apply the model at the derivation stage in the official ATLAS Athena framework. 
# Template Fit
- A likelihood fit is used to estimate the background fraction in real data from Monte Carlo, this is done with `TemplateFit/TemplateFit.cxx` code. The signal template is extracted directly from the Monte Carlo, while the backround is modelized with polynomial function. The combined model (Signal + Background) with normalization fractions is then fitted to data. The estimated number of background is subtracted from data to compute the efficiency as : 

              <a href="https://www.codecogs.com/eqnedit.php?latex=\epsilon&space;=&space;\frac{N^{Passed}_{\gamma}}{N^{All}_{\gamma}}" target="_blank"><img src="https://latex.codecogs.com/png.latex?\epsilon&space;=&space;\frac{N^{Passed}_{\gamma}}{N^{All}_{\gamma}}" title="\epsilon = \frac{N^{Passed}_{\gamma}}{N^{All}_{\gamma}}" /></a>
              
- An example of the Template Fit: 

               <img width="514" alt="Fit_Mlly.png" src="https://raw.githubusercontent.com/mobelfki/CNN-Photon-ID/main/.github/images/Fit_Mlly.png" >
               
# Results 
- The developed model shows a significant improvement and overperforms the existing identification algorithm

               <img width="514" alt="Tight" src="https://raw.githubusercontent.com/mobelfki/CNN-Photon-ID/main/.github/images/Tight_Inc_vs_Tight_CNN__UnConverted_Iso_loose.png" >
