.. _polyswift-examples:

Getting Up To Speed
---------------------------------------------------------------------------

Welcome to the PSim series of computational applications,
*powered by PolySwift++*. The PolySwift++ engine has algorithms that allow
simlations of bulk block copolymer mixtures, confined block copolymers
and nanocomposite mixtures that uses a hybrid-SCFT method for
incorporating particles explicitly in a block copolymer matrix.

This manual, *PSim Examples*, provides hands-on training
manual for new users of the PSim series of computational
applications. It demonstrates how to carry out simulations using
the PSimComposer interface to the input files.

The PSimComposer interface allows you to edit and
validate your simulation input files, run simulations in either
serial or parallel (thereby utilizing multiple cores or even
computational nodes that do not share memory), and visualize
results.  PSimComposer provides GUI editing of the main input
variables of appropriately marked-up input files. However, one
can still edit input files in any text editor, execute through
the command line and visualize in any tool that understands the
HDF5 output.

PSim setup and installation instructions are given in the
document, *PSim Installation*.
An introduction to the PSimComposer Graphical User Interface is
provided in :ref:`psimcomposer-intro`.

The subsequent sections provide examples that can be run through
PSimComposer. Each section is named for the PSim package or
module that is needed to run those examples.  That is, the
PSimBase chapter contains the examples that can be run with a
PSimBase license. All examples can be run with a PSimSuite license.

After learning how to run the PSim applications through
PSimComposer, you can turn to the manual, *PSim In Depth*,
to learn how to edit input files directly to create your own
custom simulations.  All input file blocks are specified in
the manual, *PSim Reference*.
