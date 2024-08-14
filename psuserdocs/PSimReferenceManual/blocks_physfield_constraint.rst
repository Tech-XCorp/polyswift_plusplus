.. _constraint:

Constraint Fields
------------------------------

:command:`constraint`:
    kind of :command:`PhysField` that enforces the total density in 
    incompressible SCFT models.
    Contains a pressure field and (for boundary models) a masking field that 
    denotes location of hard surfaces/nanoparticles.
    
constraint Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`excludeFields` (string vector):
    names of PhysFields for which constraint applies

:option:`preconditionFactor` (float):
    value of precondition factor for wall constraints
   
See also
~~~~~~~~~~
    - :ref:`physfields`
    - :ref:`monomerdens`
