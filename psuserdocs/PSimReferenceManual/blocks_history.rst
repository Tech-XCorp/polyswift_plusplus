.. _history:

History
---------------------

:command:`History`:

    top-level block for recording data from a PSim simulation. You can 
    use History blocks to calculate and record data about a simulation. 
    
    Throughout a simulation run, the PolySwift++ engine writes the History data 
    to an HDF5 file (that is separate from the HDF5 files that contain
    raw field and particle data). The History data file has the file name 
    structure 'runname'_History.h5.
    
    When a simulation is started or restarted, the PolySwift++ engine looks 
    for an existing History file. If a History file is found, new data is 
    appended to the contents of that existing file.

    If the PolySwift++ engine does not find a History file, a new file is 
    created. Generally, histories collect data at the end of each time step, 
    after all other blocks have been updated.


History Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`freeEnergy`:
        Excess free-energy in a copolymer mixture

    :option:`floryConstChi`:
        for a simulation w/spatially constant chi, records chi value
	due to presence of an STFunc that could be changing the value at
	each update step

:option:`updatePeriodicity` (int):
    number of update steps between recording diagnostic value

:option:`tstepBeforeStart` (int):
    number of update steps before start of updating history

:option:`tstepBeforeFinish` (int):
    number of update steps before history stops recording

        
See also
~~~~~~~~~~~~

    - :ref:`effhamil`
    - :ref:`interaction`
