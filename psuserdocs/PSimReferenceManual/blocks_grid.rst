.. _grid:

Grid
---------------------

:command:`Grid`:

    defines the properties of simulation grid.


Grid Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    specifies the type of grid to use; currently only one option:
    
    :option:`uniCartGrid`:
        uniform cartesian simulation grid


:option:`cellSizes` (float vector):
    scaled size of grid cell size in each direction

:option:`numCellsGlobal` (int vector):
    number of cells in each direction

:option:`decomp` (string):
    name of a Decomp object used decompose grid in multi-processor simulations


        
See also
~~~~~~~~~~~~

    - :ref:`decomp` 
