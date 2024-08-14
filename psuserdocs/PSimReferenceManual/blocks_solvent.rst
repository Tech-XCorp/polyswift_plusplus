.. _Solvent:


Solvent
------------------

:command:`Solvent`:

    blocks are one of the simulation “entities” in Polyswift++.
    A simulation “entity” represents real experimental objects to be studied.
    A solvent is any object that contains a self-consistent field that describes
    how a monomer-like species affects the mixtures being simulated


Solvent Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`simpleSolvent`:
      a simple uncharged solvent species that interacts through a Flory-type
      interaction

:option:`volfrac`:
    The total volume fraction of this species. When there are multiple Polymer
    or Solvent objects the total volfrac must = 1.

:option:`scfield`:
    object name of a PhysField that defines the type of monomer associated 
    with this solvent entity.
