#!/usr/bin/env python
"""
@file    pspp-gen.py

@brief   Read parameters, string replaces for PBS template file
         This must be called by a driver script (ps-run.py) 

@version $Id$

Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

# Load modules
import os
import sys
import string
from optparse import OptionParser

# This is to obtain copyfile
import shutil
from shutil import *

def raw_default(prompt, dflt=None):
    if dflt:
        prompt = "%s [%s]: " % (prompt, dflt)
        res = raw_input(prompt)
        if not res and dflt:
            return dflt
        return res

def selectHostName(rawName):

    # Might have to add other search strings
    bfindVal1 = string.find(rawName,"b0")
    bfindVal2 = string.find(rawName,"b1")
    bfindVal3 = string.find(rawName,"b2")

    if ( (bfindVal1 != -1) or
         (bfindVal2 != -1) or
         (bfindVal3 != -1)  ):
        newNameStr = "oic"
    else:
        newNameStr = rawName

    return newNameStr

##################################################

######################################################
# Command line option parse
parser = OptionParser()

parser.add_option("-d","--directory",
                  dest="runDirName",
                  help="Run directory name")

parser.add_option("-i","--inputName",
                  dest="inputName",
                  default="polyswift",
                  help="Input file pre name")

parser.add_option("-b",
                  dest="binDir",
                  help="Scripts bin directory (finds executable")

parser.add_option("-r","--restartstep",
                  dest="restartStep",
                  default=0,
                  help="Index for restart from file")

(options,args) = parser.parse_args()

# Acessing options
runDirName  = str(options.runDirName)
restartStep = str(options.restartStep)
binDir      = str(options.binDir)
inputName   = str(options.inputName)
######################################################

########################################################################################
# Notes:
#   To add variables in the template PBS file,
#   add a variable of the form --> MARKER_'variable-name'

def writeTplFileOIC(binDir):

    t=open('pspp.tpl', 'w')

    print >> t, '#!/bin/bash'
    print >> t, '#'
    print >> t, '#PBS -N MARKER_JOB_NAME'
    print >> t, '#PBS -V'
    print >> t, '#PBS -e ps-MARKER_INPUT_NAME.err'
    print >> t, '#PBS -o ps-MARKER_INPUT_NAME.out'
    print >> t, '#PBS -q MARKER_QUEUE'
    print >> t, '#PBS -l nodes=MARKER_NODES:ppn=MARKER_PPN'
    print >> t, '#PBS -l walltime=24:00:00'
    print >> t, " "
    print >> t, 'PBS_O_WORKDIR=MARKER_PBS_O_WORKDIR'
    print >> t, 'INPUT_NAME=MARKER_INPUT_NAME'
    print >> t, 'PARALLEL_BUILD=' + binDir + '/polyswift'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## RESTORE PARAMETERS'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'let RESTORE_INDEX=MARKER_RESTORE_INDEX'
    print >> t, " "
    print >> t, 'if [ $RESTORE_INDEX = 0 ]; then'
    print >> t, '    echo "Turning restore option OFF"'
    print >> t, '    RESTORE_OPTION='
    print >> t, 'else'
    print >> t, '    echo "Turning restore option ON"'
    print >> t, '    RESTORE_OPTION="-r $RESTORE_INDEX"'
    print >> t, 'fi'
    print >> t, 'echo "restore option = $RESTORE_OPTION"'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## DATE STAMP ON STATS FILE'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'STATS_FILE_PREFIX=stats'
    print >> t, 'YEAR=`date +%Y`'
    print >> t, 'DAYS=`date +%j`'
    print >> t, 'HOURS=`date +%H`'
    print >> t, 'MINUTES=`date +%M`'
    print >> t, 'SECONDS=`date +%S`'
    print >> t, 'STATS_FILE="stats_$YEAR.$DAYS.$HOURS$MINUTES$SECONDS.dat"'
    print >> t, 'echo "Writing standard output to $STATS_FILE"'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## BEGINNING OF EXECUTION'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'cd  $PBS_O_WORKDIR'
    print >> t, 'echo " " > $STATS_FILE'
    print >> t, 'echo This is the workdir $PBS_O_WORKDIR >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo "The master node of this job is " `hostname`     >> $STATS_FILE'
    print >> t, 'echo "The working directory is" `echo $PBS_O_WORKDIR` >> $STATS_FILE'
    print >> t, 'echo "This job runs on the following nodes:"          >> $STATS_FILE'
    print >> t, 'echo "The nodefile is $PBS_NODEFILE"                  >> $STATS_FILE'
    print >> t, 'echo `cat $PBS_NODEFILE`                              >> $STATS_FILE'
    print >> t, "NPROCS=`wc -l ${PBS_NODEFILE} | awk  '{print $1}'`"
    print >> t, 'echo This job has allocated $NPROCS nodes             >> $STATS_FILE'
    print >> t, " "
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, 'echo "Start time : " `date`                        >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "Running Polyswift++                        " >> $STATS_FILE'
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, " "
    print >> t, 'mpirun -np $NPROCS ${PARALLEL_BUILD} ${RESTORE_OPTION} -i ./${INPUT_NAME}.pre >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, 'echo "Finish time : " `date`                       >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    t.close()

def writeTplFileFrost(binDir):

    t=open('pspp.tpl', 'w')

    print >> t, '#!/bin/bash'
    print >> t, '#'
    print >> t, '#PBS -N MARKER_JOB_NAME'
    print >> t, '#PBS -A mat022'
    print >> t, '#PBS -V'
    print >> t, '#PBS -e ps-MARKER_INPUT_NAME.err'
    print >> t, '#PBS -o ps-MARKER_INPUT_NAME.out'
    print >> t, '#PBS -q MARKER_QUEUE'
    print >> t, '#PBS -l nodes=MARKER_NODES:ppn=MARKER_PPN'
    print >> t, '#PBS -l walltime=12:00:00'
    print >> t, '#PBS -l gres=widow2'
    print >> t, " "
    print >> t, 'PBS_O_WORKDIR=MARKER_PBS_O_WORKDIR'
    print >> t, 'INPUT_NAME=MARKER_INPUT_NAME'
    print >> t, 'PARALLEL_BUILD=' + binDir + '/polyswift'
    print >> t, " "
    print >> t, ". /usr/share/Modules/init/bash load python/2.6.4"
    print >> t, "module list"
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## RESTORE PARAMETERS'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'let RESTORE_INDEX=MARKER_RESTORE_INDEX'
    print >> t, " "
    print >> t, 'if [ $RESTORE_INDEX = 0 ]; then'
    print >> t, '    echo "Turning restore option OFF"'
    print >> t, '    RESTORE_OPTION='
    print >> t, 'else'
    print >> t, '    echo "Turning restore option ON"'
    print >> t, '    RESTORE_OPTION="-r $RESTORE_INDEX"'
    print >> t, 'fi'
    print >> t, 'echo "restore option = $RESTORE_OPTION"'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## DATE STAMP ON STATS FILE'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'STATS_FILE_PREFIX=stats'
    print >> t, 'YEAR=`date +%Y`'
    print >> t, 'DAYS=`date +%j`'
    print >> t, 'HOURS=`date +%H`'
    print >> t, 'MINUTES=`date +%M`'
    print >> t, 'SECONDS=`date +%S`'
    print >> t, 'STATS_FILE="stats_$YEAR.$DAYS.$HOURS$MINUTES$SECONDS.dat"'
    print >> t, 'echo "Writing standard output to $STATS_FILE"'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## BEGINNING OF EXECUTION'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'cd  $PBS_O_WORKDIR'
    print >> t, 'echo " " > $STATS_FILE'
    print >> t, 'echo This is the workdir $PBS_O_WORKDIR >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo "The master node of this job is " `hostname`     >> $STATS_FILE'
    print >> t, 'echo "The working directory is" `echo $PBS_O_WORKDIR` >> $STATS_FILE'
    print >> t, 'echo "This job runs on the following nodes:"          >> $STATS_FILE'
    print >> t, 'echo "The nodefile is $PBS_NODEFILE"                  >> $STATS_FILE'
    print >> t, 'echo `cat $PBS_NODEFILE`                              >> $STATS_FILE'
    print >> t, "NPROCS=`wc -l ${PBS_NODEFILE} | awk  '{print $1}'`"
    print >> t, 'echo This job has allocated $NPROCS nodes             >> $STATS_FILE'
    print >> t, " "
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, 'echo "Start time : " `date`                        >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "Running Polyswift++                        " >> $STATS_FILE'
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, " "
    print >> t, 'mpiexec_mpt -np $NPROCS ${PARALLEL_BUILD} ${RESTORE_OPTION} -i ./${INPUT_NAME}.pre >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, 'echo "Finish time : " `date`                       >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    t.close()

def writeTplFile(machineName, binDir):

    t=open('pspp.tpl', 'w')

    print >> t, '#!/bin/bash'
    print >> t, '#'
    print >> t, '#PBS -N MARKER_JOB_NAME'
    print >> t, '#PBS -V'
    print >> t, '#PBS -e ps-MARKER_INPUT_NAME.err'
    print >> t, '#PBS -o ps-MARKER_INPUT_NAME.out'

    if (machineName == 'iter'):
        print >> t, '#PBS -q MARKER_QUEUE'
        print >> t, '#PBS -l nodes=MARKER_NODES:ppn=MARKER_PPN'
        print >> t, '#PBS -l walltime=24:00:00'

    if (machineName == 'carbon'):
        print >> t, '#PBS -q s3opt12'
        print >> t, '#PBS -l nodes=MARKER_NODES:ppn=4'
        print >> t, '#PBS -l walltime=24:00:00'

    if ((machineName == 'oxygen03') or (machineName == 'oxygen02')):
        print >> t, '#PBS -q MARKER_QUEUE'
        print >> t, '#PBS -l nodes=MARKER_NODES:ppn=MARKER_PPN'
        print >> t, '#PBS -l walltime=12:00:00'

    print >> t, " "
    print >> t, " "
    print >> t, 'PBS_O_WORKDIR=MARKER_PBS_O_WORKDIR'
    print >> t, 'RUN_FLAG=MARKER_RUN_FLAG'
    print >> t, 'INPUT_NAME=MARKER_INPUT_NAME'
    print >> t, " "
    print >> t, 'SERIAL_BUILD='   + binDir + '/polyswiftser'
    print >> t, 'PARALLEL_BUILD=' + binDir + '/polyswift'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## RESTORE PARAMETERS'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'let RESTORE_INDEX=MARKER_RESTORE_INDEX'
    print >> t, " "
    print >> t, 'if [ $RESTORE_INDEX = 0 ]; then'
    print >> t, '    echo "Turning restore option OFF"'
    print >> t, '    RESTORE_OPTION='
    print >> t, 'else'
    print >> t, '    echo "Turning restore option ON"'
    print >> t, '    RESTORE_OPTION="-r $RESTORE_INDEX"'
    print >> t, 'fi'
    print >> t, 'echo "restore option = $RESTORE_OPTION"'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## DATE STAMP ON STATS FILE'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'STATS_FILE_PREFIX=stats'
    print >> t, 'YEAR=`date +%Y`'
    print >> t, 'DAYS=`date +%j`'
    print >> t, 'HOURS=`date +%H`'
    print >> t, 'MINUTES=`date +%M`'
    print >> t, 'SECONDS=`date +%S`'
    print >> t, 'STATS_FILE="stats_$YEAR.$DAYS.$HOURS$MINUTES$SECONDS.dat"'
    print >> t, 'echo "Writing standard output to $STATS_FILE"'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    print >> t, '## BEGINNING OF EXECUTION'
    print >> t, '## ------------------------------------------------------------------'
    print >> t, " "
    print >> t, 'cd  $PBS_O_WORKDIR'
    print >> t, 'echo " " > $STATS_FILE'
    print >> t, 'echo This is the workdir $PBS_O_WORKDIR >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo "The master node of this job is " `hostname`     >> $STATS_FILE'
    print >> t, 'echo "The run flag is $RUN_FLAG"                      >> $STATS_FILE'
    print >> t, 'echo "The working directory is" `echo $PBS_O_WORKDIR` >> $STATS_FILE'
    print >> t, 'echo "This job runs on the following nodes:"          >> $STATS_FILE'
    print >> t, 'echo "The nodefile is $PBS_NODEFILE"                  >> $STATS_FILE'
    print >> t, 'echo `cat $PBS_NODEFILE`                              >> $STATS_FILE'
    print >> t, "NPROCS=`wc -l ${PBS_NODEFILE} | awk  '{print $1}'`"
    print >> t, 'echo This job has allocated $NPROCS nodes             >> $STATS_FILE'
    print >> t, " "
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, 'echo "Start time : " `date`                        >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "Running Polyswift++                        " >> $STATS_FILE'
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, " "
    print >> t, 'if [ $RUN_FLAG = "serial" ]; then'
    print >> t, '   ${SERIAL_BUILD} ${RESTORE_OPTION} -i ./${INPUT_NAME}.pre'
    print >> t, 'else'
    print >> t, '  mpirun -machinefile  ${PBS_NODEFILE} -np $NPROCS ${PARALLEL_BUILD}  ${RESTORE_OPTION} -i ./${INPUT_NAME}.pre >> $STATS_FILE'
    print >> t, 'fi'
    print >> t, " "
    print >> t, 'echo " "                                           >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, 'echo "Finish time : " `date`                       >> $STATS_FILE'
    print >> t, 'echo "****************************************** " >> $STATS_FILE'
    print >> t, " "
    print >> t, '## ------------------------------------------------------------------'
    t.close()

########################################################################################

#######################################################################
# Set parameters

skiplines = 1
datalines = 200
inFile    = "pspp.tpl"
outFile   = "pspp.pbs"
# inputName = "polyswift"
accountName = "none"
hostName = os.getenv("HOSTNAME")
hostNameStr = selectHostName(hostName)
userNameStr = os.getenv("LOGNAME")

if (hostNameStr == 'carbon'):
    defaultPPN  = '4'

if ((hostNameStr == 'oxygen03') or (hostNameStr == 'oxygen02')):
    defaultPPN = '8'

if (hostNameStr == 'iter'):
    defaultPPN = '8'

if (hostNameStr == 'oic'):
#    defaultPPN = '8'
    defaultPPN = '6'


if ( (hostNameStr == 'frost2') or (hostNameStr == 'frost3') or
     (hostNameStr == 'frost4') or (hostNameStr == 'frost5') ):

    defaultPPN = '16'
    if (userNameStr == "swsides"):
        accountName = "MAT022"
        print "Using Frost account: ", accountName
    elif (userNameStr == "rkr"):
        accountName = "STF006"
        print "Using Frost account for rkr: ", accountName
    else:
        accountName = "default"
        print "Frost account not recognized"

runFlag      =     raw_default("Run type:[serial/parallel]: ", 'parallel')
restartIndex = int(raw_default("File index (0 no restart):  ", restartStep))
numNodes     = int(raw_default("Number of nodes:            ", '2'))
ppnumNodes   = int(raw_default("Number of procs/nodes:      ", defaultPPN))

numProcs = ppnumNodes * numNodes
queueName = 'defaults set by pspp-gen.py'

if ((hostNameStr == 'oxygen03') or (hostNameStr == 'oxygen02')):
    if (numNodes <= 2):
        queueName = 'small'
    if ( (numNodes > 2) and (numNodes <= 8) ):
        queueName = 'medium'
    if ( (numNodes > 8) and (numNodes <= 16) ):
        queueName = 'high'

if (hostNameStr == 'iter'):
    queueName = 'batch'

if ( (hostNameStr == 'frost2') or
     (hostNameStr == 'frost3') or
     (hostNameStr == 'frost4') or
     (hostNameStr == 'frost5') ):
    #    queueName = 'computation'
    queueName = 'batch'

if (hostNameStr == 'oic'):
    if (numNodes <= 8):
        if (userNameStr == 'rkr'):
            queueName = 'cnms10fq'
        else:
            queueName = 'cnms08tq'
    else:
        if (userNameStr == 'rkr'):
#            queueName = 'cnmsq'
            queueName = 'cnms10fq'
        else:
            queueName = 'cnms08fq'

print " "
print "Run type entered =          " , runFlag
print "File index (0 = no restart) " , restartIndex
print "Number of nodes =           " , numNodes
print "Procs/node =                " , ppnumNodes
print "Queue name =                " , queueName
print "Total procs =               " , numProcs
print "Input file prefix =         " , inputName
print " "
#######################################################################

###########################################################################
# Write out template PBS file and substitute MARKER values

print " "
print "Using queue defaults for ", hostNameStr
print " "

if (hostNameStr == 'oic'):
    writeTplFileOIC(binDir)

elif ( (hostNameStr == 'frost2') or
       (hostNameStr == 'frost3') or
       (hostNameStr == 'frost4') or
       (hostNameStr == 'frost5') ):
    writeTplFileFrost(binDir)

else:
    writeTplFile(hostNameStr, binDir)

f=open(inFile,  'r+')
g=open(outFile, 'w')

for x in range(1,skiplines):
    strline = f.readline()
    g.write(strline)

for x in range(1,datalines):
    strline = f.readline()
    strline = strline.replace('MARKER_ACCOUNT',       accountName )
    strline = strline.replace('MARKER_JOB_NAME',      runDirName[-15:] )
    strline = strline.replace('MARKER_PBS_O_WORKDIR', os.getcwd() )
    strline = strline.replace('MARKER_RESTORE_INDEX', str(restartIndex) )
    strline = strline.replace('MARKER_RUN_FLAG',      runFlag )
    strline = strline.replace('MARKER_INPUT_NAME',    inputName )
    strline = strline.replace('MARKER_QUEUE',         queueName )
    strline = strline.replace('MARKER_NODES',         str(numNodes) )
    strline = strline.replace('MARKER_PPN',           str(ppnumNodes) )
    strline = strline.replace('MARKER_PROCS',         str(numProcs) )
    g.write(strline)

f.close()
g.close()
###########################################################################

print "*****************************"
print "    pspp-gen.py all done     "
print "*****************************"
