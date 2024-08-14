.. _EffHamil:


Effective Hamiltonian
---------------------------

:command:`EffHamil`:

	The effective Hamiltonian block is essentially a container for updater
	and interaction blocks. The combination of interaction and updater 
	blocks defines the model and how the physical fields are manipulated 
	throughout the simulation.



Effective Hamiltonian Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    only one:

    :option:`canonicalMF`:
       mean-field canonical model


:option:`updaterSequence`:
    A list of identifiers for Updater objects that lists the order in which
    they are to be applied


EffHamil Sub-Blocks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    In addition to **EffHamil** parameters, the **EffHamil** block must contain
    at least one of the following blocks:

       - :ref:`Updater`
       - :ref:`Interaction`

    There is at least one 'Updater' block for the polymer/solvent physical 
    fields.  Updater blocks can be added for spectral filtering and/or other 
    auxillary update algorithms.

    There is at least one 'Interaction' block for a minimal copolymer model with
    two monomer species. There should be an 'Interaction' block for each unique
    combination of two monomer species (e.g. for 3 monomer species in bulk 
    there should be 3 'Interaction' blocks).


Example of EffHamil block
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

	<EffHamil mainHamil>

	  kind = canonicalMF
          updaterSequence = [wAwB]

 	  <Updater wAwB>

	     kind = steepestDescent
	     type = incompressible
	     relaxlambdas = [lambda1 lambda2]
	     noise = noise_strength
	     updatefields = [totStyrDens totEthyDens]
	     interactions = [StyrEthy]

	  </Updater>

	  <Interaction StyrEthy>
	     kind = flory
	     chi = chiAB
	     scfields = [totStyrDens totEthyDens]
	  </Interaction>

        </EffHamil>
