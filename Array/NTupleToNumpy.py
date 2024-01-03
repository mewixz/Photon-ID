#!/usr/bin/env python

import ROOT 
import uproot as pr
import pandas as pd
import numpy as np
from argparse import ArgumentParser
import sys

def getArgs():
	"""
	Get arguments from command line.
	"""
	args = ArgumentParser(description="Argumetns for NTupleToNumpy for ANN")
	args.add_argument('-s', '--samples', action='store', required=True, help='Samples name to process .txt splitted by ,')
	args.add_argument('-i', '--inputdir', action='store', default='/eos/user/m/mobelfki/MLPhotonNTuple_FF/', help='Input Directory')
	args.add_argument('-f', '--features', action='store', required=True, help='Features list')
	args.add_argument('-o', '--outdir', action='store', default='output_FF', help='Output Directory')
	args.add_argument('-l', '--toline', action='store', default=False, help= 'apply vector to line')
	return args.parse_args()

class NTupleToNumpy:
	"""
	Produce numpy arrays form NTuple class 
	The output is saved in Array/output directory
	Doesn't take any inputs 
	"""

	def __init__(self, name, line):
		self.name = name;
		self.line = line;
	
	def loadTree(self, path, tree):
		self.Tree = pr.open(path)[tree];
		
	def loadFeatures(self, txt):
	
		self.Features = txt;
		

	def Execute(self):
		
		df = self.Tree.pandas.df(self.Features, flatten=False)
		data = df.to_numpy()
		data = data[:,:]
	
		X1 = []
		X2 = []
		X3 = []
		for i in range(0,data.shape[0]):
			x1 = []
			x2 = []
			x3 = []
			
			for j in range(0,data.shape[1]-1):	
					aa = data[i]
					aa = np.array(aa)
					aa = aa[:-48]
					if j < 3:
						x1.append(aa[j])
					if j < 6 and j >=3:
						x2.append(aa[j])
					if j < 9 and j >=6:
						x3.append(aa[j])

			x1 = np.array(x1)
			X1.append(x1)
			x2 = np.array(x2)
			X2.append(x2)
			x3 = np.array(x3)
			X3.append(x3)

		X1 = np.array(X1)
		X2 = np.array(X2)
		X3 = np.array(X3)

		return X1, X2, X3, data[:,-48:];
		
	def GetOutName(self, sample):
	
		if "_1" in sample:
			return 'Stg_1';
		if "_2" in sample:
			return 'Stg_2';
		if "_3" in sample:
			return 'Stg_3';
		if "_4" in sample:
			return 'Stg_4';
		if "_0" in sample:
			return 'Stg_0';
		return '';
	def GetOutName2(self, sample):

		if "DiJet" in sample:
			return 'DiJet_IsoTight';
		if "GammaJet" in sample:
			return 'GammaJet_IsoTight';
		if "Zlly" in sample:
			if "IsoTight" in sample:
				return 'Zlly_IsoTight';
			if "IsoLoose" in sample:
				return 'Zlly_IsoLoose';
		if "hh_yybb" in sample:
			return "hh_yybb"

def main():
	"""
	The main function of NTupleToNumpy
	"""
	args=getArgs()
	with open(args.samples,'r') as samples_file:
		samples_name = samples_file.read().splitlines();
	samples_file.close();
	features_file = args.features
	ft = []
	with open(features_file,'r') as features_file:
		ft = features_file.read().splitlines();
	features_file.close();	
	print(ft)
	#trees = {'Tree','Tree_Train','Tree_Val','Tree_Test'}
	trees = {'Tree'}
	for tree in trees:
		print(tree)
		i = 0
		for sample in samples_name:
			i = i+1
			NTuple2Numpy = NTupleToNumpy(sample, args.toline);
			NTuple2Numpy.loadTree(args.inputdir+'/'+sample,tree);
			NTuple2Numpy.loadFeatures(ft);
			X1, X2, X3, Y = NTuple2Numpy.Execute();
			name  = NTuple2Numpy.GetOutName(sample);
			name2 = NTuple2Numpy.GetOutName2(sample);
			#print(name)
			np.save(args.outdir+'/'+name+'/'+tree+'/'+name2+'.'+tree+'.'+name+'_'+str(i)+'_X1.npy', X1)
			np.save(args.outdir+'/'+name+'/'+tree+'/'+name2+'.'+tree+'.'+name+'_'+str(i)+'_X2.npy', X2)
			np.save(args.outdir+'/'+name+'/'+tree+'/'+name2+'.'+tree+'.'+name+'_'+str(i)+'_X3.npy', X3)
			np.save(args.outdir+'/'+name+'/'+tree+'/'+name2+'.'+tree+'.'+name+'_'+str(i)+'_Y.npy', Y)
			del NTuple2Numpy
if __name__ == '__main__':
	main()
		
