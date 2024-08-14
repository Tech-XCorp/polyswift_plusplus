.. _interaction:

Interaction
---------------------

:command:`Interaction`:

    blocks define the properties of general interactions for PhysFields in the
    simulation. One or more of these interactions are held by the EffHamil class
    container. Returns related quantities for updater objects


Interaction Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`flory`:
        Flory interaction between two PhysField objects

    :option:`floryWall`:
        Flory interaction between two PhysField object and a wall
	that interacts with a field with the same form as two bulk
	density fields

:option:`scfields` (string vector):
    object names for PhysFields needed for interactions


Interaction Sub-Blocks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    In addition to **Interaction** parameters, the
    **Interaction** block can contain the at least one of the following 
    block(s):

        - :ref:`stfunc`

    An 'STFunc' object can be included to specify the spatial
    dependence of interactions between physical fields.
    The 'kind=chiCutExpression' can also be used to define an expression parsed
    directly from the input file. There are other 'STFunc's available,
    see :ref:`stfunc`.

::

        <STFunc chiramp>
	  kind = chiCutExpression
	  chi_lower = 0.18
	  chi_upper = 0.188
	  chi = 0.18 + 0.000025*t
	</STFunc>

or

::

        <STFunc chirzone>
    	  kind = movTanhSlab
	  widthParam = 10.0
	  zoneSize = 10.0
	  chiNmax = 1.0
	  chiNmin = 16.0
	  pcomp = 0
	  expression = 0.0 + 0.1*t
        </STFunc>


        
See also
~~~~~~~~~~~~

    - :ref:`physfields`
    - :ref:`effhamil`
