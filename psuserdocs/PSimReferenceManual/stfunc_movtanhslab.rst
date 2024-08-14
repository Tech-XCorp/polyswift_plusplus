.. _stfunc-movTanhSlab:


Moving Hyperbolic Tangent Slab
-----------------------------------

:command:`movTanhSlab`:
    Defines properties for a moving slab (with normal parallel to grid 
    direction) with one value inside the slab and another value outside the 
    slab and a smooth boundary in-between described by a tanh() profile

.. _movtanhslab-parameters:    

movTanhSlab Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^

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

:option:`pcomp`:
     Index of the direction parallel to the zone movement (ie perpendicular
     to the zone face) (x=0, y=1, z=2)

:option:`expression`:
     Mathematical expression that determines the location of the center of
     the zone as a function of update step (eg  expression = 0.0 + 0.1*t)



Example movTanhSlab Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <STFunc chirzone>
      kind = movTanhSlab
      widthParam = 10.0
      zoneSize = 10.0
      chiNmax = 1.0
      chiNmin = 16.0
      pcomp = 0
      expression = 0.0 + 0.1*t
    </STFunc>
