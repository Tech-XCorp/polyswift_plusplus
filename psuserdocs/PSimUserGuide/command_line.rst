.. _user-guide-running-polyswift-from-the-command-line:

Running the PolySwift++ engine from the Command Line
--------------------------------------------------------

There are 2 main ways to run PolySwift++ from the command line.

      - A python script 'ps-run.py' is included in the PSim
        distribution (as well as engine-only distributions). If installed
        properly and the PATH environment variable is set correctly, 
 	'ps-run.py -h' should provide help.

      - Running the PolySwift++ executable directly.
        Both the serial and parallel PolySwift++ executables require you to 
        specify the input file (:samp:`.pre`) as well as any optional arguments.


The ps-run.py PolySwift++ run script
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:command:`ps-run.py`:

	A Python script that helps setup and run PolySwift++ in separate
  	directories. :samp:`ps-run.py -h` gives the usage message for the 
	script.  Running the command:

	:samp:`ps-run.py -d new-ps-rundir-1 -i diblock.pre`

	
	will create a new directory 'new-ps-rundir=1', copy the input file
	'diblock.pre' to this new directory and setup the run. If the hostname
	is recognized and an entry exists for the local queueing system, ps-run.py
	will create the correct PBS script and submit. If an entry does not exist
	ps-run.py will attempt to run the job locally in the background of the compute
	machine from which you are submitting the run. If you would like to have
	a PBS entry added contact Tech-X. The ps-run.py script will prompt the user
	for the information needed to setup the simulation.



Running the PolySwift++ Executable
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Your PSim distribution package contains two executable programs for
running the PolySwift++ engine, one for serial computations 
(:samp:`polyswiftser`) and another for parallel
computations (:samp:`polyswift`). Both versions
of the PolySwift++ executables are located in 
the :samp:`POLYSWIFT_BIN_DIR` directory.


.. _user-guide-running-polyswift-from-the-command-line-command-line-features:

Command Line Features
~~~~~~~~~~~~~~~~~~~~~~

If PolySwift++ is run from the command line, input file and runtime options
are specified as command line options. PSimComposer sets up
your environment prior to running PolySwift++, and therefore you must set up 
your environment when running PolySwift++ from the command line.


.. _programming-concepts-symbol-definition-on-the-command-line:

Symbol Definition on the Command Line
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

txpp allows symbols to be defined on the command line. These definitions
override any symbol definitions in the outer-most (global) scope. This
allows you to set a default value inside an input file that can then be
overridden on the command line if needed.

For example, if the following is in the outermost scope of the input
file (outside of any blocks or macros):

::

        $ X = 3
        X
    
    
Then this will result in a line containing :samp:`3` in the output. However, if
you were to invoke txpp via:

::

       txpp.py DX=4
    
    
then this will result in a line :samp:`4`.

However, if you were to define :samp:`X` inside a block (not in the global
scope), such as:

::

        <block foo>
            $ X = 3
            X
        </block>
    
    
then :samp:`X` will always be :samp:`3`, no matter what value for :samp:`X` is specified
on the command line.


.. _user-guide-running-polyswift-from-the-command-line-order-of-precedence:

Order of Parameter Precedence
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If a parameter is both set within the input file and specified on the
command line, the command line parameter value takes precedence. The
command line override enables you to configure an input file with
default values while exploring alternative parameter settings from the
command line. From the command line, you can quickly change simulation
grid sizes, dump periodicity, random seed, etc.

.. _user-guide-running-polyswift-from-the-command-line-example:


Examples of Running PolySwift++ from the Command Line
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In these examples, it is assumed that you are either in the directory
in which the :program:`polyswiftser` is installed or you have added the
appropriate directory to your PATH environment variable.


.. _user-guide-running-polyswift-from-the-command-line-command-line-options:

Command Line Options
~~~~~~~~~~~~~~~~~~~~~~

To use multiple options, the command line syntax is:

::

   ./polyswiftser -i filename [-o prefix_name] [-r num] [-n num]

in which :samp:`./polyswiftser` is used to run a serial computation.

Commonly used options that you can specify on the command line include:

:option:`-i filename`
    Read input from file named *filename*.
        
    For example:

::
    
        ./polyswiftser -i diblock.pre


:option:`-o prefix_name`
    Base names of output files on the text string *prefix_name*.
         
    For example, if you want output files named :samp:`diblockTest1` rather than :samp:`diblock`, use:

::

    ./polyswiftser -i diblock.pre -o diblockTest1


:option:`-n num`
    Run the simulation for *num* time steps. This option overrides the :option:`nsteps` parameter. 
    
    For example, if you want to run :samp:`diblock` with 50 time steps rather than 10, use:
    
::

   ./polyswiftser -i simpleEs.in -n 50



:option:`-d num`
    Dump data every *num* time steps. This option overrides the :option:`dumpPeriodicity` parameter. 
    
    For example, to run :samp:`diblock` and dump output after every 5 time steps, use:

::

   ./polyswiftser -i diblock.pre -d 5


:option:`-r num`
    Restart PolySwift++ from dump *num*. 
    
    For example, if you want to restart diblock using the output dumped at time step 50, use:

::

   ./polyswiftser -i diblock.pre -r 50



.. _user-guide-running-polyswift-from-the-command-line-serial-computation:

Serial Computation
^^^^^^^^^^^^^^^^^^^^^^^^^

The PolySwift++ executable for use in serial computation is named
:program:`polyswiftser`.  Except as noted, the explanations and tutorials within 
the :ref:`PSim In Depth <psim-in-depth>` 
and :ref:`PSim Quick Start <psim-quick-start>` manuals
demonstrate PolySwift++ usage for serial computations.
Here is an example of PolySwift++ command line invocation using an input file
named :file:`myfile.in` and specifying 1000 time steps, outputting the result
data (dumping) every thousand steps. By default, the output files for
this example would be named using the format :file:`myfile.out`.

::

    polyswiftser -i myfile.pre -n 1000 -d 1000


.. note:: 
   The above invocation line assumes you have executable in your PATH.

.. note::
   When running PolySwift++ via PSimComposer, command line options
   are not directly available, however :samp:`-i` and :samp:`-o` command line options
   described in this document are implicit; that is, these options are
   automatically invoked when running PSimComposer.


.. _user-guide-running-polyswift-from-the-command-line-running-parallel:


Parallel Computation
^^^^^^^^^^^^^^^^^^^^^^^

The PolySwift++ executable for use in parallel computation is named
:program:`polyswift`. This section explains
use of the PolySwift++ executable program for parallel computations. 
PolySwift++ for parallel computations requires the Message Passing Interface
(MPI).


.. _user-guide-running-polyswift-from-the-command-line-running-polyswift-with-parallel-queuing:

Running PolySwift++ with Parallel Queuing Systems
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Parallel queuing systems, such as LoadLeveler and PBS, require the
submission of a shell script with embedded comments that the systems
interpret. Here is an example of a basic shell script for a PBS-based
system:

::

    #PBS -N NDS_polyswift
    #PBS -l nodes=2:ppn=2
    cd /directory/containing/your/input/file
    mpiexec -np 4 polyswift -i diblock.in -n 250 -d 50

The run script ps-run.py invokes another script (pspp-gen.py, that is also included)
that will generate a PBS run script and submit to the queuing system. The specific
PBS file settings can be included in the pspp-gen.py script by contacting Tech-X.
