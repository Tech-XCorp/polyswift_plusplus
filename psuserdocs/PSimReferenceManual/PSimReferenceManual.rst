.. _psim-reference:

.. PSim Reference Manual master file.
   This must at least contain the root `toctree` directive.
   Entries in this file must correspond to matching entries 
   in the PSim Documentation Set master file.

 
PSim Reference Manual
===============================

Contents:

.. toctree::
   :maxdepth: 2

Introduction
---------------

.. toctree::
   :maxdepth: 1
   
   overview.rst
   background.rst
   input_file_basics.rst

   
Grid
-------

.. toctree::
   :maxdepth: 1
   
   blocks_grid.rst


Domain decomposition
------------------------------

.. Decomp Block

.. toctree::
   :maxdepth: 1
   
   blocks_decomp.rst

Decomp Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. toctree::
   :maxdepth: 1
   
   blocks_decomp_fftw.rst



Surfaces/Nanoparticles
--------------------------

.. Boundary Block

.. toctree::
   :maxdepth: 1

   blocks_boundary.rst

Boundary Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_bndry_fixedwall.rst
   blocks_bndry_interactingsphere.rst


Comm
---------

.. toctree::
   :maxdepth: 1

   blocks_comm.rst


Interactions
----------------

.. Interaction Block

.. toctree::
   :maxdepth: 1

   blocks_interaction.rst


Interaction Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_i_flory.rst
   blocks_i_floryWall.rst


General Updaters
------------------

.. Updater Block

.. toctree::
   :maxdepth: 1

   blocks_updater.rst


Updater Kinds 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_u_steepestdescent.rst
   blocks_u_simplespecfilter.rst
   blocks_u_multispecfilter.rst
   blocks_u_poisson.rst



EffHamil
---------

.. toctree::
   :maxdepth: 1

   blocks_effhamil.rst


FFT
---------

.. FFT Block

.. toctree::
   :maxdepth: 1

   blocks_fft.rst

FFT Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_fft_normalfftw.rst
   blocks_fft_transposefftw.rst



Physical Fields for SCFT models
----------------------------------

.. PhysField Block

.. toctree::
   :maxdepth: 1

   blocks_physfield.rst


PhysField Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_physfield_monomerdens.rst
   blocks_physfield_constraint.rst

.. For PSIM-1.1.0
..   blocks_physfield_chargedens.rst



Polymer
---------

.. toctree::
   :maxdepth: 1

   blocks_polymer.rst

Polymer Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_polymer_blockcopolymer.rst


Polymer Chain Blocks
-------------------------

.. Block

.. toctree::
   :maxdepth: 1

   blocks_block.rst

Block Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_block_flexpseudospec.rst
.. For PSIM-1.1.0
..   blocks_block_chargedflexpseudospec.rst
..   blocks_block_semiflexibleblock.rst


Solvent
---------

.. toctree::
   :maxdepth: 1

   blocks_solvent.rst

Solvent Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   blocks_solvent_simplesolvent.rst



History Diagnostics
--------------------------

.. History Block

.. toctree::
   :maxdepth: 1
   
   blocks_history.rst

History Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1
   
   blocks_hist_freeenergy.rst
   blocks_hist_floryconstchi.rst


STFunc Block
-----------------------

.. toctree::
   :maxdepth: 1

   stfunc.rst


STFunc Kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1

   stfunc_expression.rst
   stfunc_cutexpression.rst
   stfunc_chicutexpression.rst
   stfunc_movtanhslab.rst
   stfunc_switchmovtanhslab.rst
   stfunc_stpyfunc.rst
   
.. include:: ../PSimExamples/Trademark/trademark.rst
