.. _flexpseudospec:

Flexible Block Model
------------------------------

:command:`flexPseudoSpec`:
    kind of :command:`Block` that defines a flexible chain model and 
    pseudospectral solution method.

    
flexPseudoSpec Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`bSegRatio` (float):
    ratio of statistical segment length to the segment length in the polymer
    block that defines the simulation length scale

    
Example flexPseudoSpec Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <Block blockA>
      kind = flexPseudoSpec
      scfield = totStyrDens
      ds = 0.05
      lengthfrac = 0.5
      headjoined = [freeEnd]
      tailjoined = [blockB]
    </Block>


See also
~~~~~~~~~~
    - :ref:`physfields`
    - :ref:`polymer`
