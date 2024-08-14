.. _blockcopolymer:


Monodisperse Block Copolymer
---------------------------------

:command:`blockCopolymer`:

    kind of :command:`Polymer` is a container for monodisperse copolymer blocks.


blockCopolymer Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. :option:`somethingSpecific`:
..    The total volume fraction of all monomers from this type of polymer
..    volfrac must = 1.


    
Example blockCopolymer Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

      <Polymer diblock1>

        kind = blockCopolymer
	volfrac = 1.0
	length = 100

	<Block blockA>
 	  kind = flexPseudoSpec
	  scfield = totStyrDens
	  ds = 0.05
	  lengthfrac = 0.5
	  headjoined = [freeEnd]
	  tailjoined = [blockB]
        </Block>

	<Block blockB>
	  kind = flexPseudoSpec
	  scfield = totEthyDens
	  ds = 0.05
	  lengthfrac = 0.5
	  headjoined = [blockA]
	  tailjoined = [freeEnd]
        </Block>
      </Polymer>

See also
~~~~~~~~~~
    - :ref:`polymer`
