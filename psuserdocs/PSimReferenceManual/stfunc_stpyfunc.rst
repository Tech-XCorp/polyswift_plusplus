.. _stfunc-pyfunc:

External Python Function
--------------------------

:command:`pyfunc`:
    function defined by a Python script, which is contained in a separate 
    file. The parameter name is used to point to this file. 
    

.. _stpyfunc-parameters:    

pyfunc Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^ 

:option:`name` (string):
    name of the function.

:option:`paramKeys` (string vector , optional):
    names of parameter to set as keys in Python dictionary

:option:`paramValues` (float vector , optional):
    values of parameters to set as values in Python dictionary corresponding
    to the key values above



Example pyfunc Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Python block in a Python file with same name as the input file.

::

   #
   # Sine modulated wall on "negative Z" side of system
   #
   def posZFlatWall(x, y, z, t, params={'widthParam':1.0,'walledge':10}):
          
     global NX
     global NY
     global NZ

     # Convert dictionary to variable assigments
     globals().update(params)
     r = [x,y,z]
     negEdge = [x,y,0]

     negRdis = rdistance(r,negEdge)
     tanhval = math.tanh((negRdis-walledge)/widthParam)
     wallValue = (tanhval+1)/2.0
     return wallValue


Block in PSim referencing the Python function defined in the Python file.

::

  # Defines left(bottom) and right(top) edges
  # of flat walls... along Y for both 2D and 3D
  # domain decomp
  $ LOWEDGE = float(14.0)
  $ HIGHEDGE = float(LOWEDGE + 50)
  $ WALLEDGE_POSZ = float(NZ - LOWEDGE)

  # Sets the hyperbolic tangent profile
  $ WIDTHPARAM = float(0.1/DX)

  <Boundary topNeutralWall>
    kind = fixedWall
    oundaryfield = totWall
    bndryFieldThreshold = 0.005

    <STFunc walls>
      kind = pyfunc
      name = posZFlatWall
      paramKeys = [ widthParam walledge ]
      paramValues = [ WIDTHPARAM  WALLEDGE_POSZ ]
    </STFunc>

  </Boundary>
