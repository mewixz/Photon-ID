#!/usr/bin/env python
from tensorflow.python.keras import backend as K
from tensorflow.python.keras.models import load_model
from tensorflow.keras import optimizers
import onnx
import keras2onnx
import onnxruntime
from argparse import ArgumentParser
import sys
sys.path.append('../')
from DataProcessing.DataProcessing import Zlly
from Models.models import CNNHybridClassifier
data = Zlly(1)

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
    
	print(tag)
	model = HybridCNN(path, N, lr, dr, bt, CNNn, DNNn, Nout, tag)
	print('Model : ', tag)
	#model.save('ONNX/'+tag+'.h5')
	#model = load_model('ONNX/'+tag+'.h5')
	#onnx_model = keras2onnx.convert_keras(model, model.name)
	#onnx.save_model(onnx_model, 'ONNX/'+tag+'.onnx')
    
	X1 = data.Img_Lr1[0,:,:]
	X2 = data.Img_Lr2[0,:,:]
	X3 = data.Img_Lr3[0,:,:]
    
	print('X1')
	print(X1)
	print('X2')
	print(X2)
	print('X3')
	print(X3)

	X1 = X1.reshape(1, 2, 56, 1)
	X2 = X2.reshape(1, 11, 7, 1)
	X3 = X3.reshape(1, 11, 4, 1)
    

	Keras_model = load_model('ONNX/'+tag+'.h5')
	ONNX_model  = onnxruntime.InferenceSession('ONNX/'+tag+'.onnx')
	pred = Keras_model.predict([X1,X2,X3])
	print('Keras probabilty : ' , pred[0])
	X = [X1, X2, X3]
	feed = dict([(input.name, X[n]) for n, input in enumerate(ONNX_model.get_inputs())])
	pred_onnx = ONNX_model.run(None, feed)[0]
	print('ONNX probabilty : ' , pred_onnx[0])


if __name__ == '__main__':
    main()