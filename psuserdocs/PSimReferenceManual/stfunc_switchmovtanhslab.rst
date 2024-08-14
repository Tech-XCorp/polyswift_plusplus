.. _stfunc-switchMovTanhSlab:

Switchable Moving Hyperbolic Tangent Slab
-------------------------------------------

:command:`switchMovTanhSlab`:

    Defines properties for a moving slab (with normal parallel to grid 
    direction) with one value inside the slab and another value outside the 
    slab and a smooth boundary in-between described by a tanh() profile. The 
    slab orientation switches direction (x-->y---z) once trailing edge of 
    slab is outside simulation domain


.. _switchmovtanhslab-parameters:    

Switchable Moving Tanh Slab Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 

:option:`kind`:
     :option:`movTanhSlab`

:option:`widthParam`:
     width parameter in the hyperbolic tangent profile of the slab 'edge'

:option:`zoneSize`:
     width of the zone region

:option:`chiNmax`:
     Flory :math:`chiN` parameter for the region 'inside' the zone

:option:`chiNmin`:
     Flory :math:`chiN` parameter for the region 'outside' the zone

:option:`zoneBuffers`:
     Size of buffer outside of simulation grid that the zone effectively
     moves through once it leaves the simulation (one for each cartesian
     component). Typically, these zones should be large enough to allow the
     zone to completely leave the simulation before entering by another
     direction

:option:`expression`:
     Mathematical expression that determines the location of the center of
     the zone as a function of update step (e.g.  expression = 0.0 + 0.1*t)

:option:`maxSweeps`:
     Maximun number of sweeps the zone makes during the simulation. The
     default is to continue sweeping throughout the simulation.


Example switchMovTanhSlab Block
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

    $ zoneVelocity = 0.10
    $ sizeZone = 4.0
    $ edgeWidth = 4.0
    $ sweepsMax = 3
    $ xzoneBuffer = float(NX*1.4)
    $ yzoneBuffer = float(NY*1.4)

    <STFunc chirzoneY>
      kind = switchMovTanhSlab
      widthParam = edgeWidth
      zoneSize = sizeZone
      chiNmax = 12.0
      chiNmin = 18.0
      zoneBuffers = [ xzoneBuffer yzoneBuffer ]
      expression = -60.0 + zoneVelocity*t
      maxSweeps = sweepsMax
    </STFunc>
