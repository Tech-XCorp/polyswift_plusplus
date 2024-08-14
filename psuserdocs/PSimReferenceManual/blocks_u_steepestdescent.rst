.. _steepestdescent:

Steepest-descent updating of Chemical Potential Fields
------------------------------

:command:`steepestDescent`:
    kind of :command:`Updater` that updates the chemical potential fields for 
    a mean-field polymer SCFT model

    
steepestDescent Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`relaxlambdas` (float vector):
    relaxation step sizes r1, r2. Needs r1 > 0 , r2 > 0 and r1 > r2.

:option:`noise` (float):
    factor for strength of noise term. Typical values should be [0.0 -- 0.1]

:option:`updatefields` (string vector):
    names of PhysFields that steepest descent algorithm acts upon	       

:option:`interactions` (string vector):
    names of Interaction's used in Hamiltonian model

    
Example steepestDescent Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(bulk only)

::

    <Updater wAwB>

       kind = steepestDescent
       type = incompressible
       relaxlambdas = [0.20 0.10]
       noise = 0.002

       updatefields = [totStyrDens totEthyDens]
       interactions = [StyrEthy]

    </Updater>


See also
~~~~~~~~~~
    - :ref:`updater`
    - :ref:`interaction`
    - :ref:`physfields`
