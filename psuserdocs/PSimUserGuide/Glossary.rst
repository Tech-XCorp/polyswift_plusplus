Glossary
-----------------

domain
   The rectangular Cartesian grid. The physical domain is the grid specified 
   by a user.

FDTD
   Finite-difference time-domain. The FDTD method is a technique for solving 
   problems in electromagnetics.

float
   A floating-point number.

HDF5
   Hierarchical Data Format Version 5. A library and file format, developed 
   by the National Center for
   Supercomputing Applications at the University of Illinois at 
   Urbana-Champaign, for storing graphical and
   numerical data and for transferring that data between computers. 
   PSim outputs data in :ref:`hdf5`.

input block
    An input block is an object consisting of simulation parameters in an 
    input file.  Input blocks can be nested within other input blocks. 
    For example, input blocks for copolymer blocks are nested within the 
    input block for an polymer.

input file
    A PSim simulation file, which has a .pre suffix. Users define a 
    simulation and its variables in an input file.  PSimComposer with then
    automatically run the file through a preprocessor to produce a processed
    input file.  Or, if running on the command line, you must first run the 
    input file through the preprocessor to produce a processed input file.

MPI
   Message Passing Interface. An application programming interface (API) for 
   communicating between processes executing in parallel.

multi-grid pre-conditioner
   A pre-conditioner that enables a solver to use a hierarchy of grids to 
   solve a partial differential equation problem. The multi-grid 
   pre-conditioner applies the results from coarse grids to accelerate the 
   convergence on the finest grid.

parameter
    A parameter is a variable value (integer, floating-point 
    number, or text string) that users define to create a simulation. 

parse
   To divide input into parts and determine the meaning of each part.

physical domain
   See domain. 

processed input file
    A PSim simulation file, which has a .in suffix. PSimComposer
    automatically does this for you.  Users running on the command line
    must produce the processed input file by running an input file 
    through a preprocessor prior to running.

Python
   An open-source, interpreted scripting language managed by the Python 
   Software Foundation.
