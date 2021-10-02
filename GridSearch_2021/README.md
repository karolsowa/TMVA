Welcome to BDT grid-search repository!

--------------------------------Programs included in this repo use root TMVA toolkit:--------------------------------------------

00000000000000000       00             00       0                 0              0
		0			  	# #           # #		 #		         #		        # #
		0			  	#  #         #  #		  #		        #		       #   #
		0             	#   #       #   #		   #	       #		      #     #
		0				#    #     #    #			#	      #			     #       #
		0				#     #   #     #			 #	     #		        #         #
		0				#      # #      #			  #	    #			   ############# 
		0				#		#       #			   #   #			  #             #
		0				#               #				# #				 #               #
		0				#               #				 #		        #                 #

---------------------------------------------------------------------------------------------------------------------------------

----------------------Section 1----------------------------

				   No parallelisation


	1) BDT_classifier.C - regular BDT classifier for non-parallel calculations

		Running:
		- Set signal, bkg and input file paths and add trees to TChains at the beginning of the file 
		  (Part of the code you need to change is separated from the rest by dashed lines)
		
		Output:
		- Program returns AUC value for given set of parameters
		- 'outfileName' - a file with classifier performance results, a GUI is made out of it
		- 'postBDT_file' - a file with NTUPLE including copied input tree and new branch 'BDT_response'
		- GUI drawn from file 'outfileName' (you can comment the line 283 to turn it off)

	2) BDT_classifier_fast.C - exactly the same classifier but without 'reader' part. 
	   Just calculates the AUC, no 'postBDT_file' with 'BDT_response' branch

	   Running:
	   - See point 1

	   Output:
	   - Program returns AUC value for given set of parameters
	   - 'outfileName' - a file with classifier performance results (you can uncomment line 218 to make a GUI out of it)


--------------------------------------------------------------

---------------------Section 2--------------------------------

					Parallelisation


	1) Cascade1.C - BDT classifier with some adjustments enabling parallelisation
	2) Gridsearch.C - ROOT macro which allows you to set step value for each grid-search variable. 
	   It is also responsible for writing results to output files
	3) Gridsearch.h - a header file with some variables and class declarations
	4) Gridsearch.py - a python script which runs ROOT macros in parallel (one thread for each AdaBoostBeta value)

	Running:

	(Cascade1.C)
	- See: point 1 in section 1
	- By default all notifications appearing during programme execution are redirected from screen to separate files.
	- Remember that output files and weights directories' names should differ for each thread to avoid conflicts! (Set them)
	(Gridsearch.C)
	- Each macro argument corresponds to a step value for given parameter - set them at the beginning
	- Set the outfile files path - remember that output file should be different for each thread!
	(Gridsearch.h)
	- Set initial values for each grid-search variable
	(Gridsearch.py)
	- Set path to Gridsearch.C macro in line 47
	- To start grid-search type 'python Gridsearch.py' in your terminal



--------------------------------------------------------------

---------------------Section 3--------------------------------
	
				  Additional files

	1) MaxAUC.C - ROOT macro which finds maximum AUC value in output file from grid-search. 

	   Running:
	   - The only argument of this macro corresponds to AdaBoostBeta value included (by default) in each 
	   	 output file name
	   - Set path to all output files
	   - 

	2) MaxAUC.h - a header file with some variables and class declarations
	3) RunMaxAUC.C - ROOT macro which runs MaxAUC.C for each AdaBoostValue and therefore finds maximum AUC value
					 and the best set of parameters

	   Running:
	   - Boolean argument: 0 - finds and prints the best set of parameters
	                       1 - finds and prints the best set of parameters, then runs the classifier for optimal parameters
	4) Optimize.C - macro which finds optimal parameters values using HPO method 

	   Running:
	   - See point 1 in section 1
	   - Set number of folds (HPO is based on sth similar to k-fold validation) - you'll get recommended parameters values for each fold

	5) Optimize2.C - macro which finds optimal parameters values using OTP method

	   Running:
	   - See point 1 in section 1

	6) ClassifierCompare.C - custom drawing macro comparing plots for BDT cut values obtained for grid-search, HPO and OTP (in preparation)
	7) Signal.C - see: point 6 (in preparation)

					

--------------------------------------------------------------


