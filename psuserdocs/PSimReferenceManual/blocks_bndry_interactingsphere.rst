.. _interactingsphere:

Spherical Nanoparticles
------------------------------

:command:`interactingSphere`:
    kind of :command:`Boundary`. Specifies properties of a movable spherical
    nanoparticle.

    
interactingSphere Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`STFunc` (block):
    Space-time function block for specifying a region where particles can be
    dynamically inserted into the simulation domain

:option:`maxNumPtcls` (int):
    maximum number of particles to be inserted

:option:`updateMovePeriod` (int):
    number of iterations between particle position update calculation

:option:`updateAddPeriod` (int):
    number of iterations between particle insertion attempts

:option:`tstepBeforeFirstAdd` (int):
    number of iterations between particle insertion attempts begin

:option:`ptclNoiseFactor` (float):
    strength of random spatial displacement [0.0 -- 1.0]

:option:`interfaceWidth` (float):
    controls the size of particle edge (width in a Tanh() profile)

:option:`scaleForceFactor` (float):
    scales the strength of effective force used in position update algorithm

:option:`radius` (int):
    radius of a particle in number of simulation cells

:option:`maxdr` (float):
    maximum displacement allowed for particle update (in R_g)

:option:`boundaryfield` (string):
    block name of PhysField for specifying interactions with surrounding
    polymer densities

:option:`boundaryFieldThreshold` (float):
    value of incompressiblity field that defines 'edge' of a particle.
    Used in particle-particle, particle-wall distance calculations

    
Example interactingSphere Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is an example of a Boundary block that specifies parameters for
spherical nanoparticles that interact with the polymer chains. These
interactions exert effective forces on the nanoparticles, that move the
particles. This in turn causes the polymer chains to rearrange. This
continues until the system reaches a self-consistent steady state configuration
of nanoparticles and polymer fields.

::

  $ PTCLRADIUS_SIZE = 0.20 # in units of Rg
  $ PTCLRADIUS = int(PTCLRADIUS_SIZE/DX)

  $ PTCLWIDTH_SIZE = 0.10 # in units of Rg
  $ PTCLWIDTH = PTCLWIDTH_SIZE/DX

  $ LI_EDGE = float(0.4 * NX)
  $ RI_EDGE = float(NX - LI_EDGE)


  <Boundary sphereParticle1>
     kind = interactingSphere
     maxNumPtcls = 3
     updateMovePeriod = 500
     updateAddPeriod = 500
     tstepBeforeFirstAdd = 500
     ptclNoiseFactor = 0.5
     interfaceWidth = PTCLWIDTH
     scaleForceFactor = 10.0
     radius = PTCLRADIUS
     maxdr = 0.25
     boundaryfield = totParticle
     bndryFieldThreshold = 0.0005

     <STFunc insertRegion>
       kind = pyfunc
       name = stripeRegionsX
       paramKeys = [ leftedge rightedge ]
       paramValues = [ LI_EDGE  RI_EDGE ]
     </STFunc>

  </Boundary>
 
In this example the Boundary block has a nested STFunc block that controls
where nanoparticles are allowed to be inserted into the system as the system
relaxes towards a steady-state solution. If this STFunc block 'insertRegion'
is removed, nanoparticles are allowed to attempt insertion anywhere within
the simulation.


See also
~~~~~~~~~~
    - :ref:`updater` 
    - :ref:`physfields`
    - :ref:`stfunc`
