#!/usr/bin/env python
from tensorflow.keras.models import Sequential, Model
from tensorflow.keras.layers import Dense, Activation, Dropout, BatchNormalization, LSTM, concatenate, Input, Conv2D, MaxPooling2D, Flatten, GlobalAveragePooling2D, AlphaDropout, GaussianNoise
from tensorflow.keras import initializers, regularizers


#Classifier class 

class Classifier():
	
	"""
	Classifier class
	
	Args: 
		name : name of the classifier
		n_inputs : number of inputs features
		n_outputs : number of outputs
		drop_rate : dropout rate
	"""

	def __init__(self, name, n_inputs, n_outputs, drop_rate, n_neuros):
		self.NInt = n_inputs;
		self.NOut = n_outputs;
		self.nN   = n_neuros;
		self.DropRate = drop_rate;
		self.Sequential = Sequential()
		self.Sequential.add(LSTM(self.nN, activation='selu',recurrent_activation='sigmoid'))
		self.Sequential.add(BatchNormalization())
		self.Sequential.add(Dense(512, activation='selu'));
		self.Sequential.add(Dropout(self.DropRate));		
		self.Sequential.add(Dense(512, activation='selu'));
		#self.Sequential.add(Dropout(self.DropRate));	
		self.Sequential.add(Dense(256, activation='selu'));
		self.Sequential.add(Dropout(self.DropRate));
		self.Sequential.add(Dense(128, activation='selu'));
		#self.Sequential.add(Dense(4, activation='elu', kernel_initializer=initializers.VarianceScaling(scale=1.0, mode='fan_in', distribution='uniform')));
		self.Sequential.add(Dense(self.NOut, activation='sigmoid'));


