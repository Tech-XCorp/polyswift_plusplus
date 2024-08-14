.. _Polymer:


Polymer
-------------------

:command:`Polymer`:

    blocks are one of the simulation “entities” in Polyswift++.
    A simulation “entity” represents real experimental objects to be studied.
    A polymer is any object that contains sub-blocks representing contiguous
    monomer chains



Polymer Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`blockCopolymer`:
        Monodisperse model of arbitrary architecture of copolymer blocks

:option:`volfrac`:
    The total volume fraction of all monomers from this type of polymer
    chain. When there are multiple Polymer or Solvent blocks the total
    volfrac must = 1.

:option:`length`:
    This is the number of statistically independent segments comprising
    this polymer. This scales the value of Flory interaction parameters
    and charge strengths.



            
Polymer Sub-Blocks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    In addition to **Polymer** parameters, the
    **Polymer** block must contain the at least one of the following block(s):

        - :ref:`block`

    One 'Block' object is included for each block on the copolymer object.
    Note: one 'Block' object corresponds to a homopolymer chain.
