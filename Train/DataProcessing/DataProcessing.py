#!/usr/bin/env python
import numpy as np

class Data:
	"""
	Data class for data pre-processing
	Training type DNN or ANN
	"""

	def __init__(self,Strategy):
		self.Dir = '/sps/atlas/m/mbelfkir/output_FF/';

		self.Stg = Strategy;
		print('Loading Test Data ...')
		self.loadTestData();
		print('Loading Validation Data ...')
		self.loadValData();
		print('Loading Training Data ...')
		self.loadTrainData();
		self.setXYZ();
		#self.convertY();

	def setXYZ(self):

		self.X1_Train = self.TrainDataX1[:,:];
		self.X2_Train = self.TrainDataX2[:,:];
		self.X3_Train = self.TrainDataX3[:,:];

		self.Y_Train = self.TrainDataY[:,-1];
	
		self.Z_Train = self.TrainDataY[:,:-1]
		
		self.X1_Val = self.ValDataX1[:,:];
		self.X2_Val = self.ValDataX2[:,:];
		self.X3_Val = self.ValDataX3[:,:];

		self.Y_Val = self.ValDataY[:,-1];

		self.Z_Val = self.ValDataY[:,:-1]

		self.X1_Test = self.TestDataX1[:,:];
		self.X2_Test = self.TestDataX2[:,:];
		self.X3_Test = self.TestDataX3[:,:];

		self.Y_Test = self.TestDataY[:,-1];
		self.Z_Test = self.TestDataY[:,:-1];

		self.SS_Train = self.SetSSVar(self.Z_Train)
		self.SS_Val   = self.SetSSVar(self.Z_Val)
		self.SS_Test  = self.SetSSVar(self.Z_Test)
		
		self.Eta_Train = self.Z_Train[:,5]
		self.Pt_Train  = self.Z_Train[:,4]
			
		self.Eta_Val   = self.Z_Val[:,5]
		self.Pt_Val    = self.Z_Val[:,4]
		
		self.Eta_Test  = self.Z_Test[:,5]
		self.Pt_Test   = self.Z_Test[:,4]


	def loadGlobalData(self):
		
		self.GlobalDataX = np.load(self.GetName('GammaJet','Tree_Train','X2'), allow_pickle=True);
		self.GlobalDataY = np.load(self.GetName('GammaJet','Tree_Train','Y'), allow_pickle=True);

	def loadTrainData(self):
		
		self.TrainDataX1 = np.load(self.GetName('GammaJet','Tree_Train','X2'), allow_pickle=True);
		self.TrainDataX1 = np.append(self.TrainDataX1, np.load(self.GetName('DiJet','Tree_Train','X2'), allow_pickle=True), axis=0)
		self.TrainDataX2 = np.load(self.GetName('GammaJet','Tree_Train','X1'), allow_pickle=True);
		self.TrainDataX2 = np.append(self.TrainDataX2, np.load(self.GetName('DiJet','Tree_Train','X1'), allow_pickle=True), axis=0)
		self.TrainDataX3 = np.load(self.GetName('GammaJet','Tree_Train','X3'), allow_pickle=True);
		self.TrainDataX3 = np.append(self.TrainDataX3, np.load(self.GetName('DiJet','Tree_Train','X3'), allow_pickle=True), axis=0)
		self.TrainDataY  = np.load(self.GetName('GammaJet','Tree_Train','Y'), allow_pickle=True);
		self.TrainDataY  = np.append(self.TrainDataY, np.load(self.GetName('DiJet','Tree_Train','Y'), allow_pickle=True), axis=0)

	def loadTestData(self):
		
		self.TestDataX1 = np.load(self.GetName('GammaJet','Tree_Test','X2'), allow_pickle=True);
		self.TestDataX1 = np.append(self.TestDataX1, np.load(self.GetName('DiJet','Tree_Test','X2'), allow_pickle=True), axis=0)
		self.TestDataX2 = np.load(self.GetName('GammaJet','Tree_Test','X1'), allow_pickle=True);
		self.TestDataX2 = np.append(self.TestDataX2, np.load(self.GetName('DiJet','Tree_Test','X1'), allow_pickle=True), axis=0)
		self.TestDataX3 = np.load(self.GetName('GammaJet','Tree_Test','X3'), allow_pickle=True);
		self.TestDataX3 = np.append(self.TestDataX3, np.load(self.GetName('DiJet','Tree_Test','X3'), allow_pickle=True), axis=0)
		self.TestDataY  = np.load(self.GetName('GammaJet','Tree_Test','Y'), allow_pickle=True);
		self.TestDataY  = np.append(self.TestDataY, np.load(self.GetName('DiJet','Tree_Test','Y'), allow_pickle=True), axis=0)

	def loadValData(self):
		
		self.ValDataX1 = np.load(self.GetName('GammaJet','Tree_Val','X2'), allow_pickle=True);
		self.ValDataX1 = np.append(self.ValDataX1, np.load(self.GetName('DiJet','Tree_Val','X2'), allow_pickle=True), axis=0)
		self.ValDataX2 = np.load(self.GetName('GammaJet','Tree_Val','X1'), allow_pickle=True);	
		self.ValDataX2 = np.append(self.ValDataX2, np.load(self.GetName('DiJet','Tree_Val','X1'), allow_pickle=True), axis=0)	
		self.ValDataX3 = np.load(self.GetName('GammaJet','Tree_Val','X3'), allow_pickle=True);
		self.ValDataX3 = np.append(self.ValDataX3, np.load(self.GetName('DiJet','Tree_Val','X3'), allow_pickle=True), axis=0)
		self.ValDataY = np.load(self.GetName('GammaJet','Tree_Val','Y'), allow_pickle=True);
		self.ValDataY = np.append(self.ValDataY, np.load(self.GetName('DiJet','Tree_Val','Y'), allow_pickle=True), axis=0)

	def convertY(self):
		
		self.Train_Sig = 0.
		self.Train_Bkg = 0.

		self.Val_Sig   = 0.
		self.Val_Bkg   = 0.
		
		self.Test_Sig  = 0.
		self.Test_Bkg  = 0.

		self.Train_UncSig = 0.
		self.Train_UncBkg = 0.

		self.Val_UncSig   = 0.
		self.Val_UncBkg   = 0.
		
		self.Test_UncSig  = 0.
		self.Test_UncBkg  = 0.

		self.Train_CSig = 0.
		self.Train_CBkg = 0.

		self.Val_CSig   = 0.
		self.Val_CBkg   = 0.
		
		self.Test_CSig  = 0.
		self.Test_CBkg  = 0.

		self.Y_Train1 = []
		self.Y_Val1   = []
		self.Y_Test1  = []
		for i in range(0,self.Y_Train.shape[0]):
			if(self.Y_Train[i] == True):
				self.Y_Train1.append(1)
				self.Train_Sig  = self.Train_Sig + 1.
				if self.Z_Train[i,-4] == 0:
					self.Train_UncSig =  self.Train_UncSig + 1.
				else:
					self.Train_CSig = self.Train_CSig + 1. 
			else:
				self.Y_Train1.append(0)
				self.Train_Bkg  = self.Train_Bkg + 1.
				if self.Z_Train[i,-4] == 0:
					self.Train_UncBkg =  self.Train_UncBkg + 1.
				else:
					self.Train_CBkg = self.Train_CBkg + 1.

		for i in range(0,self.Y_Val.shape[0]):
			if(self.Y_Val[i] == True):
				self.Y_Val1.append(1)
				self.Val_Sig  = self.Val_Sig + 1. 
				if self.Z_Val[i,-4] == 0:
					self.Val_UncSig =  self.Val_UncSig + 1.
				else:
					self.Val_CSig = self.Val_CSig + 1. 
			else:
				self.Y_Val1.append(0)
				self.Val_Bkg  = self.Val_Bkg + 1.
				if self.Z_Val[i,-4] == 0:
					self.Val_UncBkg =  self.Val_UncBkg + 1.
				else:
					self.Val_CBkg = self.Val_CBkg + 1.

		for i in range(0,self.Y_Test.shape[0]):
			if(self.Y_Test[i] == True):
				self.Y_Test1.append(1)
				self.Test_Sig  = self.Test_Sig + 1.
				if self.Z_Test[i,-4] == 0:
					self.Test_UncSig =  self.Test_UncSig + 1.
				else:
					self.Test_CSig = self.Test_CSig + 1. 
			else:
				self.Y_Test1.append(0)
				self.Test_Bkg  = self.Test_Bkg + 1.
				if self.Z_Test[i,-4] == 0:
					self.Test_UncBkg =  self.Test_UncBkg + 1.
				else:
					self.Test_CBkg = self.Test_CBkg + 1.

		self.Y_Train = np.array(self.Y_Train1)
		self.Y_Val = np.array(self.Y_Val1)
		self.Y_Test = np.array(self.Y_Test1)
		

	def SetSSVar(self, a):
		a1 = a[:,20:24];
		a2 = a[:,25:35];
		a3 = a[:,37:39];
		a4 = a[:,40:42];
		a5 = a[:,43:44];	
	
		a1 = np.append(a1,a2, axis=-1)
		a1 = np.append(a1,a3, axis=-1)
		a1 = np.append(a1,a4, axis=-1)
		a1 = np.append(a1,a5, axis=-1)

		return a1	

	def GetName(self, a, t, x):

		name = self.Dir+'Stg_'+str(self.Stg)+'/'+t+'/'+a+'_IsoTight.'+t+'.Stg_'+str(self.Stg)+'_1_'+x+'.npy';
		return name;
		
	
