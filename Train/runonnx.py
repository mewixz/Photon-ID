from tensorflow.python.keras import backend as K
from tensorflow.python.keras.models import load_model
import sys
sys.path.append('../')
from argparse import ArgumentParser
from Models.models import Classifier, MClassifier, SSClassifier, CNNClassifier, CNNHybridClassifier
from DataProcessing.DataProcessing import HH
from tensorflow.keras import optimizers
from tensorflow.keras.models import model_from_json
from sklearn.metrics import roc_curve, roc_auc_score, auc
from scipy import interp
from itertools import cycle
from tensorflow.keras.callbacks import EarlyStopping, ModelCheckpoint, ReduceLROnPlateau, LearningRateScheduler
from sklearn.utils import class_weight, shuffle
from tensorflow.keras.callbacks import Callback
from sklearn.metrics import confusion_matrix, f1_score, precision_score, recall_score
from sklearn.model_selection import train_test_split
from Models.calls import roc_on_epoch, multiIDG
from tensorflow.keras.utils import plot_model
from tensorflow.keras.preprocessing.image import ImageDataGenerator as IDG
import itertools
import os
import math
import numpy as np
import onnxruntime
import time
loop_count = 3
data = HH()

#X1 = np.array([34.0883,106.1904,56.0517,30.2691,-71.0525,97.9426,-18.0221,789.2883,5.1097,988.3741,8.2326,65.2033,184.9822,-38.8316,-71.9822,-19.9412,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000] , dtype=float)



#X2 = np.array([-198.4358,-113.6180,5.1097,-37.0045,289.4931,257.6733,782.3950,560.1284,159.9271,201.2025,473.3647,2477.8027,1492.1558,132.2787,418.2895,299.0426,1018.1180,578.6639,31.0622,26.0289,65.2033,146.7892,42.9945,36.1129,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000], dtype=float)


#X3 = np.array([9.9956,-3.9330,4.0690,13.0108,-6.9818,42.9945,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000], dtype=float)



#sum1 = np.sum(X1)
#sum2 = np.sum(X2)
#sum3 = np.sum(X3)
#X1 /= sum1
#X2 /= sum2
#X3 /= sum3

X1 = data.Img_Lr1[1000,:,:]
X2 = data.Img_Lr2[1000,:,:]
X3 = data.Img_Lr3[1000,:,:]


X1 = X1.reshape(1, 2, 56, 1)
X2 = X2.reshape(1, 11, 7, 1)
X3 = X3.reshape(1, 11, 4, 1)


model = 'CNNPhotonID_MixtureNN_CNNn_256_DNNn_128_lr_1e-4_dr_8_bt_32'
# load Keras and ONNX model
net = load_model(model+'.h5')
onnx_model = model+'.onnx'
sess = onnxruntime.InferenceSession(onnx_model)

# Keras predtion
print('Keras')
start_time = time.time()
for i in range(loop_count):
    XX1 = X1.reshape(1 , 2, 56, 1)
    XX2 = X2.reshape(1 , 11, 7, 1)
    XX3 = X3.reshape(1 , 11, 4, 1)
    pred = net.predict([XX1,XX2,XX3])
    print(pred[0])
print("Keras inferences with %s second in average" %((time.time() - start_time) / loop_count))


# ONNX prediction
print('ONNX')
print(sess.get_inputs())
X = [X1, X2, X3]
X = X if isinstance(X, list) else [X]
feed = dict([(input.name, X[n]) for n, input in enumerate(sess.get_inputs())])


start_time = time.time()
for i in range(loop_count):
    pred_onnx = sess.run(None, feed)[0]
    print(pred_onnx)
print("ONNX inferences with %s second in average" %((time.time() - start_time) / loop_count))