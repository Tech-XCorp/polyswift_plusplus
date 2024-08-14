.. _programming-concepts-execution-and-data-generation:

Output Files
----------------------------------

All data produced by the PolySwift++ engine is output using the 
HDF5 :samp:`.h5` 
data file format. These output files are dumped as defined by the
user; they can be written at the end of a simulation, for example, or
every n steps to create time series that can be used to see how a system
evolves over time. Additionally, these output files can be used to
restart a run and continue from a given point; for example, if a user
has run a simulation for 1000 time steps and wishes to see how the
simulation progresses if run for another 1000.

For more information on the HDF5 format of PSim output files, please see
:ref:`hdf5`
