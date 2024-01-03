#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import sys
sys.path.append('../')
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
import os
import math

output = 'Stg2/Stg3/Bkg/'

seed = 215
np.random.seed(seed)
data = CNNData(1)

lst= ['Inc', 'UnConv', 'Conv']
PT = [10,20,30,40,60,80,2000]
ETA= [0.,0.6,1.37,1.52,1.8,2.4]
crackbin=2
    
def getArgs():
	"""
	Get arguments from command line.
	"""
	args = ArgumentParser(description="Argumetns for training for DNN")
	args.add_argument('-n',    '--nepochs',       action='store', default='04',      help='Number of epochs')
	args.add_argument('-m', '--model', action='store', default='', help='weight')
	args.add_argument('-p', '--path', action='store', default='/pbs/home/m/mbelfkir/MLPhotonID/Train/Output_FF', help='weight path')
	args.add_argument('-o',    '--noutput',       action='store', default=1,       help='Number of outputs from the model nclass')
	args.add_argument('-l',    '--learning_rate', action='store', default=0.0001, help='Learning rate')
	args.add_argument('-d',    '--dropout_rate',  action='store', default=0.08,     help='Dropout rate')
	args.add_argument('-b',    '--batch_size',    action='store', default=32,      help='Batch size')
	args.add_argument('-CNNn', '--CNNn',          action='store', default=256,     help='Number of neurons in each CNN layer')
	args.add_argument('-DNNn', '--DNNn',          action='store', default=128,     help='Number of neurons in each DNN layer')
	return args.parse_args()


def HybridCNN(path, Nepochs, learing_rate, dropout_rate, batch_size, N_CNN_neurons, N_DNN_neurons, Noutputs, tag):
	
	clf = CNNHybridClassifier(tag,N_CNN_neurons,N_DNN_neurons,Noutputs,dropout_rate).model
	adm = optimizers.Adam(lr=learing_rate)
	clf.compile(loss=['binary_crossentropy'], optimizer=adm, metrics=['binary_accuracy']);
	clf.load_weights(path+'/Model_'+tag+'_Ep_'+str(Nepochs)+'.weights')
	return clf


def plot_roc_curve(y_true, y_pred, Sig_Eff, Bkg_Eff, tag, model_tag):

	plt.figure()
	fpr, tpr, _ = roc_curve(list(y_true), list(y_pred))
	auc = roc_auc_score(list(y_true), list(y_pred))
	plt.plot(tpr, 1-fpr, color='darkorange', lw=2, label='ROC curve (area = %0.4f)' %  auc)
	plt.plot(Sig_Eff,Bkg_Eff, 'ob')
	plt.xlim([0., 1.0])
	plt.ylim([0., 1.0])
	plt.xlabel('Signal Eff')
	plt.ylabel('Bkg Rejection')
	plt.title('ROC')
	plt.legend(['CNN ROC', 'ATLAS Tigh WP'], loc="lower left")
	plt.title('NN ROC ' + tag)
	plt.savefig(output+'ROC_'+tag+'__'+model_tag+'.png')
	plt.close()
	plot_CNN_output(y_true, y_pred, tag)
	
def plot_roc_curve_eta_bin(y_true, y_pred, Sig_Eff, Bkg_Eff, tag, ptbin, etabin):
	plt.figure()
	fpr, tpr, _ = roc_curve(list(y_true), list(y_pred))
	auc = roc_auc_score(list(y_true), list(y_pred))
	plt.plot(tpr, 1-fpr, color='darkorange', lw=2, label='ROC curve (area = %0.4f)' %  auc)
	plt.plot(Sig_Eff,Bkg_Eff, 'ob')
	plt.xlim([0., 1.0])
	plt.ylim([0., 1.0])
	plt.xlabel('Signal Eff')
	plt.ylabel('Bkg Rejection')
	plt.title('ROC')
	plt.legend(['CNN ROC (area = %0.3f)' %  auc, 'ATLAS Tigh WP'],loc="lower left")
	plt.title('NN ROC ' + tag + "PT :["+ str(PT[ptbin])+ ","+ str(PT[ptbin+1])+"] ETA:["+str(ETA[etabin])+ ","+ str(ETA[etabin+1])+"]")
	plt.savefig(output+'ROC_'+tag+'_'+'_PT_'+str(ptbin)+'_ETA_'+str(etabin)+'.png')
	plt.close()
	plot_CNN_output_eta_bin(y_true, y_pred, tag, ptbin, etabin)
	
	return auc

