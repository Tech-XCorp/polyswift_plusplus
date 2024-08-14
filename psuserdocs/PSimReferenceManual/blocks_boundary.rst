.. _boundary:

Boundary
---------------------

:command:`Boundary`:

    blocks define the properties of fields that specify inclusions in an SCFT
    copolymer simulation. An inclusion is a region of the simulation that 
    excludes polymer/solvent densities by changing the average total density 
    enforced by the incompressibility condition. Blocks derived from the 
    Boundary block can be used to model surfaces, confinement and nanoparticle 
    composite mixtures.
    
    Boundary blocks specify properties of surfaces/particles.

    One or more boundary blocks can be created in a simulation.


Boundary Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
:option:`kind`:
    one of:

    :option:`fixedWall`:
        boundary object for a hard stationary surface

    :option:`interactingSphere`:
        boundary object for a spherical nanoparticle that interacts
	with polymer fields.

:option:`boundaryfield` (string):
    object name for a PhysField  needed for interactions


            
Boundary Sub-Blocks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    In addition to **Boundary** parameters, the
    **Boundary** block can contain the at least one of the
    following block(s):

        - :ref:`stfunc`

    An 'STFunc' object can be included to specify the spatial
    dependence of the Flory interaction parameters (kind='flory')

        
See also
~~~~~~~~~~~~

    - :ref:`physfields`
    - :ref:`interaction`
