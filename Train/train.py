#!/usr/bin/env python
import numpy as np
#import matplotlib.pyplot as plt
import sys
from argparse import ArgumentParser
from Models.models import Classifier, MClassifier, SSClassifier, CNNClassifier, CNNHybridClassifier
from DataProcessing.DataProcessing import Data, CNNData
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
# np.set_printoptions(threshold=sys.maxsize)
import os
import math

seed = 215
np.random.seed(seed)
data = CNNData(1)
nohealthy = CNNData(2)


def getArgs():
    """
    Get arguments from command line.
    """
    args = ArgumentParser(description="Argumetns CNN")
    args.add_argument('-n',    '--nepochs',       action='store',
                      default=10,      help='Number of epochs')
    args.add_argument('-o',    '--noutput',       action='store',
                      default=1,       help='Number of outputs from the model nclass')
    args.add_argument('-l',    '--learning_rate', action='store',
                      default=0.0001, help='Learning rate')
    args.add_argument('-d',    '--dropout_rate',  action='store',
                      default=0.08,     help='Dropout rate')
    args.add_argument('-b',    '--batch_size',    action='store',
                      default=32,      help='Batch size')
    args.add_argument('-CNNn', '--CNNn',          action='store',
                      default=256,     help='Number of neurons in each CNN layer')
    args.add_argument('-DNNn', '--DNNn',          action='store',
                      default=128,     help='Number of neurons in each DNN layer')
    args.add_argument('-V',    '--val',           action='store',
                      default=False,   help='Validation')
    args.add_argument('-G',    '--Gen',           action='store',
                      default=False,   help='Train the model on generator')
    args.add_argument('-O',    '--output',        action='store',
                      default='/pbs/home/m/mbelfkir/MLPhotonID/Train/Output/',    help='Output Directory')
    return args.parse_args()


def lr_decay(epoch, lr):
    decay_rate = 0.5
    decay_step = 50
    if epoch % decay_step == 0 and epoch:
        return lr * decay_rate
    return lr


def trainHybridCNN(Nepochs, learing_rate, dropout_rate, batch_size, N_CNN_neurons, N_DNN_neurons, Noutputs, tag, OutDir, isGen):

    clf = CNNHybridClassifier(
        'HybridCNN_Classifier', N_CNN_neurons, N_DNN_neurons, Noutputs, dropout_rate).model

    adm = optimizers.Adam(lr=learing_rate)
    clf.compile(loss=['binary_crossentropy'],
                optimizer=adm, metrics=['binary_accuracy'])
    print(clf.summary())
    
    es1 = EarlyStopping(monitor='val_loss', patience=5,
                        verbose=1, mode='min', min_delta=0.0001)
    save_wgt_on_epoch = ModelCheckpoint(
        OutDir+'Model_'+tag+'_Ep_{epoch:02d}.weights', monitor='loss', mode='min', verbose=1, save_best_only=True)
    roc = roc_on_epoch()
    lrScheduler = LearningRateScheduler(lr_decay, verbose=1)

    X1 = np.append(data.Img_Lr1_Train, data.Img_Lr1_Val, axis=0)
    X2 = np.append(data.Img_Lr2_Train, data.Img_Lr2_Val, axis=0)
    X3 = np.append(data.Img_Lr3_Train, data.Img_Lr3_Val, axis=0)
    #X4 = np.append(data.ETA_Train, data.ETA_Val, axis=0)
    Y  = np.append(data.Y_Train, data.Y_Val, axis=0)
    #X1, X2, X3, Y = shuffle(X1, X2, X3, Y, random_state=seed)
    
    
    NH_X1 = np.append(nohealthy.Img_Lr1_Train, nohealthy.Img_Lr1_Val, axis=0)[:Y.shape[0],:]
    NH_X2 = np.append(nohealthy.Img_Lr2_Train, nohealthy.Img_Lr2_Val, axis=0)[:Y.shape[0],:]
    NH_X3 = np.append(nohealthy.Img_Lr3_Train, nohealthy.Img_Lr3_Val, axis=0)[:Y.shape[0],:]
    NH_Y  = np.append(nohealthy.Y_Train, nohealthy.Y_Val, axis=0)[:Y.shape[0]]
    NH_Y  = np.multiply(NH_Y, 1)
    #NH_X1, NH_X2, NH_X3, NH_Y = shuffle(NH_X1, NH_X2, NH_X3, NH_Y, random_state=seed)
    
    X1 = np.append(X1, NH_X1, axis=0)
    X2 = np.append(X2, NH_X2, axis=0)
    X3 = np.append(X3, NH_X3, axis=0)
    Y  = np.append(Y, NH_Y, axis=0)
    
    X1, X2, X3, Y = shuffle(X1, X2, X3, Y, random_state=seed)
    
    class_weights = class_weight.compute_class_weight('balanced', np.unique(Y), Y)
    
    X1, X2, X3, Y = reshapeX123Y(X1, X2, X3, Y)

    #clf.load_weights(OutDir+'Model_FF_Stg1_CNN_Nep_50_CNNn_256_DNNn_128_lr_1e-06_dr_0.0_bt_128_Ep_50.weights')

    history = "local"

    if isGen:
        print('Training on Generator ...')
        datagen = IDG(rotation_range=180, horizontal_flip=True, validation_split=0.33, featurewise_center=True, featurewise_std_normalization=True)
        train_itr = multiIDG(generator=datagen, X1=X1, X2=X2, X3=X3, Y=Y,
                             batch_size=batch_size, img_h=128, img_W=128, subset='training')
        val_itr = multiIDG(generator=datagen, X1=X1, X2=X2, X3=X3, Y=Y,
                           batch_size=batch_size, img_h=128, img_W=128, subset='validation')

        callbacks_list = [save_wgt_on_epoch]

        history = clf.fit_generator(train_itr, validation_data=val_itr, epochs=Nepochs, shuffle=False, steps_per_epoch=(
            Y.shape[0]*.66)/batch_size, validation_steps=(Y.shape[0]*.33)/batch_size, class_weight=class_weights, callbacks=callbacks_list)
        return history, clf
    else:
        callbacks_list = [save_wgt_on_epoch]

        history = clf.fit(x={'Input1': X1, 'Input2': X2, 'Input3': X3}, y=Y, epochs=Nepochs, batch_size=batch_size,
                          shuffle=True, validation_split=0.33, callbacks=callbacks_list, class_weight=class_weights)
        
        #clf.save(OutDir+'CNNPhotonID_MixtureNN_CNNn_256_DNNn_128_lr_1e-4_dr_8_bt_32')
    return history, clf