def plot_CNN_output_eta_bin(y_true, y_pred, tag, ptbin, etabin):
	
	plt.figure()
	Sig_mask = y_true == 1
	Bkg_mask = y_true == 0
	plt.hist(y_pred[Sig_mask], bins = 50, histtype='step',  density='True')
	plt.hist(y_pred[Bkg_mask], bins = 50, histtype='step',  density='True')
	plt.legend(['Signal', 'Background'])
	plt.title('NN Ouput ' + tag + "PT :["+ str(PT[ptbin])+ ","+ str(PT[ptbin+1])+"] ETA:["+str(ETA[etabin])+ ","+ str(ETA[etabin+1])+"]")
	plt.savefig(output+'P_'+tag+'_'+'_PT_'+str(ptbin)+'_ETA_'+str(etabin)+'.png')
	plt.close()
	
def plot_CNN_output(y_true, y_pred, tag):
	
	plt.figure()
	Sig_mask = y_true == 1
	Bkg_mask = y_true == 0
	plt.hist(y_pred[Sig_mask], bins = 50, histtype='step',  density='True')
	plt.hist(y_pred[Bkg_mask], bins = 50, histtype='step',  density='True')
	plt.legend(['Signal', 'Background'])
	plt.title('NN Ouput ' + tag)
	plt.savefig(output+'P_'+tag+'.png')
	plt.close()

def computeTightEff(y_true, z_true):
	
	mask_TP = (y_true == 1)*(z_true == 1)
	mask_TN = (y_true == 0)*(z_true == 0)
	mask_FP = (y_true == 0)*(z_true == 1)
	mask_FN = (y_true == 1)*(z_true == 0)
	
	TP = y_true[mask_TP].shape[0] 
	TN = y_true[mask_TN].shape[0]
	FP = y_true[mask_FP].shape[0]
	FN = y_true[mask_FN].shape[0]
	
	if TP ==0:
		TP = 0.1
	if FN == 0:
		FN = 0.1
	if TN == 0:
		TN = 0.1
	if FP == 0:
		FP = 0.1
	
	
	Sig = float(TP/(TP+FN))
	Bkg = float(TN/(TN+FP))
	
	if Sig == 0:
		Sig = 0.001
	if Bkg == 0:
		Bkg = 0.001
	
	return Sig, Bkg
	
def scanEff(y_true, y_pred, true_Sig_Eff):
	
	P = np.arange(0.,1.0,0.001)
	X = 0
	Y = 0
	Z = 0 
	for p in P:
		mask_TP = (y_true == 1)*(y_pred > p)
		mask_TN = (y_true == 0)*(y_pred <= p)
		mask_FP = (y_true == 0)*(y_pred > p)
		mask_FN = (y_true == 1)*(y_pred <= p)

		TP = y_true[mask_TP].shape[0] 
		TN = y_true[mask_TN].shape[0]
		FP = y_true[mask_FP].shape[0]
		FN = y_true[mask_FN].shape[0]

		Sig_Eff = float(TP/(TP+FN))
		Bkg_Eff = float(TN/(TN+FP))
		
		if Sig_Eff < 0.001 or Bkg_Eff < 0.001 :
			continue
		
		if abs(float(format(true_Sig_Eff, '.3f')) - float(format(Sig_Eff, '.3f'))) < 0.005:
			return Sig_Eff, Bkg_Eff, p
		if abs(float(format(true_Sig_Eff, '.3f')) - float(format(Sig_Eff, '.3f'))) < 0.01:
			X = Sig_Eff; Y = Bkg_Eff; Z = p
	return X, Y, Z

