
##  TMVA_neutrinos.C
	Macro performing classification in a loop, each time for a slightly different set of parameters. It can be stopped at any time. It automatically starts the calculations from the last set of parameters stored in the AUC.txt file, so when you want to continue the grid search, you can run it again and again, without any changes in code or any additional parameters. 

##  TMVA_header.h
	Header file for TMVA_neutrinos.C

##  RunTMVA.C
	Macro which runs TMVA_neutrinos.C in loop until something is wrong or the loop inside TMVA_neutrinos.C is over

##  TMCA_neutrinos.CA - needs examination!!!
	Classification Application macro - no loop, but full classifying process (including TMVA Reader) for the input dataset

##  MaxAUC_neutrinos.C
	Macro which finds the set of parameters that ended up with the highest value of AUC. It takes the name of the file where results are stored as a parameter.

##  MaxAUC_neutrinos.h
	Header file for MaxAUC_neutrinos.C

##  RunMaxAUC_neutrinos.C - unfinished!!!
	Runs the classification application macro for the best set of parameters found by automatically invoked MaxAUC.C

##  NewTrees.C
	Macro creating two trees - SignalTree and BackgroundTree out of one input tree

