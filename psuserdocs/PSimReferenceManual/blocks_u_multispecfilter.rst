.. _multispecfilter:

Multiple-value Spectral Filter
------------------------------

:command:`multiSpecFilter`:
    kind of :command:`Updater` uses spectral filter technique to aid relaxing 
    chemical potential fields towards an ordered phase segregated state.

    
multiSpecFilter Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`cutoffFactor` (float):
    cutoff factor for determining filter strength [0.0 -- <1.0]

:option:`updatefields` (string vector):
    names of PhysFields that will be filtered

:option:`specCellSizes` (int vector):
    number of cells in [x,y,z] direction defining a filtering region

    
Example multiSpecFilter Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <Updater specFilter>

      kind = multiSpecFilter
      applyFrequency = 100
      applyStart = 200
      updatefields = [totStyrDens totEthyDens]
      cutoffFactor = 0.60
      specCellSizes = [4 4 1]

    </Updater>

See also
~~~~~~~~~~
    - :ref:`updater`
    - :ref:`physfields`