class MClassifier():

	def __init__(self, name, n_inputs_1, n_inputs_2, n_inputs_3, n_out, lstm1, lstm2, lstm3, n1, n2, n3, n, d1, d2, d3, d):
		
		self.name = name;
		self.Lr1Size = 112;
		self.Lr2Size = 77; 
		self.Lr3Size = 44;
	
		self.nI1 = n_inputs_1;
		self.nI2 = n_inputs_2;
		self.nI3 = n_inputs_3;

		self.NLSTM1 = lstm1;
		self.NLSTM2 = lstm2;
		self.NLSTM3 = lstm3;

		self.N1 = n1;
		self.N2 = n2;
		self.N3 = n3;
		self.N  = n;
		
		self.D1 = d1;
		self.D2 = d2;
		self.D3 = d3;
		self.D  =  d;

		self.Nout = n_out;
		
		"""
		DNN1-Lr1
		"""
			
		input_N1 = Input(shape=(self.nI1,self.Lr1Size), name='Input1');
		#x_1 = input_N1
		x_1 = BatchNormalization()(input_N1);
		x_1 = LSTM(self.NLSTM1, activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='LSTM1' )(x_1)
		#x_1 = Dense(self.N1,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D11'   )(x_1)
		#x_1 = Dropout(self.D1)(x_1)
		#x_1 = Dense(self.N1,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D12'   )(x_1)
		#x_1 = Dense(self.N1,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D13'   )(x_1)
		out_1 = Dense(self.N1,  activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D14'   )(x_1)

		"""
		DNN2-Lr2
		"""
		input_N2 = Input(shape=(self.nI2,self.Lr2Size), name='Input2');
		#x_2 = input_N2
		x_2 = BatchNormalization()(input_N2);
		x_2 = LSTM(self.NLSTM2, activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='LSTM2' )(x_2)
		#x_2 = Dense(self.N2,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D21'   )(x_2)
		#x_2 = Dropout(self.D2)(x_2)
		#x_2 = Dense(self.N2,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D22'   )(x_2)
		#x_2 = Dense(self.N2,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D23'   )(x_2)
		out_2 = Dense(self.N2,  activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D24'   )(x_2)
		
		"""
		DNN3-Lr3
		"""
		input_N3 = Input(shape=(self.nI3,self.Lr3Size), name='Input3');
		#x_3 = input_N3
		x_3 = BatchNormalization()(input_N3);
		x_3 = LSTM(self.NLSTM3, activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='LSTM3' )(x_3)
		#x_3 = Dense(self.N3,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D31'   )(x_3)
		#x_3 = Dropout(self.D3)(x_3)
		#x_3 = Dense(self.N3,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D32'   )(x_3)
		#x_3 = Dense(self.N3,   activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D33'   )(x_3)
		out_3 = Dense(self.N3,  activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D34'   )(x_3)
		

		"""
		DNN-Photon
		"""

		x = concatenate([out_1,out_2,out_3]);
		x = Dense(self.N,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D1'   )(x)
		x = Dropout(self.D)(x)
		x = Dense(self.N,    activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D2'   )(x)
		x = Dense(4,   activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='D3'   )(x)
		out = Dense(self.Nout,  activation='sigmoid', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), name='Output')(x)

		self.model = Model(inputs=[input_N1, input_N2, input_N3], outputs=out)

class SSClassifier():
	
	"""
	Classifier class
	
	Args: 
		name : name of the classifier
		n_inputs : number of inputs features
		n_outputs : number of outputs
		drop_rate : dropout rate
	"""

	def __init__(self, name, n_inputs, n_outputs, drop_rate, n_neuros):
		
		self.name = name;
		self.NInt = n_inputs;
		self.NOut = n_outputs;
		self.nN       = n_neuros;
		self.DropRate = drop_rate;

		self.Sequential = Sequential()
		self.Sequential.add(BatchNormalization())
		self.Sequential.add(Dense(n_neuros, activation='selu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));
		self.Sequential.add(Dropout(self.DropRate));		
		self.Sequential.add(Dense(n_neuros, activation='selu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));
		#self.Sequential.add(Dense(n_neuros, activation='selu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));
		#self.Sequential.add(Dropout(self.DropRate));
		#self.Sequential.add(Dense(n_neuros, activation='selu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));
		self.Sequential.add(Dense(self.NOut, activation='sigmoid', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));


class CNNClassifier():
	
	def __init__(self,name):


		self.Sequential = Sequential()
		self.Sequential.add(Conv2D(128,  kernel_size=2, activation='relu', input_shape=(11,7,1)))
		self.Sequential.add(MaxPooling2D((2, 2)))
		self.Sequential.add(Conv2D(128, kernel_size=2, activation='relu'))
		self.Sequential.add(MaxPooling2D((2, 2)))
		self.Sequential.add(Flatten())
		self.Sequential.add(Dense(94, activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));
		self.Sequential.add(Dense(94, activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));
		self.Sequential.add(Dropout(0.1));
		self.Sequential.add(Dense(32, activation='relu', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));
		self.Sequential.add(Dense(1, activation='sigmoid', kernel_initializer=initializers.VarianceScaling(scale=2.0, mode='fan_in', distribution='uniform'), bias_initializer='zeros'));

class CNNHybridClassifier():
	
	def __init__(self,name, N_CNN_neurons, N_DNN_neurons, Noutputs, dropout_rate):

		self.name = name

		input1 = Input(shape=(2,56,1,), name='Input1');
		X1 = Conv2D(N_CNN_neurons,  kernel_size=(2,2), activation='selu',kernel_initializer='lecun_normal', name='Cov11')(input1)
		X1 = MaxPooling2D((1, 2), name='MaxP11')(X1)
		X1 = Conv2D(N_CNN_neurons,  kernel_size=(1,2), activation='selu',kernel_initializer='lecun_normal', name='Cov12')(X1)
		X1 = MaxPooling2D((1, 2), name='MaxP12')(X1)
		#X1 = GlobalAveragePooling2D(name="AvgMaxP1")(X1)
		X1 = Flatten(name='Flat1')(X1)

		input2 = Input(shape=(11,7,1), name='Input2');
		X2 = Conv2D(N_CNN_neurons,  kernel_size=(2,2), activation='selu', kernel_initializer='lecun_normal', name='Cov21')(input2)
		X2 = MaxPooling2D((2, 1), name='MaxP21')(X2)
		X2 = Conv2D(N_CNN_neurons,  kernel_size=(2,1), activation='selu',kernel_initializer='lecun_normal', name='Cov22')(X2)
		X2 = MaxPooling2D((2, 1), name='MaxP22')(X2)
		#X2 = GlobalAveragePooling2D(name="AvgMaxP2")(X2)
		X2 = Flatten(name='Flat2')(X2)
		 

		input3 = Input(shape=(11,4,1), name='Input3');
		X3 = Conv2D(N_CNN_neurons,  kernel_size=(2,2), activation='selu',kernel_initializer='lecun_normal', name='Cov31')(input3)
		X3 = MaxPooling2D((2, 1), name='MaxP31')(X3)
		X3 = Conv2D(N_CNN_neurons,  kernel_size=(2,1), activation='selu',kernel_initializer='lecun_normal', name='Cov32')(X3)
		X3 = MaxPooling2D((2, 1), name='MaxP32')(X3)
		#X3 = GlobalAveragePooling2D(name="AvgMaxP3")(X3)
		X3 = Flatten(name='Flat3')(X3)
		
		#input4 = Input(shape=(1,), name='Input4');
		#X4 = Dense(64, activation='selu', kernel_initializer='lecun_normal', name='D41')(input4)
		
		X = concatenate([X1,X2,X3], name='Concatenate');
		X = Dense(N_DNN_neurons, activation='selu', kernel_initializer='lecun_normal', name='D1')(X)
		X = GaussianNoise(0.1)(X);
		#X = AlphaDropout(dropout_rate, name='DO1')(X)
		X = Dense(N_DNN_neurons, activation='selu', kernel_initializer='lecun_normal', name='D2')(X)
		X = GaussianNoise(0.1)(X);
		#X = Dropout(dropout_rate, name='DO2')(X)
		X = Dense(N_DNN_neurons, activation='selu', kernel_initializer='lecun_normal', name='D3')(X)
		#X = Dropout(dropout_rate, name='DO3')(X)
		out = Dense(Noutputs,  activation='sigmoid', kernel_initializer='lecun_normal', name='Output')(X)
		
		self.model = Model(inputs=[input1,input2,input3], outputs=out)
	
		

		
		