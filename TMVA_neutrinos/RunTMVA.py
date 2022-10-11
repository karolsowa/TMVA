import os, sys, subprocess

def runMacro(macroName, splash=False, interprete=False, batch=True):          
    shellCommand = ["root"]                                                                 
    if interprete is False:                                                                 
        shellCommand.append("-q")                                                           
    if splash is False:
        shellCommand.append("-l")
    if batch is True:
        shellCommand.append("-b")                                                            
    shellCommand.append(macroName)                                    
    print("Run Macro", shellCommand)                                                        
    a = subprocess.run(shellCommand, stdout = subprocess.PIPE)     
    return a

def main():
    rc = 0
    while(rc == 0):
        proc = runMacro("/home/student/Summer_School_IFJ/Analysis  T2K/TMVA_neutrinos.C")
        data = proc.stdout
        rc = proc.returncode
        print(f"Rc:{rc}")
        print("[INFO]:: End of script execution")
    
if __name__ == "__main__":
    main()