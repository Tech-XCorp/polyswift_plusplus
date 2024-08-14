.. _monomerdens:

Monomer Density Fields
------------------------------

:command:`monomerDens`:
    kind of :command:`PhysField` that defines a physical field in the SCFT 
    model. Contains a monomer density and the conjugate chemical potential.
    
monomerDens Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`initOption` (string):
    string flag for setting the initial chemical potential field.
    
    :option:`random`:
        uniform random values set betweeen [-0.5, 0.5]


Example monomerDens Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <PhysField totStyrDens>
        kind = monomerDens
        type = fieldD3R
	initOption = "random"
    </PhysField>


See also
~~~~~~~~~~
    - :ref:`physfields`
