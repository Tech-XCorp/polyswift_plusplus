.. _is-tutorial-intro:

Interaction Surfaces Tutorial
--------------------------------------------------------

.. _is-tutorial-intro-basic-simulation-model:

Interacting Surfaces/Confinement Model
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This tutorial covers how to simulate a dense melt of confined monodisperse 
AB diblock copolymer chains.

A Hamiltonian for this system can be written as

      .. math::
       H = \frac{1}{4 R_{{\rm g}0}^2 } \sum^{n}_{\alpha = 1}
           \int_0^1 ds \left ( \frac{ d {\vec r}_{\alpha}(s)}{ds} \right )^{2} 
            + \rho_0^{-1} \int d{\vec r} \ \chi_{AB} \ {\hat \rho_A}({\vec r}) {\hat \rho_B}({\vec r}) \\
            + \rho_0^{-1} \int d{\vec r} \ \chi_{WA} \    \varrho_W ({\vec r}) {\hat \rho_A}({\vec r}) \\
	    + \rho_0^{-1} \int d{\vec r} \ \chi_{WB} \    \varrho_W ({\vec r}) {\hat \rho_B}({\vec r})

The first two terms are the same as the bulk model and are described 
in :ref:`is-tutorial-lesson-1`.
The positions of each wall-monomer can in principle be
described explicitly with a function :math:`{\hat \varrho_W} ({\vec r})`,
as are the monomers within the diblock chains.
However, this is unnecessary because these particles
are forced to be stationary and therefore the entropy due
to the wall-particles does not contribute to the total partition function.
The particle density inside the wall is assumed to be sufficiently large,
such that the continuous density field :math:`\varrho_W ({\vec r})` is an 
appropriate description of the interactions between particles in the wall and 
monomers along the polymer chains.

With :math:`\chi_{AB} > 0`, an overall repulsive force between
dissimilar monomers is modelled, which drives phase separation. The wall 
interaction parameters :math:`\chi_{WA}`
and
:math:`\chi_{WB}` determine the
tendency for the polymer chains to either wet or be excluded from the 
confining surface.
The relative sizes of the :math:`\chi` parameters determine the effective 
attraction or replusion for a monomer species to a wall species.
For example, if :math:`\chi_{WA} < \chi_{WB}` then A monomers preferentially 
wet the surface wall.
If :math:`\chi_{WB} < \chi_{WA}` then B monomers preferentially wet the
surface wall, and if :math:`\chi_{WB}=\chi_{WA}` then the wall is effectively 
neutral.
The absolute sizes of :math:`\chi_{WA}, \chi_{WB}`.... determine how strongly 
the monomers are excluded from the confinement walls.


The A/B monomer density operators and the wall density function are:

      .. math::
       {\hat \rho_A}({\vec r}) = N \sum^{n}_{\alpha = 1} \int_0^f ds \ \delta({\vec r} - {\vec r}_{\alpha}(s)) \\
       {\hat \rho_B}({\vec r}) = N \sum^{n}_{\alpha = 1} \int_f^1 ds \ \delta({\vec r} - {\vec r}_{\alpha}(s)) \\
        \varrho_W ({\vec r}) \in  [0,\rho_0] .

where :math:`f` is the fraction of the chain consisting of A monomers and
:math:`\varrho_W ({\vec r})` can in general be an arbitrary function of position.


.. _is-tutorial-intro-about-the-example-pre-file:

About the Example Input File, interactingSurfaces.pre
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :ref:`interactingSurfaces.pre` file has been selected for use with the 
example simulation in this section because it is representative of
the input blocks needed to define a simulation of this type.
These blocks define geometries, setup the additional fields
needed to implement the confinement model and define the appropriate energetic
model to describe the interactions of the confining surfaces and the monomers 
on the copolymer chains.


PolySwift++ Features Demonstrated
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The interactingSurfaces example demonstrates the following PSim features:

    - specifying geometry

    - defining wall interaction model

    - associating different surfaces with specific monomer interactions


In the next section :ref:`is-tutorial-lesson-1`, you will examine the 
differences in the input file blocks for simulations with confinement.