def save_model_history(model_history, tag, OutDir):

    np.save(OutDir+'Accuracy_Train_'+tag+'.npy',
            np.asarray(model_history.history['binary_accuracy']))
    np.save(OutDir+'Accuracy_Val_'+tag+'.npy',
            np.asarray(model_history.history['val_binary_accuracy']))
    np.save(OutDir+'Loss_Train_'+tag+'.npy',
            np.asarray(model_history.history['loss']))
    np.save(OutDir+'Loss_Val_'+tag+'.npy',
            np.asarray(model_history.history['val_loss']))


def reshapeX123Y(X1,X2,X3,Y):

    X1 = X1.reshape(X1.shape[0], 2, 56, 1)
    X2 = X2.reshape(X2.shape[0], 11, 7, 1)
    X3 = X3.reshape(X3.shape[0], 11, 4, 1)
    Y  = np.multiply(Y, 1)
    return X1, X2, X3, Y

def main():
    """
    The main function of train
    """

    args = getArgs()
    N = int(args.nepochs)
    lr = float(args.learning_rate)
    dr = float(args.dropout_rate)
    bt = int(args.batch_size)
    CNNn = int(args.CNNn)
    DNNn = int(args.DNNn)
    Nout = int(args.noutput)
    OutDir = str(args.output)
    isval = bool(args.val)
    isGen = bool(args.Gen)

    tag = ''

    if not isGen:
        tag = 'WithGN_FF_Stg2_CNN_Nep_'+str(N)+'_CNNn_'+str(CNNn)+'_DNNn_' + \
            str(DNNn)+'_lr_'+str(lr)+'_dr_'+str(dr)+'_bt_'+str(bt)
    else:
        tag = 'WithGN_FF_Stg2_CNN_Nep_'+str(N)+'_CNNn_'+str(CNNn)+'_DNNn_'+str(DNNn) + \
            '_lr_'+str(lr)+'_dr_'+str(dr)+'_bt_'+str(bt)+'_Generator'

    model = "local"
    if isval == False:

        print('Training ...')
        print('Model tag : ', tag)

        history, model = trainHybridCNN(
            N, lr, dr, bt, CNNn, DNNn, Nout, tag, OutDir, isGen)
        save_model_history(history, tag, OutDir)

        model_json = model.to_json()
        with open(OutDir+tag+"_model.json", "w") as json_file:
            json_file.write(model_json)
        model.save_weights(OutDir+tag+"_model.h5")
    else:
        print('Validation ...')
        print('Retreiving : ', tag)

        model_json_file = open(OutDir+tag+"_model.json", 'r')
        model_json = model_json_file.read()
        model_json_file.close()
        model = model_from_json(model_json)
        model.load_weights(OutDir+tag+"_model.h5")
        adm = optimizers.Adam(lr=learning_rate)
        model.compile(loss=['binary_crossentropy'],
                      optimizer=adm, metrics=['binary_accuracy'])

if __name__ == '__main__':
    main()
