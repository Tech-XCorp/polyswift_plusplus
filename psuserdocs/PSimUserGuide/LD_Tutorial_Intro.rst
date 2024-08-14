.. _ld-tutorial-intro:

Linear Diblock Simulation Tutorial
--------------------------------------------------------

.. _ld-tutorial-intro-basic-simulation-model:

Basic Linear Diblock Simulation Model
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This tutorial covers how to simulate a dense melt of monodisperse AB 
diblock copolymers chains.  A Hamiltonian for this system can be written as:

      .. math::
       H = \frac{1}{4 R_{{\rm g}0}^2 } \sum^{n}_{\alpha = 1}
           \int_0^1 ds \left ( \frac{ d {\vec r}_{\alpha}(s)}{ds} \right )^{2} +
           \rho_0^{-1} \int d{\vec r} \ \chi \ {\hat \rho_A}({\vec r}) {\hat \rho_B}({\vec r}) .

where the first term expresses the free-energy of a "Gaussian thread" model.
This term can be derived by taking the continuous limit of spherical monomers
interacting through harmonic springs.
The second term expresses a Flory-type interaction energy between chemically 
distinct monomers (eg A-B).

The monomer density operators for the **A** and **B** species are

    .. math::
       {\hat \rho_A}({\vec r}) = N \sum^{n}_{\alpha = 1} \int_0^f ds \ \delta({\vec r} - {\vec r}_{\alpha}(s)) \\
       {\hat \rho_B}({\vec r}) = N \sum^{n}_{\alpha = 1}\int_f^1 ds \ \delta({\vec r} - {\vec r}_{\alpha}(s))

where :math:`f=0.5` and corresponds to a symmetric AB diblock.

The SCF theory yields a partition function

    .. math::
       Z = \int \prod^{n}_{\alpha = 1} \tilde{D} {\vec r}_{\alpha} \
      \exp \left \lbrace
           - \rho_0^{-1} \int d{\vec r} \ \chi \ {\hat \rho_A}({\vec r}) {\hat \rho_B}({\vec r})
           \right \rbrace
      \delta[\rho_0 - {\hat \rho_A} - {\hat \rho_B}]

with

    .. math::
       \tilde{D} {\vec r}_{\alpha} =  D {\vec r}_{\alpha} \exp \left \lbrace
           - \frac{1}{4 R_{{\rm g}0}^2 } \int_0^1
           ds \left ( \frac{ d {\vec r}_{\alpha}(s)}{ds} \right )^{2}
           \right \rbrace


This tutorial examines the parameters and input blocks that comprise the 
linearDiblock.pre file distributed with PSim.
The linearDiblock.pre file demonstrates the basics of numerical self-consistent
field theory (SCFT) simulations of bulk copolymers using the PolySwift++ engine.
This section assumes you are familiar with the material in the  
section, :ref:`basic-concepts-for-all-simulations`, from this manual.


.. _ld-tutorial-intro-about-the-example-pre-file:

About the Example Input File, linearDiblock.pre
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :ref:`linearDiblock.pre` file has been selected
for use with the example simulation in this section because this file is
simple enough to examine section by section, yet complicated enough to
demonstrate a variety of physics features and PSim features.


PolySwift++ Features Demonstrated
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The linearDiblock example demonstrates the following PSim features:

    - setup for the pseudospectral solution algorithm

    - basics for setting up bulk simulation of copolymer system

    - visualizing morphology results and diagnostics


In Lesson 1 of this Linear Diblock Simulation Tutorial, you will define the 
conceptual model for the linearDiblock example on the 2D Cartesian grid 
following the instructions in :ref:`ld-tutorial-lesson-1-symmetric-diblock`.