def scanRej(y_true, y_pred, true_Bkg_Eff):
    
	P = np.arange(0.,1.0,0.001)
	X = 0
	Y = 0
	Z = 0 
	for p in P:
		mask_TP = (y_true == 1)*(y_pred > p)
		mask_TN = (y_true == 0)*(y_pred <= p)
		mask_FP = (y_true == 0)*(y_pred > p)
		mask_FN = (y_true == 1)*(y_pred <= p)

		TP = y_true[mask_TP].shape[0] 
		TN = y_true[mask_TN].shape[0]
		FP = y_true[mask_FP].shape[0]
		FN = y_true[mask_FN].shape[0]

		Sig_Eff = float(TP/(TP+FN))
		Bkg_Eff = float(TN/(TN+FP))
		
		if Sig_Eff < 0.001 or Bkg_Eff < 0.001 :
			continue
		
		if abs(float(format(true_Bkg_Eff, '.3f')) - float(format(Bkg_Eff, '.3f'))) < 0.005:
			return Sig_Eff, Bkg_Eff, p
		if abs(float(format(true_Bkg_Eff, '.3f')) - float(format(Bkg_Eff, '.3f'))) < 0.01:
			X = Sig_Eff; Y = Bkg_Eff; Z = p
	return X, Y, Z
    
    
    
def maxEff(y_true, y_pred):
	
	P = np.arange(0.,1.0,0.001)
	S = []
	X = []
	Bkg= []
	Sig= []
	for p in P:
		
		mask_TP = (y_true == 1)*(y_pred > p)
		mask_TN = (y_true == 0)*(y_pred <= p)
		mask_FP = (y_true == 0)*(y_pred > p)
		mask_FN = (y_true == 1)*(y_pred <= p)
		
		TP = y_true[mask_TP].shape[0] 
		TN = y_true[mask_TN].shape[0]
		FP = y_true[mask_FP].shape[0]
		FN = y_true[mask_FN].shape[0]
		
		Sig_Eff = float(TP/(TP+FN))
		Bkg_Eff = float(TN/(TN+FP))
		
		N_Sig = float(Sig_Eff*(TP+FN))
		N_Bkg = float((1-Bkg_Eff)*(TN+FP))
		
		if N_Sig < 1.:
			continue
		if N_Bkg < 1.:
			continue
		if Sig_Eff < 0.90:
			continue
			
		s = N_Sig/math.sqrt(N_Bkg+N_Sig)
		
		S.append(s)
		X.append(p)
		Bkg.append(Bkg_Eff)
		Sig.append(Sig_Eff)
	return np.array(S), np.array(X), np.array(Sig), np.array(Bkg)
		
def getData(Conversion):
	mask = []
	if Conversion == 'UnConv':
		mask = data.Z_Test[:,-4] == 0
	else :
		mask = data.Z_Test[:,-4] != 0
	return mask

def getData_pT_Eta(ptbin, etabin):
	
	pt  = data.Z_Test[:,4]*1e-3
	eta = abs(data.Z_Test[:,5])
	
	PT_min = PT[ptbin]
	PT_max = PT[ptbin+1]
	
	Eta_min = ETA[etabin]
	Eta_max = ETA[etabin+1]
	
	print("PT :[", PT_min, ",", PT_max,"] ETA:[", Eta_min, ",", Eta_max,"]" )
	
	mask_PT_min = pt > PT_min
	mask_PT_max = pt <= PT_max
	
	mask_ETA_min = eta > Eta_min
	mask_ETA_max = eta <= Eta_max
	
	mask = mask_PT_min * mask_PT_max * mask_ETA_min * mask_ETA_max
	
	return mask

def plot_matrix(matrix, title):
	
	matrix_new = np.delete(matrix, crackbin, 1)
	plt.figure()
	plt.imshow(matrix_new*100, cmap='RdBu')
	plt.colorbar()
	plt.clim(0, 100);
	for i in range(matrix_new.shape[0]):
		for j in range(matrix_new.shape[1]):
			plt.text(j, i, float(format(matrix_new[i, j]*100, '.2f')), ha="center", va="center", color="k")
	plt.xlabel('|ETA| bin')
	plt.ylabel('PT bin')
	plt.title(title)
	plt.savefig(output+''+title+'.png')
	plt.close()
	
