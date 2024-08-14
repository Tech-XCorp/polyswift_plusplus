.. _stfunc-chiCutExpression:

Expression w/Cutoff for Flory Chi
--------------------------------------------------

:command:`chiCutExpression`:
    function that can be defined by an arbitrary expression,
    along with upper and lower bounds with names that emphasize
    use for Flory :math:`\chi` parameters
    
    
.. _chicutexpression-stfunc-parameters:
    
chiCutExpression Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`expression` (string):
    expression to be evaluated.

:option:`chi_lower` (float):
    Flory chi lower bound

:option:`chi_upper` (float):
    Flory chi upper bound



Example chiCutExpression STFunc Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

       <STFunc chiramp>
          kind = chiCutExpression
          chi_lower = 0.18
          chi_upper = 0.188
          chi = 0.18 + 0.000025*t
       </STFunc>
