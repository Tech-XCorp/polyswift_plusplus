.. _freeenergy:

Bulk Free-Energy
------------------------------

:command:`freeEnergy`:
    kind of :command:`History` that records the excess free-energy for a
    copolymer mixture



freeEnergy Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`updaterName` (string):
    string name for Updater block needed to calculate the free-energy
   
    
Example freeEnergy Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

   <History freeE1>
     kind = freeEnergy
     updatePeriodicity = 10
     updaterName = wAwB
   </History>


See also
~~~~~~~~~~
    - :ref:`history`