def merge_TVT(model):
	
	X1_Test = data.Img_Lr1_Test
	X2_Test = data.Img_Lr2_Test
	X3_Test = data.Img_Lr3_Test
	X4_Test = abs(data.ETA_Test)
	
	X1_Train = data.Img_Lr1_Train
	X2_Train = data.Img_Lr2_Train
	X3_Train = data.Img_Lr3_Train
	X4_Train = abs(data.ETA_Train)
	
	X1_Val = data.Img_Lr1_Val
	X2_Val = data.Img_Lr2_Val
	X3_Val = data.Img_Lr3_Val
	X4_Val = abs(data.ETA_Val)
	
	X1_Test, X2_Test, X3_Test    = reshape(X1_Test, X2_Test, X3_Test)
	X1_Train, X2_Train, X3_Train = reshape(X1_Train, X2_Train, X3_Train)
	X1_Val, X2_Val, X3_Val       = reshape(X1_Val, X2_Val, X3_Val)

	pred_test  = model.predict([X1_Test,X2_Test,X3_Test]);
	pred_train = model.predict([X1_Train,X2_Train,X3_Train]);
	pred_val   = model.predict([X1_Val,X2_Val,X3_Val]);

	plt.figure()
	
	plt.hist(pred_train, bins = 50, histtype='step',  density='True')
	plt.hist(pred_test, bins = 50, histtype='step',  density='True')
	plt.hist(pred_val, bins = 50, histtype='step',  density='True')

	plt.legend(['Train', 'Test', 'Validation'])
	plt.savefig(output+'model_merge_TVT.png')
	plt.close()
	
def reshape(X1, X2, X3):
	
	X1 = X1.reshape(X1.shape[0], 2, 56, 1)
	X2 = X2.reshape(X2.shape[0], 11, 7, 1)
	X3 = X3.reshape(X3.shape[0], 11, 4, 1)
	return X1, X2, X3

def plot_scan(Z, X, S, B, tag):
	
	max_Z = Z.argmax()
	plt.figure()
	plt.plot(X,Z)
	plt.plot(X[max_Z], Z[max_Z], 'or')
	plt.legend(['Scan', 'WP ('+str(format(X[max_Z], '.3f'))+','+str(format(Z[max_Z], '.3f'))+')'])
	plt.xlabel('probability cut')
	plt.ylabel('Significance S/sqrt(S+B)')
	plt.savefig(output+'Z_'+tag+'.png')
	plt.close()
	
	plt.figure()
	plt.plot(X,S)
	plt.plot(X[max_Z], S[max_Z], 'or')
	plt.legend(['Scan', 'WP ('+str(format(X[max_Z], '.3f'))+','+str(format(S[max_Z], '.3f'))+')'])
	plt.xlabel('probability cut')
	plt.ylabel('Sig Eff')
	plt.savefig(output+'S_'+tag+'.png')
	plt.close()
	
	plt.figure()
	plt.plot(X,B)
	plt.plot(X[max_Z], B[max_Z], 'or')
	plt.legend(['Scan', 'WP ('+str(format(X[max_Z], '.3f'))+','+str(format(B[max_Z], '.3f'))+')'])
	plt.xlabel('probability cut')
	plt.ylabel('Bkg rejection')
	plt.savefig(output+'B_'+tag+'.png')
	plt.close()

