Welcome to BDT grid-search repository!

Programs included in this repo use root TMVA toolkit:

#################     	##             ##		#                 #        		 #
		#			  	# #           # #		 #		         #		        # #
		#			  	#  #         #  #		  #		        #		       #   #
		#             	#   #       #   #		   #	       #		      #     #
		#				#    #     #    #			#	      #			     #       #
		#				#     #   #     #			 #	     #		        #         #
		#				#      # #      #			  #	    #			   ############# 
		#				#		#       #			   #   #			  #             #
		#				#               #				# #				 #               #
		#				#               #				 #		        #                 #


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


--------------------------------------------------------------

---------------------Section 3--------------------------------

					Additional files

--------------------------------------------------------------


