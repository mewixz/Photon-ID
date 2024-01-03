/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

//
// MCLumi: class to return filter efficiency and xsection for MC samples
//         starting from information stored in txt file via getAMI5xsec.pl                                                                                             
//         script (+ eventual k-factors)
//
// Usage: .L MCLumi.C+ (or link your code against MCLumi)
//        MClumi foo("mc11a_lumi_p756.txt");
//        unsigned int mc_channel_number = 116870;
//        double gen_xsec = foo.getXsec(sample);
//        double filter_eff = foo.getFEff(sample);
//        ...
//                                                                                                                                 
 
#include "MyAnalysis/MCLumi.h"
#include "PathResolver/PathResolver.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>

MCLumi::MCLumi(std::string filename)
{
    // read samples and corresponding infos from file, and fill internal structures
    //std::string filename = "MyAnalysis/Files/samples.txt";
    std::ifstream infile; 
    infile.open(filename.c_str()); 
    //process file
    while( !infile.eof() )
    {
        char buf[500];
	infile.getline(buf,500,'\n');
	if (buf[0]=='#')
	{
	    //cout << "skipping line " << buf << endl;
	    continue;
	}
	std::stringstream str(buf);

	string septoken;	
	string sample;
	int mc_channel_number;
	long nevts;
	double xsec;
	double feff;
	double lumi;
	
	str >> sample >> septoken
	    >> mc_channel_number >> septoken
	    >> nevts >> septoken
	    >> xsec >> septoken
	    >> feff >> septoken 
	    >> lumi >> septoken;
	
	//cout << sample << " " << nevts << endl;

	if( !infile.good()  ) break; 
	if ( sample[0] == '#' ) continue;
	samples.push_back(mc_channel_number);
	effs.push_back(std::make_pair(mc_channel_number,feff));
	xsecs.push_back(std::make_pair(mc_channel_number,xsec));
    }
    //listSamples();
}

void MCLumi::listSamples()
{
    for (unsigned int i=0; i<samples.size(); i++)
	cout << samples[i] << endl;
}

bool MCLumi::containsSample(unsigned int mc_channel_number)
{
    for (unsigned int i=0; i<samples.size(); i++)
    {
	if (samples[i] == mc_channel_number) return true;
    }
    return false;
}

double MCLumi::getXsec(unsigned int mc_channel_number)
{
    for (unsigned int i=0; i<samples.size(); i++)
    {
	if (xsecs[i].first == mc_channel_number) return xsecs[i].second;
    }
    std::cout<<"MCLumi::getXsec : sample " << mc_channel_number << " not found, returning 0.0" << std::endl;
    return 0.0;
}

double MCLumi::getFEff(unsigned int mc_channel_number)
{
    for (unsigned int i=0; i<samples.size(); i++)
    {
	if (effs[i].first == mc_channel_number) return effs[i].second;
    }
   // std::cout<<"MCLumi::getFEff : sample " << mc_channel_number << " not found, returning 0.0" << std::endl;
    return 0.0;
}
