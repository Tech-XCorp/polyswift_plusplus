.. _poisson:

Poisson-Boltzmann Updater
------------------------------

:command:`poissonUpdater`:
    kind of :command:`Updater` uses spectral methods to solve a Poisson equation
    for a bulk system. Dielectric is assumed homogeneous

    
poissonUpdater Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`bjerrumLen` (float):
    Bjerrum length factor scaled for polyelectrolyte models (see References 
    for details)

:option:`updatefields` (string):
    names of charge density PhysFields that will be used in Poisson solve.

    
Example simpleSpecFilter Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

  <Updater ePotential>

    kind = poissonUpdater
    bjerrumLen = 0.001
    updatefields = [ totChargeDens ]

  </Updater>


See also
~~~~~~~~~~
    - :ref:`updater`
    - :ref:`physfields`
    - :ref:`chargedens`