class CNNData:
	def __init__(self,Strategy):
		self.data = Data(Strategy)
		self.data_un = Data(2)
        
		print('Loading CNN Data ...')
		self.Img_Lr2_Train = self.setData('Train','Lr2')[:,:,:]
		self.Img_Lr2_Val   = self.setData('Val','Lr2')[:,:,:]

		self.Img_Lr1_Train = self.setData('Train','Lr1')[:,:,:]
		self.Img_Lr1_Val   = self.setData('Val','Lr1')[:,:,:]

		self.Img_Lr3_Train = self.setData('Train','Lr3')[:,:,:]
		self.Img_Lr3_Val   = self.setData('Val','Lr3')[:,:,:]
        
		self.Img_Lr3_Test  = np.append(self.setData('Test','Lr3')[:,:,:], self.Img_Lr3_Val, axis=0)
		self.Img_Lr3_Test  = np.append(self.Img_Lr3_Test, self.Img_Lr3_Train, axis=0)
        
		self.Img_Lr2_Test  = np.append(self.setData('Test','Lr2')[:,:,:], self.Img_Lr2_Val, axis=0)
		self.Img_Lr2_Test  = np.append(self.Img_Lr2_Test, self.Img_Lr2_Train, axis=0)
        
		self.Img_Lr1_Test  = np.append(self.setData('Test','Lr1')[:,:,:], self.Img_Lr1_Val, axis=0)
		self.Img_Lr1_Test  = np.append(self.Img_Lr1_Test, self.Img_Lr1_Train, axis=0)
        
		print('CNN Data Loaded')

		self.Y_Train    = np.append(self.data.Y_Train[:], self.data_un.Y_Train[:], axis = 0)
		self.Y_Val      = np.append(self.data.Y_Val[:],   self.data_un.Y_Val[:],   axis = 0)
		self.Y_Test     = np.append(self.data.Y_Test[:],  self.data_un.Y_Test[:],  axis = 0)

		self.Y_Test     = np.append(self.Y_Test, self.Y_Val,   axis=0)
		self.Y_Test     = np.append(self.Y_Test, self.Y_Train, axis=0) 

		self.Z_Train    = np.append(self.data.Z_Train[:,:], self.data_un.Z_Train[:,:], axis = 0)
		self.Z_Val      = np.append(self.data.Z_Val[:,:],   self.data_un.Z_Val[:,:],   axis = 0)
		self.Z_Test     = np.append(self.data.Z_Test[:,:],  self.data_un.Z_Test[:,:],  axis = 0)

		self.Z_Test     = np.append(self.Z_Test, self.Z_Val,   axis=0)
		self.Z_Test     = np.append(self.Z_Test, self.Z_Train, axis=0)
       
		
		self.ETA_Train  = np.append(self.data.Eta_Train.reshape(self.data.Eta_Train.shape[0],1), self.data_un.Eta_Train.reshape(self.data_un.Eta_Train.shape[0],1), axis=0)
		self.ETA_Val    = np.append(self.data.Eta_Val.reshape(self.data.Eta_Val.shape[0],1),     self.data_un.Eta_Val.reshape(self.data_un.Eta_Val.shape[0],1),     axis=0)
		self.ETA_Test   = np.append(self.data.Eta_Test.reshape(self.data.Eta_Test.shape[0],1),   self.data_un.Eta_Test.reshape(self.data_un.Eta_Test.shape[0],1),     axis=0)
        
		self.ETA_Test   = np.append(self.ETA_Test, self.ETA_Val, axis=0)
		self.ETA_Test   = np.append(self.ETA_Test, self.ETA_Train, axis=0)
		
		self.PT_Train   = np.append(self.data.Pt_Train.reshape(self.data.Pt_Train.shape[0],1),  self.data.Pt_Train.reshape(self.data.Pt_Train.shape[0],1), axis=0)
		self.PT_Val     = np.append(self.data.Pt_Val.reshape(self.data.Pt_Val.shape[0],1),      self.data.Pt_Val.reshape(self.data.Pt_Val.shape[0],1),     axis=0)
		self.PT_Test    = np.append(self.data.Pt_Test.reshape(self.data.Pt_Test.shape[0],1),    self.data.Pt_Test.reshape(self.data.Pt_Test.shape[0],1),   axis=0)
        
		self.PT_Test   = np.append(self.PT_Test, self.PT_Val, axis=0)
		self.PT_Test   = np.append(self.PT_Test, self.PT_Train, axis=0)

	def setData(self, name, lr):
		return np.append(np.load(self.data.Dir+'Stg_'+str(self.data.Stg)+'/Tree_'+name+'/Img_'+lr+'_'+name+'.npy', allow_pickle=True), np.load(self.data_un.Dir+'Stg_'+str(self.data_un.Stg)+'/Tree_'+name+'/Img_'+lr+'_'+name+'.npy', allow_pickle=True), axis=0);
			
	
