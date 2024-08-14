.. _programming-concepts-python-token-evaluator-txpppy:

Python Token Evaluator (txpp.py)
------------------------------------

The Python preprocessor has the following features:

    - It accepts a file, conventionally with suffix :samp:`.pre`, for 
      processing.
    - Lines in that file that start with the character :samp:`$` are processed 
      by the preprocessor.
    - Those lines are sent through the python interpreter for evaluation
    - The resulting values are replaced and written to a new file with
      suffix, :samp:`.in`

.. note::
   Recent versions of Tech-X codes (including PSim) use the txpp.py code 
   internally (i.e. the txpp.py python code is called within simulation 
   engines using the Python/C++ API)
   The txpp.py code can still be used externally, in order to debug input 
   file code.


For example, suppose one has a file, myfile.pre, containing,

::

	################################################
	# A Boundary object
	################################################
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

    
Execution of:

::

     <txpp.py directory>/txpp.py --prefile=myfile.pre


produces a file, myfile.in that contains:

::

    
    

	################################################
	# A Boundary object
	################################################
	#$ CYLRADIUS = float(NX/2 - int(0.05*NX))
	# -->   CYLRADIUS       =       58.0
	#$ CYLCENTER_X = float(NX/2)
	# -->   CYLCENTER_X     =       64.0
	#$ CYLCENTER_Y = float(NY/2)
	# -->   CYLCENTER_Y     =       64.0
	#$ CYLCENTER_Z = float(1)
	# -->   CYLCENTER_Z     =       1.0
	# Sets the hyperbolic tangent profile
	#$ WIDTHPARAM = 1.5
	# -->   WIDTHPARAM      =       1.5
    
	<Boundary softEthyWalls>
	  kind=fixedWall
	  boundaryfield=totWall
  
	  <STFunc walls>
	    kind=pyfunc
	    name=cylinder
            paramKeys=[radius widthParam cylcenterX cylcenterY cylcenterZ]
            paramValues=[58.0 1.5 64.0 64.0 1.0]
          </STFunc>

        </Boundary>

    
This mechanism facilitates modifying files to change systems size,
resolution, or other parameters while keeping requisite mathematical
relationships intact.

The preprocessor imports math, so one can include statements such as:

::

        $ PI = math.pi 
    
    
and then use the variable PI in the pre file. The replace
occurs for commented lines as well.
This is useful for printing out intermediate values for, e.g.,
debugging. In addition, the value of 
any variable named :samp:`NDIM` defined in the input file can be modified 
on the command line with the directive :samp:`NDIM=2`, for example, 
to have all occurrences of NDIM in the file replace by :samp:`2` instead 
of the value defined in the file. This enables writing only a single pre 
file for simulations of multiple dimensionalities when the differences 
in the file follow from the value of NDIM alone.


Interpretation of Parameters and Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The Python preprocessor simplifies setup of complex simulations by 
providing an abstraction mechanism. With the pre-processor's macro 
features, you can write parameterized input files and encapsulate input 
file fragments into libraries for later use. By using a macro (described in
detail later) you can easily reuse the same code in more than one place. 
Rather than repeating the code, you just call the single macro to be 
substituted for that code each place you would like to use it. 

The type of value expected by PSim for each parameter and variable is
identified in parentheses next to the name of that parameter or variable in
the description of each feature or block. The PSim input parser
distinguishes between types for integers, floating-point numbers, and
strings based on input format. Expressions are parsed for values
following the order:

-  integer
-  floating-point
-  string (text)
-  vector


Integer
   If a symbol can be parsed as an integer, PSim assumes that integer is the
   correct type. 
   
   Example of an integer: 
   
   :samp:`42` 


Floating Point
   If the integer format comparison fails, the software tries to parse the 
   symbol as a floating-point number. Floating-point numbers must be 
   written with a decimal point so they will not be interpreted as integer 
   parameters. 
   
   Examples of floating-point numbers: 

   :samp:`3.14159` 

   :samp:`1.60217646e-19` 

   To assign an integer value to a floating-point variable, make sure it is 
   written with a decimal point, otherwise, the variable will be 
   interpreted as an integer, and this will likely produce unexpected 
   results. 

   Example of correct integer value format for usage with a floating-point 
   variable: 

   :samp:`3\.` 

   Example of incorrect integer value format for usage with a floating-point variable: 

   :samp:`3` 


String
   If the integer and subsequent floating-point comparisons both fail 
   PSim interprets the variable as string of text characters rather than a number. 


Vector
   When parsing a vector type of variable, PSim uses the same logic and order of
   analysis as for non-vector variables. For example, 
   PSim would interpret the following example as a vector having three 
   integer components: 
   
   :samp:`lowerBounds = [-1 -1 -1]`
