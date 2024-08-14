.. _hdf5:

HDF5 Format PSim Output Files
-------------------------------

Hierarchical Data Format Version 5 (HDF5) is a library and file format
for storing graphical and numerical data and for transferring that data
between computers. PSim outputs data in HDF5 format.

The Hierarchical Data Format was developed by the National Center for
Supercomputing Applications at the University of Illinois at
Urbana-Champaign. For more information about HDF5, please see the web
page at: `http://hdfgroup.org/HDF5 <http://hdfgroup.org/HDF5>`_

HDF5 Files
^^^^^^^^^^^^

PSim outputs data in HDF5 format files that have :samp:`.h5` suffixes.

PSim produces one HDF5 file for each field or species at each dump
time. For example, if the simulation parameter :samp:`nsteps = 100` and the
simulation parameter :samp:`dumpPeriodicity = 10`, PSim dumps data 10 times
during the simulation and outputs a total of 10 HDF5 files for each
field or species while running the simulation.

For more information about HDF5, see:
`http://hdfgroup.org/HDF5 <http://hdfgroup.org/HDF5>`_


Change the Names of Output Files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you want to change the names of the output files, which include the
:samp:`.h5` files, you can specify the :samp:`-o` output option when you run PSim.

For example, you want to replace :file:`linearDiblock` with :file:`linearDiblockTest1` in the
names of the :file:`linearDiblock` simulation’s output files. Run serial PSim from the
command line using this command:

::

    polyswiftser -i linearDiblock.in -o linearDiblockTest1




Display the Content of .h5 Files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :program:`h5dump` utility converts the binary data in :samp:`.h5` files into
human-readable ASCII data in :samp:`.txt` files.

The h5dump utility is available for all the platforms on which PSim
runs. You can download the utility from:`http://hdfgroup.org/HDF5 <http://hdfgroup.org/HDF5>`_
The basic command is:

::

    h5dump -o output_file_name.txt your_h5_file.h5


To convert the file 'diblock_totStyrDens_1000100.h5' to ASCII text format, use this
command:

::

    h5dump -o diblock_totStyrDens_1000100.txt diblock_totStyrDens_1000100.h5


For information about HDF5 and the :program:`h5dump` utility, see the *HDF5 User’s
Guide* and *HDF5 Reference Manual*, which are available at:
`http://www.hdfgroup.org/HDF5/doc/index.html <http://www.hdfgroup.org/HDF5/doc/index.html>`_
