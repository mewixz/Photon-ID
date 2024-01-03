# NTuple 
- A RootCore based framework to apply selection and strategies (0, 1, 2 and 3):

  - 0 : Healthy (Completed) cluster and $|\eta|<1.8$; 
  - 1 : Healthy (Completed) cluster and $|\eta|<2.5$; 
  - 2 : UnHealthy (UnCompleted) cluster and $|\eta|<2.5$; 
  - 3 : UnHealthy (UnCompleted) cluster and $|\eta|<1.8$;

# Setup 
On the first time running:
    setupATLAS;
    lsetup 'rcsetup Base,2.5.1';
    rc find_packages;
    rc compile;
Every logging:
    source rcSetup.sh
# Run 
    runCleanAndGet Input_Dir Ouput_Dir Strategy Evt_first Evt_last Selection

- The code takes the output from [ZllAthDerivation](https://github.com/mobelfki/CNN-Photon-ID/tree/main/ZllyAthDerivation)
    