def main():

	args = getArgs();
	path = str(args.path)
	weight = str(args.model)
	N = str(args.nepochs)
	lr = float(args.learning_rate)
	dr = float(args.dropout_rate)
	bt = int(args.batch_size)
	CNNn = int(args.CNNn)
	DNNn = int(args.DNNn)
	Nout = int(args.noutput)

	tag = 'FF_Stg2_CNN_Nep_'+str('20')+'_CNNn_'+str(CNNn)+'_DNNn_' + \
            str(DNNn)+'_lr_'+str(lr)+'_dr_'+str(dr)+'_bt_'+str(bt)

	model = HybridCNN(path, N, lr, dr, bt, CNNn, DNNn, Nout, tag)
    
	#merge_TVT(model)
	#exit()
    
    
	X1 = data.Img_Lr1_Test
	X2 = data.Img_Lr2_Test
	X3 = data.Img_Lr3_Test
	X4 = abs(data.ETA_Test)
	
	X1 = X1.reshape(X1.shape[0], 2, 56, 1)
	X2 = X2.reshape(X2.shape[0], 11, 7, 1)
	X3 = X3.reshape(X3.shape[0], 11, 4, 1)
    
    #Scan respect to Signal Efficiency 

	y_pred = model.predict([X1,X2,X3]);
	y_pred = y_pred.reshape(y_pred.shape[0],)


	Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test, data.Z_Test[:,-2])
	plot_roc_curve(data.Y_Test,y_pred, Sig_Eff, Bkg_Eff,'Inclusive', tag)
	
	CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanEff(data.Y_Test, y_pred, Sig_Eff)
	print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
	

	Z, X, S, B = maxEff(data.Y_Test, y_pred)
	plot_scan(Z, X, S, B, 'Inclusive')
	
	
	Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[getData('UnConv')], data.Z_Test[:,-2][getData('UnConv')])
	plot_roc_curve(data.Y_Test[getData('UnConv')], y_pred[getData('UnConv')], Sig_Eff, Bkg_Eff, 'UnConverted', tag)
	
	CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanEff(data.Y_Test[getData('UnConv')], y_pred[getData('UnConv')], Sig_Eff)
	print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
	
	Z, X, S, B = maxEff(data.Y_Test[getData('UnConv')], y_pred[getData('UnConv')])
	plot_scan(Z, X, S, B, 'UnConverted')
	
	Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[getData('Conv')], data.Z_Test[:,-2][getData('Conv')])
	plot_roc_curve(data.Y_Test[getData('Conv')], y_pred[getData('Conv')], Sig_Eff, Bkg_Eff, 'Converted', tag)
	
	CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanEff(data.Y_Test[getData('Conv')], y_pred[getData('Conv')], Sig_Eff)
	print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
	
	Z, X, S, B = maxEff(data.Y_Test[getData('Conv')], y_pred[getData('Conv')])
	plot_scan(Z, X, S, B, 'Converted')
	
	Auc_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Auc_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Auc_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Bkg_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Sig_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	CNN_Bkg_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Bkg_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Bkg_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	CNN_Sig_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Sig_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Sig_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Bkg_Imp_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Imp_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Imp_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Sig_Imp_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Imp_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Imp_Con = np.zeros((len(PT)-1,len(ETA)-1))
    
	for ptbin in range(0, len(PT)-1):
		for etabin in range(0, len(ETA)-1):
			if etabin == crackbin:
				continue
			
			# Inclusive 
			
			mask = getData_pT_Eta(ptbin, etabin)
			Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			Auc_Inc.itemset((ptbin,etabin), plot_roc_curve_eta_bin(data.Y_Test[mask], y_pred[mask], Sig_Eff, Bkg_Eff, 'Inclusive', ptbin, etabin))
			Bkg_Inc.itemset((ptbin,etabin), Bkg_Eff)
			Sig_Inc.itemset((ptbin,etabin), Sig_Eff)

			CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanEff(data.Y_Test[mask], y_pred[mask], Sig_Eff)
			CNN_Bkg_Inc.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Inc.itemset((ptbin, etabin), CNN_Sig_Eff)

			print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
			
			Bkg_Imp_Inc.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Inc.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))

			# UnConverted 
			
			mask = getData_pT_Eta(ptbin, etabin)*getData('UnConv')
			Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			Auc_Unc.itemset((ptbin,etabin), plot_roc_curve_eta_bin(data.Y_Test[mask], y_pred[mask], Sig_Eff, Bkg_Eff, 'UnConverted', ptbin, etabin))
			Bkg_Unc.itemset((ptbin,etabin), Bkg_Eff)
			Sig_Unc.itemset((ptbin,etabin), Sig_Eff)
			
			
			CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanEff(data.Y_Test[mask], y_pred[mask], Sig_Eff)
			CNN_Bkg_Unc.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Unc.itemset((ptbin, etabin), CNN_Sig_Eff)

			print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
			Bkg_Imp_Unc.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Unc.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))

			# Converted 
			
			mask = getData_pT_Eta(ptbin, etabin)*getData('Conv')
			Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			Auc_Con.itemset((ptbin,etabin), plot_roc_curve_eta_bin(data.Y_Test[mask], y_pred[mask], Sig_Eff, Bkg_Eff, 'Converted', ptbin, etabin))	
			Bkg_Con.itemset((ptbin,etabin), Bkg_Eff)
			Sig_Con.itemset((ptbin,etabin), Sig_Eff)
			
			CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanEff(data.Y_Test[mask], y_pred[mask], Sig_Eff)
			CNN_Bkg_Con.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Con.itemset((ptbin, etabin), CNN_Sig_Eff)

			print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
			Bkg_Imp_Con.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Con.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))


	plot_matrix(Auc_Inc, 'ROC_Inclusive')
	plot_matrix(Auc_Unc, 'ROC_UnConverted')
	plot_matrix(Auc_Con, 'ROC_Converted')
	
	plot_matrix(Bkg_Inc, 'Bkg_Rej_Inclusive')
	plot_matrix(Bkg_Unc, 'Bkg_Rej_UnConverted')
	plot_matrix(Bkg_Con, 'Bkg_Rej_Converted')
	
	plot_matrix(Sig_Inc, 'Sig_Eff_Inclusive')
	plot_matrix(Sig_Unc, 'Sig_Eff_UnConverted')
	plot_matrix(Sig_Con, 'Sig_Eff_Converted')
	
	plot_matrix(CNN_Bkg_Inc, 'CNN_Bkg_Rej_Inclusive')
	plot_matrix(CNN_Bkg_Unc, 'CNN_Bkg_Rej_UnConverted')
	plot_matrix(CNN_Bkg_Con, 'CNN_Bkg_Rej_Converted')
	
	plot_matrix(CNN_Sig_Inc, 'CNN_Sig_Eff_Inclusive')
	plot_matrix(CNN_Sig_Unc, 'CNN_Sig_Eff_UnConverted')
	plot_matrix(CNN_Sig_Con, 'CNN_Sig_Eff_Converted')
	
	plot_matrix(Bkg_Imp_Inc, 'Bkg_Imp_Inclusive')
	plot_matrix(Bkg_Imp_Unc, 'Bkg_Imp_UnConverted')
	plot_matrix(Bkg_Imp_Con, 'Bkg_Imp_Converted')
	
	plot_matrix(Sig_Imp_Inc, 'Sig_Imp_Inclusive')
	plot_matrix(Sig_Imp_Unc, 'Sig_Imp_UnConverted')
	plot_matrix(Sig_Imp_Con, 'Sig_Imp_Converted')
	
	exit()
	
    #--------------------------------------+
    # Scan respect to Background Rejection |
    #--------------------------------------+
	

	Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test, data.Z_Test[:,-2])
	plot_roc_curve(data.Y_Test,y_pred, Sig_Eff, Bkg_Eff,'Inclusive', tag)
	
	CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanRej(data.Y_Test, y_pred, Bkg_Eff)
	print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
	
	Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[getData('UnConv')], data.Z_Test[:,-2][getData('UnConv')])
	plot_roc_curve(data.Y_Test[getData('UnConv')], y_pred[getData('UnConv')], Sig_Eff, Bkg_Eff, 'UnConverted', tag)
	
	CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanRej(data.Y_Test[getData('UnConv')], y_pred[getData('UnConv')], Bkg_Eff)
	print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
	
	Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[getData('Conv')], data.Z_Test[:,-2][getData('Conv')])
	plot_roc_curve(data.Y_Test[getData('Conv')], y_pred[getData('Conv')], Sig_Eff, Bkg_Eff, 'Converted', tag)
	
	CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanRej(data.Y_Test[getData('Conv')], y_pred[getData('Conv')], Bkg_Eff)
	print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
	
	Auc_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Auc_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Auc_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Bkg_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Sig_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	CNN_Bkg_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Bkg_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Bkg_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	CNN_Sig_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Sig_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	CNN_Sig_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Bkg_Imp_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Imp_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Bkg_Imp_Con = np.zeros((len(PT)-1,len(ETA)-1))
	
	Sig_Imp_Inc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Imp_Unc = np.zeros((len(PT)-1,len(ETA)-1))
	Sig_Imp_Con = np.zeros((len(PT)-1,len(ETA)-1))
    
	for ptbin in range(0, len(PT)-1):
		for etabin in range(0, len(ETA)-1):
			if etabin == crackbin:
				continue
			
			# Inclusive 
			
			mask = getData_pT_Eta(ptbin, etabin)
			Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			Auc_Inc.itemset((ptbin,etabin), plot_roc_curve_eta_bin(data.Y_Test[mask], y_pred[mask], Sig_Eff, Bkg_Eff, 'Inclusive', ptbin, etabin))
			Bkg_Inc.itemset((ptbin,etabin), Bkg_Eff)
			Sig_Inc.itemset((ptbin,etabin), Sig_Eff)

			CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanRej(data.Y_Test[mask], y_pred[mask], Bkg_Eff)
			CNN_Bkg_Inc.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Inc.itemset((ptbin, etabin), CNN_Sig_Eff)

			print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
			
			Bkg_Imp_Inc.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Inc.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))

			# UnConverted 
			
			mask = getData_pT_Eta(ptbin, etabin)*getData('UnConv')
			Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			Auc_Unc.itemset((ptbin,etabin), plot_roc_curve_eta_bin(data.Y_Test[mask], y_pred[mask], Sig_Eff, Bkg_Eff, 'UnConverted', ptbin, etabin))
			Bkg_Unc.itemset((ptbin,etabin), Bkg_Eff)
			Sig_Unc.itemset((ptbin,etabin), Sig_Eff)
			
			
			CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanRej(data.Y_Test[mask], y_pred[mask], Bkg_Eff)
			CNN_Bkg_Unc.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Unc.itemset((ptbin, etabin), CNN_Sig_Eff)

			print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
			Bkg_Imp_Unc.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Unc.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))

			# Converted 
			
			mask = getData_pT_Eta(ptbin, etabin)*getData('Conv')
			Sig_Eff, Bkg_Eff = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			Auc_Con.itemset((ptbin,etabin), plot_roc_curve_eta_bin(data.Y_Test[mask], y_pred[mask], Sig_Eff, Bkg_Eff, 'Converted', ptbin, etabin))	
			Bkg_Con.itemset((ptbin,etabin), Bkg_Eff)
			Sig_Con.itemset((ptbin,etabin), Sig_Eff)
			
			CNN_Sig_Eff, CNN_Bkg_Eff, CNN_cut = scanRej(data.Y_Test[mask], y_pred[mask], Bkg_Eff)
			CNN_Bkg_Con.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Con.itemset((ptbin, etabin), CNN_Sig_Eff)

			print("Sig Eff : ", CNN_Sig_Eff, " Bkg Rej : ", CNN_Bkg_Eff, " Cut : ", CNN_cut, " ATLAS Eff : ", Sig_Eff, " ATLAS Rej : ", Bkg_Eff)
			Bkg_Imp_Con.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Con.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))


	plot_matrix(Auc_Inc, 'ROC_Inclusive')
	plot_matrix(Auc_Unc, 'ROC_UnConverted')
	plot_matrix(Auc_Con, 'ROC_Converted')
	
	plot_matrix(Bkg_Inc, 'Bkg_Rej_Inclusive')
	plot_matrix(Bkg_Unc, 'Bkg_Rej_UnConverted')
	plot_matrix(Bkg_Con, 'Bkg_Rej_Converted')
	
	plot_matrix(Sig_Inc, 'Sig_Eff_Inclusive')
	plot_matrix(Sig_Unc, 'Sig_Eff_UnConverted')
	plot_matrix(Sig_Con, 'Sig_Eff_Converted')
	
	plot_matrix(CNN_Bkg_Inc, 'CNN_Bkg_Rej_Inclusive')
	plot_matrix(CNN_Bkg_Unc, 'CNN_Bkg_Rej_UnConverted')
	plot_matrix(CNN_Bkg_Con, 'CNN_Bkg_Rej_Converted')
	
	plot_matrix(CNN_Sig_Inc, 'CNN_Sig_Eff_Inclusive')
	plot_matrix(CNN_Sig_Unc, 'CNN_Sig_Eff_UnConverted')
	plot_matrix(CNN_Sig_Con, 'CNN_Sig_Eff_Converted')
	
	plot_matrix(Bkg_Imp_Inc, 'Bkg_Imp_Inclusive')
	plot_matrix(Bkg_Imp_Unc, 'Bkg_Imp_UnConverted')
	plot_matrix(Bkg_Imp_Con, 'Bkg_Imp_Converted')
	
	plot_matrix(Sig_Imp_Inc, 'Sig_Imp_Inclusive')
	plot_matrix(Sig_Imp_Unc, 'Sig_Imp_UnConverted')
	plot_matrix(Sig_Imp_Con, 'Sig_Imp_Converted')
    
	"""
    
	print('Apply Selection ... ', 0.95)
	
	y_class = y_pred > 0.95
	
	for ptbin in range(0, len(PT)-1):
		for etabin in range(0, len(ETA)-1):
			if etabin == crackbin:
				continue
				
			# Inclusive 
			
			mask = getData_pT_Eta(ptbin, etabin)
			Sig_Eff, Bkg_Eff         = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			CNN_Sig_Eff, CNN_Bkg_Eff = computeTightEff(data.Y_Test[mask], y_class[mask])

			CNN_Bkg_Inc.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Inc.itemset((ptbin, etabin), CNN_Sig_Eff)
			
			Bkg_Imp_Inc.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Inc.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))

			# UnConverted 
			
			mask = getData_pT_Eta(ptbin, etabin)*getData('UnConv')
			Sig_Eff, Bkg_Eff         = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			CNN_Sig_Eff, CNN_Bkg_Eff = computeTightEff(data.Y_Test[mask], y_class[mask])
			
			CNN_Bkg_Unc.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Unc.itemset((ptbin, etabin), CNN_Sig_Eff)

			Bkg_Imp_Unc.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Unc.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))

			# Converted 
			
			mask = getData_pT_Eta(ptbin, etabin)*getData('Conv')
			Sig_Eff, Bkg_Eff         = computeTightEff(data.Y_Test[mask], data.Z_Test[:,-2][mask])
			CNN_Sig_Eff, CNN_Bkg_Eff = computeTightEff(data.Y_Test[mask], y_class[mask])
			
			CNN_Bkg_Con.itemset((ptbin, etabin), CNN_Bkg_Eff)
			CNN_Sig_Con.itemset((ptbin, etabin), CNN_Sig_Eff)

			Bkg_Imp_Con.itemset((ptbin, etabin), ((CNN_Bkg_Eff - Bkg_Eff)/Bkg_Eff))
			Sig_Imp_Con.itemset((ptbin, etabin), (((CNN_Sig_Eff - Sig_Eff)/Sig_Eff)))	

	plot_matrix(CNN_Bkg_Inc, 'CNN_Cut_Bkg_Rej_Inclusive')
	plot_matrix(CNN_Bkg_Unc, 'CNN_Cut_Bkg_Rej_UnConverted')
	plot_matrix(CNN_Bkg_Con, 'CNN_Cut_Bkg_Rej_Converted')
	
	plot_matrix(CNN_Sig_Inc, 'CNN_Cut_Sig_Eff_Inclusive')
	plot_matrix(CNN_Sig_Unc, 'CNN_Cut_Sig_Eff_UnConverted')
	plot_matrix(CNN_Sig_Con, 'CNN_Cut_Sig_Eff_Converted')
	
	plot_matrix(Bkg_Imp_Inc, 'Bkg_Cut_Imp_Inclusive')
	plot_matrix(Bkg_Imp_Unc, 'Bkg_Cut_Imp_UnConverted')
	plot_matrix(Bkg_Imp_Con, 'Bkg_Cut_Imp_Converted')
	
	plot_matrix(Sig_Imp_Inc, 'Sig_Cut_Imp_Inclusive')
	plot_matrix(Sig_Imp_Unc, 'Sig_Cut_Imp_UnConverted')
	plot_matrix(Sig_Imp_Con, 'Sig_Cut_Imp_Converted')
	
	
	for ptbin in range(0, len(PT)-1):
		for etabin in range(0, len(ETA)-1):
			if etabin == crackbin:
				continue
				
			#Inclusive 
			mask = getData_pT_Eta(ptbin, etabin)
			
			#UnConverted 
			
	"""		
			
if __name__ == '__main__':
    main()
