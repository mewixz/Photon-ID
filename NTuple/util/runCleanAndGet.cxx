#include "CleanAndGet/CleanAndGet.h"

int main(int argc, char* argv []){

	if(argc == 1)
	{
		std::cout<<" Input Directory not provided"<<std::endl;	
		return 0;
	}
	const char* APP_NAME = argv[ 0 ];
	CHECK( xAOD::Init( APP_NAME ) ); 
	if(argc<5)
	{
		std::cout<<"Error, Arugements "<<std::endl;
	}
	
	TString input = argv[1];
	TString output = argv[2];
	Int_t str = std::stoi(argv[3]);
	TString name = argv[6];
	
	Int_t i,n;

	if(argc <= 4) 
	{
		i = 0;
		n = 0;
	}else if( argc <= 5) 
	{
		i = std::stoi(argv[4]);
		n = 0;
	}else {
		i = std::stoi(argv[4]);
		n = std::stoi(argv[5]);
	}
	
	
	CleanAndGet* obj = new CleanAndGet(input,output,str,i,n,name);
	if(!obj->initilize()) return 0;
	if(!obj->runCleanAndGet()) return 0;
	if(!obj->finilize()) return 0;
	return 1;
}
