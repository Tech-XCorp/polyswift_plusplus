.. _physfields:

Physical Fields
-----------------------

:command:`PhysField`:

	The physical fields are the primary data-containing blocks in
	Polyswift++. They contain a field for the main observable quantity
        and its corresponding conjugate potential within the self-consistent
	field theory.



Physical Field Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`monomerDens`
       Monomer density field from monomers on polymer chains or those
       from solvent species

    :option:`constraint`
       Field that enforces the incompressibility condition

:option:`type`:
    Specific parameter for physical fields that set the datatype and dimension
    of the underlying data field


Example of Physical Field block
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

	<PhysField totStyrDens>
	   kind = monomerDens
	   type = fieldD3R
	</PhysField>
