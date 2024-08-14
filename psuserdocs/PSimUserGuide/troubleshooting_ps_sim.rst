.. _troubleshooting-ps-sim:

Troubleshooting Numerical SCFT Simulations
---------------------------------------------

This chapter discusses common problems that you may encounter as you
modify examples and develop your own SCFT polymer simulations.

The Simulation Does Not Start/Finish Properly
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If submitted via PBS, check queue time limits

Check contour step size. If one copolymer block in simulation is too small
the contour step size for this block may have to be decreased.


The Simulation Becomes Unstable
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Check the resolution and update step size.

For hybrid SCFT, check simulation log for excessive overlap for incompressibility
constriant
