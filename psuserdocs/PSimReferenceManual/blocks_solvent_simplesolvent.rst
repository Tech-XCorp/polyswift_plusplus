.. _simplesolvent:


Simple Neutral Solvent
-------------------------

:command:`simpleSolvent`:

    blocks simulate a model of a solvent such that each solvent unit
    has the same volume as a monomer on the polymer chains.
    This is a neutral solvent model.


simpleSolvent Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. :option:`somethingSpecific`:
..    object name of a PhysField that defines the type of monomer associated



Example simpleSolvent Block
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

	<Solvent solvent1>
	  kind = simpleSolvent
	  volfrac = 0.20
	  scfield = totSolDens
	</Solvent>
