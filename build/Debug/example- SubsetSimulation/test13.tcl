
#reliability

randomVariable 1 normal  5000.0   [expr 0.05*5000.0] 
# randomVariable 1 normal  5000.0   [expr 0.05*5000.0]  5000.0
# randomVariable 2 lognormal  355.0   [expr 0.05*355.0]  355.0
# randomVariable 3 lognormal  337500.0   [expr 0.05*337500.0] 337500.0

# randomVariablePositioner 1 -createRV3 lognormal $E  [expr 0.05*$E]   -element 1     -material E  
# randomVariablePositioner 2 -createRV3 lognormal $fy [expr 0.05*$fy]  -element 1     -material sigmaY  
# randomVariablePositioner 3 -createRV3 lognormal $P  [expr 0.05*$P]   -loadPattern 1 -loadAtNode 2  1

performanceFunction 1 "0.13-{u_2_1}"

probabilityTransformation    Nataf            -print 0
randomNumberGenerator CStdLib
# reliabilityConvergenceCheck  Standard         -e1 1.0e-3    -e2 1.0e-3  -print 1
gFunEvaluator                OpenSees Timeinvariant         -file test12.tcl

# #gradGEvaluator               OpenSees
# gradGEvaluator               FiniteDifference -pert 1000
# searchDirection              iHLRF
# meritFunctionCheck           AdkZhang         -multi 2.0    -add 10.0   -factor 0.5  
# stepSizeRule                 Armijo           -maxNum 50    -base 0.5   -initial 1.0 2  -print 1 -sphere 1000.0 1.0 1.0
# startPoint                   Mean
# findDesignPoint              StepSearch       -maxNumIter 100 -printDesignPointX designPointX.out

#runFORMAnalysis FORMoutput.out
#runMonteCarloResponseAnalysis  -outPutFile  m.out -maxNum 10000 -print 1
#runSubsetSimulationAnalysis  -outPutFile  m.out -SamplingType HMC -NumSeedSamples 1000 -print 1
runSubsetSimulationAnalysis  -outPutFile  m.out -SamplingType MH 2 -NumSeedSamples 5000 -print 1