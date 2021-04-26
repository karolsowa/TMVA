import os, sys, subprocess

# root macro run format
# root -l "macro.C+(arg1,arg2,arg3,...,argn)"

# run: python Run_Selection.py 2015
def argConversion(arg):                             
    if type(arg) in [float, int]:                   
        return str(arg)                             
    elif type(arg) in [str]:
        return '"' + arg + '"'
    else:
        return '"' + str(arg) + '"'

def argConnection(arglist=[]):                              
    if not (arglist is [] or arglist is None):              
        shellargv = list(arglist)                            
        arglist_conv = list()                               
        for arg in arglist:
            arglist_conv.append(argConversion(arg))
        return '('+','.join(arglist_conv)+')'               
    else:
        return ''

def runMacro(macroName, arglist=None, splash=False, interprete=False, batch=True):          
    shellCommand = ["root"]                                                                 
    if interprete is False:                                                                 
        shellCommand.append("-q")                                                           
    if splash is False:
        shellCommand.append("-l")
    if batch is True:
        shellCommand.append("-b")                                                            
    shellCommand.append(macroName+argConnection(arglist))                                    
    print("Run Macro", shellCommand)                                                        
    a = subprocess.Popen(shellCommand)                                                      
    return a

def main():
    AdaBoostStep = 0.05
    AdaBoostList = [0.01]
    i = 0
    while AdaBoostList[i] < 0.6 - AdaBoostStep:
        a = AdaBoostList[i] + AdaBoostStep
        AdaBoostList.append(round(a,2))
        i += 1
    for Ada  in AdaBoostList:
        a = runMacro("/home/sowrol/repo/GridSearch_2021/GridSearch.C", arglist = [Ada])

    print("[INFO]:: End of script execution")

if __name__ == "__main__":
    main()