class Zlly:
    
	def __init__(self, Strategy, inputDir='/sps/atlas/m/mbelfkir/Zllg/',Iso='Tight'):
		self.Stg = Strategy
		self.Dir = inputDir
		self.Iso = Iso
		self.load();
		self.setXYZ();
		try:
			self.setCNNData()
		except:
			print('CNN not yet ready, please generate it by CelltoImage')
            
	def load(self): 
		self.DataX1 = np.load(self.GetName('Zlly','Tree','X2'), allow_pickle=True);
		self.DataX2 = np.load(self.GetName('Zlly','Tree','X1'), allow_pickle=True);	
		self.DataX3 = np.load(self.GetName('Zlly','Tree','X3'), allow_pickle=True);
		self.DataY = np.load(self.GetName('Zlly','Tree','Y'), allow_pickle=True);
        

	def GetName(self, a, t, x):
		name = self.Dir+'Stg_'+str(self.Stg)+'/'+t+'/'+a+'_Iso'+self.Iso+'.'+t+'.Stg_'+str(self.Stg)+'_1_'+x+'.npy';
		return name;
    
	def setXYZ(self):
		self.X1 = self.DataX1[:,:];
		self.X2 = self.DataX2[:,:];
		self.X3 = self.DataX3[:,:];
		self.Y = self.DataY[:,-1] == 22;
		self.Z = self.DataY[:,:-1];
        
		self.ETA = self.Z[:,4]
		self.PT  = self.Z[:,3]
		self.E   = self.Z[:,6]
        
	def getData(self, name, lr):
		#return np.load(self.Dir+str(self.Stg)+'/Tree/Img_'+lr+'_'+name+'.npy', allow_pickle=True);
		return np.load(self.Dir+'Stg_'+str(self.Stg)+'/Tree/Img_'+lr+'_'+name+'_Iso'+self.Iso+'.npy', allow_pickle=True);
        
	def setCNNData(self):
        
		self.Img_Lr2 = self.getData('Tree','Lr2')[:,:,:]
		self.Img_Lr1 = self.getData('Tree','Lr1')[:,:,:]
		self.Img_Lr3 = self.getData('Tree','Lr3')[:,:,:]
        
