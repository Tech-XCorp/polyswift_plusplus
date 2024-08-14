.. _fixedwall:

Fixed Wall/Surface
------------------------------

:command:`fixedwall`:
    kind of :command:`Boundary`. Specifies properties of a stationary 
    substrate or confinement surface

    
fixedwall Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`STFunc` (block):
    Space-time function block for specifying the spatial extent of the boundary
    object

:option:`wallEdgeThreshold` (object):
    value of scaled incompressibility value for defining the 'edge' of a 
    surface. Used in distance calculations, e.g. between a wall and a 
    nanoparticle.

    
Example fixedWall Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    $ CYLRADIUS = float(NX/2 - int(0.05*NX))
    $ CYLCENTER_X = float(NX/2)
    $ CYLCENTER_Y = float(NY/2)
    $ CYLCENTER_Z = float(1)

    # Sets the hyperbolic tangent profile
    $ WIDTHPARAM = 1.5

    <Boundary softEthyWalls>

      kind = fixedWall
      boundaryfield = totWall

      <STFunc walls>
         kind = pyfunc
         name = cylinder
         paramKeys = [ radius widthParam cylcenterX cylcenterY cylcenterZ ]
         paramValues = [ CYLRADIUS WIDTHPARAM CYLCENTER_X CYLCENTER_Y CYLCENTER_Z ]
      </STFunc>

    </Boundary>
 

See also
~~~~~~~~~~
    - :ref:`updater` 
    - :ref:`physfields`
    - :ref:`stfunc`
