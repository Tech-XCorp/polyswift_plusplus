.. _updater:

Updater
---------------------

:command:`Updater`:

    blocks define the properties of general updaters for PhysFields in the
    simulation. One or more of these updaters are held by the EffHamil class
    container. The Updater block has access to PhysField and Interaction blocks
    needed to update a list of PhysField's at each iteration step.


Updater Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`steepestDescent`:
        Mean-field steepest descent algorithm (with noise) for advancing 
	the chemical potential fields

    :option:`simpleSpecFilter`:
	spectral filter algorithm with one global filtering value

    :option:`multiSpecFilter`:
	spectral filter algorithm with multiple filtering values

    :option:`poissonUpdater`:
        Poisson equation solver

:option:`updateFields` (string vector):
    object names for PhysFields needed for update

:option:`applyFrequency` (integer):
    number of update steps between application of updater

:option:`applyStart` (integer):
    number of update steps before first application of updater

:option:`applyEnd` (integer):
    number of update steps before turning off updater

        
See also
~~~~~~~~~~~~

    - :ref:`physfields`
    - :ref:`effhamil` 
