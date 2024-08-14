.. _stfunc-expression:

General Expression
--------------------------------------------------

:command:`expression`:
    function that can be defined by an arbitrary expression. 
    :samp:`expression` is the most versatile STFunc, and uses 
    the parameter :samp:`expression` to define the expression to be used.

.. note::
    only certain STFunc blocks may be used in specific top-level blocks.
    See reference manual for each top-level blocks for details.
    

.. _expression-stfunc-parameters:
    
expression Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`expression` (string):
    expression to be evaluated.


Example expression Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

      <STFunc component0>
        kind = expression
        expression = 100.*sin(2.0e9*t)
      </STFunc>

