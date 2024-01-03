import numpy as np
from tensorflow.keras.callbacks import Callback
from sklearn.metrics import confusion_matrix, f1_score, precision_score, recall_score, roc_curve, roc_auc_score
import tensorflow as tf
from scipy import interp
import scipy 

class roc_on_epoch(Callback):
	def on_train_begin(self, logs):
		self.rocs = []

	def on_epoch_end(self, epoch, logs):

		val_data   =self.validation_data
		print(val_data) 
		val_pred   = self.model.predict([self.validation_data[0].reshape(self.validation_data[0].shape[0],2,56,1), self.validation_data[1].reshape(self.validation_data[0].shape[0],11,7,1), self.validation_data[2].reshape(self.validation_data[0].shape[0],11,4,1)]).round()
		val_target = self.validation_data[3]
		auc = roc_auc_score(val_target, val_pred, average='micro')
		print('AUC = %0.4f' %  auc )
		self.rocs.append(auc)
		logs['val_AUC'] = auc
		#print(logs)

	def on_train_end(self, logs):
		rocs = np.array(self.rocs)
		print(' Averge AUC = %0.3f +/- %0.3f ' % (np.mean(rocs), np.std(rocs)))


def multiIDG(generator, X1, X2, X3, Y, batch_size, img_h, img_W, subset):

	genX1 = generator.flow(x=X1,y=Y, batch_size=batch_size, shuffle=False, subset=subset)
	genX2 = generator.flow(x=X2,y=Y, batch_size=batch_size, shuffle=False, subset=subset)
	genX3 = generator.flow(x=X3,y=Y, batch_size=batch_size, shuffle=False, subset=subset)

	while True:
	
		X1i = genX1.next()
		X2i = genX2.next()
		X3i = genX3.next()
	
		yield [X1i[0], X2i[0], X3i[0]], X1i[1]
	
		