class AFS_FS:
    
	def __init__(self, Strategy, Sim='AFS'):
		self.Stg = Strategy
		self.Dir = '/sps/atlas/m/mbelfkir/'+ Sim +'/'
		self.load();
		self.setXYZ();
		try:
			self.setCNNData()
		except:
			print('CNN not yet ready, please generate it by CelltoImage')
            
	def load(self): 
		self.DataX1 = np.load(self.GetName('GammaJet','Tree_Test','X2'), allow_pickle=True);
		self.DataX2 = np.load(self.GetName('GammaJet','Tree_Test','X1'), allow_pickle=True);	
		self.DataX3 = np.load(self.GetName('GammaJet','Tree_Test','X3'), allow_pickle=True);
		self.DataY = np.load(self.GetName('GammaJet','Tree_Test','Y'), allow_pickle=True);
        

	def GetName(self, a, t, x):
		name = self.Dir+'Stg_'+str(self.Stg)+'/'+t+'/'+a+'_IsoTight.'+t+'.Stg_'+str(self.Stg)+'_1_'+x+'.npy';
		return name;
    
	def setXYZ(self):
		self.X1 = self.DataX1[:,:];
		self.X2 = self.DataX2[:,:];
		self.X3 = self.DataX3[:,:];
		self.Y = self.DataY[:,-1];
		self.Z = self.DataY[:,:-1];
        
		self.ETA = self.Z[:,5]
		self.PT  = self.Z[:,4]
		self.E   = self.Z[:,7]
        
	def getData(self, name, lr):
		return np.load(self.Dir+'Stg_'+str(self.Stg)+'/Tree_Test/Img_'+lr+'_'+name+'.npy', allow_pickle=True);
        
	def setCNNData(self):
        
		self.Img_Lr2 = self.getData('Tree_Test','Lr2')[:,:,:]
		self.Img_Lr1 = self.getData('Tree_Test','Lr1')[:,:,:]
		self.Img_Lr3 = self.getData('Tree_Test','Lr3')[:,:,:]


