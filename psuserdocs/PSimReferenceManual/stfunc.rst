.. _stfunc:

Space-Time Function Block
--------------------------------------------

:command:`STFunc`:
    Space-Time-Function block. Many blocks in PSim can use space-time 
    functions. These are normally contained within other blocks that 
    require time signals or spatial profiles. 

    When using an STFunc block, place the function in its own STFunc block, 
    specifying the function using the :option:`kind` parameter as in the 
    :ref:`example-stfunc-block`. 

    Most users make use of :samp:`kind = expression` to write their own 
    functions. 

    Descriptions of STFunc parameters are described on each STFunc kind 
    page. 


STFunc Kinds
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    - :ref:`stfunc-expression`

    - :ref:`stfunc-cutExpression`

    - :ref:`stfunc-chiCutExpression`

    - :ref:`stfunc-movTanhSlab`

    - :ref:`stfunc-switchMovTanhSlab`

    - :ref:`stfunc-pyfunc`

.. _example-stfunc-block:

Example STFunc Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example the *Interaction* block parameter 'chi' is replaced
by the STFunc 'chiramp' which sets the chi values as a function of update step.

::

    <Interaction StyrEthy>
       kind = flory
       scfields = [totStyrDens totEthyDens]
       printdebug = DBINTERACTION

       <STFunc chiramp>
          kind = chiCutExpression
          chi_lower = 0.18
          chi_upper = 0.188
          # This will add to the default chi above
          chi = 0.18 + 0.000025*t
       </STFunc>

    </Interaction>
