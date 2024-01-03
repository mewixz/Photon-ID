#!/usr/bin/env python
import numpy as np
import sys
sys.path.append('../Train/')
from DataProcessing.DataProcessing import Data, Zlly, AFS_FS, HH



data = Zlly(4,Iso='Loose', inputDir='/sps/atlas/m/mbelfkir/Zlly/')
path = '/sps/atlas/m/mbelfkir/Zlly/Stg_4/Tree/'


#data = Data(2)
#path = '/sps/atlas/m/mbelfkir/output_FF/Stg_2/'

#data = HH()
#path = '/sps/atlas/m/mbelfkir/output_yybb/SM/Tree/'

def setData(data, Eta, Phi):
	out = []
	for i in range(0,data.shape[0]):
		#print(i, data.shape[0])
		mtrx = []
		sumE = np.sum(data[i,:])
		
		for phi in range(0,Phi):
			#print('Phi : ', phi, Phi)
			EtaCl = []
			for eta in range(0,Eta):
				if sumE == 0:
					EtaCl.append(data[i,phi + Phi*eta])
				else:
					EtaCl.append(data[i,phi + Phi*eta]/sumE)
			mtrx.append(EtaCl)
		mtrx2 = np.matrix(mtrx)
		out.append(mtrx2)
	return np.asarray(out)

def CompletCluster(data, Eta, Phi):
	out = [] 
	for i in range(0, data.shape[0]):
		matrix = np.zeros((Phi,Eta))
		sumE = np.sum(data[i,:])
		for phi in range(0,Phi):
			for eta in range(0, Eta):
				matrix.itemset((eta, phi), data[i,phi + Phi*eta]/sumE)
				
		out.append(matrix)
	return np.asarray(out)



Lr2 = setData(data.X2[:,0,:], 7, 11)
Lr1 = setData(data.X1[:,0,:], 56, 2)
Lr3 = setData(data.X3[:,0,:], 4, 11)

np.save(path+'Img_Lr2_Tree_IsoLoose.npy',Lr2)
np.save(path+'Img_Lr1_Tree_IsoLoose.npy',Lr1)
np.save(path+'Img_Lr3_Tree_IsoLoose.npy',Lr3)

exit()


Lr2_Train = setData(data.X2_Train[:,0,:], 7, 11)
Lr2_Val   = setData(data.X2_Val[:,0,:], 7, 11)
Lr2_Test  = setData(data.X2_Test[:,0,:], 7, 11)

np.save(path+'Tree_Train/Img_Lr2_Train.npy',Lr2_Train)
np.save(path+'Tree_Val/Img_Lr2_Val.npy',Lr2_Val)
np.save(path+'Tree_Test/Img_Lr2_Test.npy',Lr2_Test)


Lr1_Train = setData(data.X1_Train[:,0,:], 56, 2)
Lr1_Val   = setData(data.X1_Val[:,0,:], 56, 2)
Lr1_Test  = setData(data.X1_Test[:,0,:], 56, 2)

np.save(path+'Tree_Train/Img_Lr1_Train.npy',Lr1_Train)
np.save(path+'Tree_Val/Img_Lr1_Val.npy',Lr1_Val)
np.save(path+'Tree_Test/Img_Lr1_Test.npy',Lr1_Test)

Lr3_Train = setData(data.X3_Train[:,0,:], 4, 11)
Lr3_Val   = setData(data.X3_Val[:,0,:], 4, 11)
Lr3_Test  = setData(data.X3_Test[:,0,:], 4, 11)

np.save(path+'Tree_Train/Img_Lr3_Train.npy',Lr3_Train)
np.save(path+'Tree_Val/Img_Lr3_Val.npy',Lr3_Val)
np.save(path+'Tree_Test/Img_Lr3_Test.npy',Lr3_Test)