class HH:
    
	def __init__(self, kappa='SM', inputDir='/sps/atlas/m/mbelfkir/output_yybb/'):
		self.Stg = kappa
		self.Dir = inputDir
		self.load();
		self.setXYZ();
		try:
			self.setCNNData()
		except:
			print('CNN not yet ready, please generate it by CelltoImage')
            
	def load(self): 
		self.DataX1 = np.load(self.GetName('hh_yybb','Tree','X2'), allow_pickle=True);
		self.DataX2 = np.load(self.GetName('hh_yybb','Tree','X1'), allow_pickle=True);	
		self.DataX3 = np.load(self.GetName('hh_yybb','Tree','X3'), allow_pickle=True);
		self.DataY = np.load(self.GetName('hh_yybb','Tree','Y'), allow_pickle=True);
        

	def GetName(self, a, t, x):
		name = self.Dir+str(self.Stg)+'/'+t+'/'+a+'.'+t+'._1_'+x+'.npy';
		return name;
    
	def setXYZ(self):
		self.X1 = self.DataX1[:,:];
		self.X2 = self.DataX2[:,:];
		self.X3 = self.DataX3[:,:];
		self.Y = self.DataY[:,-1];
		self.Z = self.DataY[:,:-1];
        
		self.ETA = self.Z[:,-19]
		self.PT  = self.Z[:,-20]
		self.PHI = self.Z[:,-18]
		self.E   = self.Z[:,-17]
        
	def getData(self, name, lr):
		return np.load(self.Dir+str(self.Stg)+'/Tree/Img_'+lr+'_'+name+'.npy', allow_pickle=True);
        
	def setCNNData(self):
        
		self.Img_Lr2 = self.getData('Tree','Lr2')[:,:,:]
		self.Img_Lr1 = self.getData('Tree','Lr1')[:,:,:]
		self.Img_Lr3 = self.getData('Tree','Lr3')[:,:,:]
