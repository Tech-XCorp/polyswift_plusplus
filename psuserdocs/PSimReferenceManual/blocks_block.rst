.. _block:

Block
---------------------

:command:`Block`:

   define the concept of a linear string of chemically
   similar monomers with a "head" end and a "tail" end. These blocks exist
   as space curves in the SCF theory. Kinds of block models include flexible, 
   and semi-flexible.  Specific solution models include pseudo-spectral.
   These blocks are nested in Polymer blocks.


Block Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`flexPseudoSpec`:
        Fully flexible chain model solved with pseudo-spectral algorithm

    :option:`chargedFlexPseudoSpec`:
        Fully flexible chain model solved with pseudo-spectral algorithm
	and including a contribution from an electric potentail term in
	polyelectrolyte models

    :option:`semiflexibleBlock`:
	Semi-flexible block model (not-fully implemented). Templated over
	different NDIM parameters



:option:`scfield` (string):
    object name of a PhysField that defines the type of monomer in this block.

:option:`blockfield` (string: optional):
    object name of a PhysField that tracks contribution to overall density
    for this block only

:option:`lengthfrac` (float):
    Length fraction of this block. Values --> [0.0 -- 1.0].
    All lengthfrac values for all blocks in a Polymer object must sum to 1.0.

:option:`headjoined` (string vector):
    List of block name(s) that the head of this block is(are) connected.
    If this block end is not connected to any other block then the
    reserved string "freeEnd"

:option:`tailjoined` (string vector):
    Same of headjoined but for the other end of this block

:option:`ds` (float):
    Contour step size
        
See also
~~~~~~~~~~~~

    - :ref:`physfields` 
    - :ref:`effhamil` 
