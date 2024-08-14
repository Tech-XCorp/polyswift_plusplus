.. _simplespecfilter:

Spectral Filter
------------------------------

:command:`simpleSpecFilter`:
    kind of :command:`Updater` uses spectral filter technique to aid relaxing 
    chemical potential fields towards an ordered phase segregated state.

    
simpleSpecFilter Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`cutoffFactor` (float):
    cutoff factor for determining filter strength [0.0 -- <1.0]

:option:`updatefields` (string vector):
    names of PhysFields that will be filtered


    
Example simpleSpecFilter Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <Updater specFilter>

      kind = simpleSpecFilter
      applyFrequency = 100
      applyStart = 200
      updatefields = [totStyrDens totEthyDens]
      cutoffFactor = 0.60

    </Updater>


See also
~~~~~~~~~~
    - :ref:`updater`
    - :ref:`physfields`
