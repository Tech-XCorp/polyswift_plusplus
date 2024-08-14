.. _floryconstchi:

Record Flory Chi (spatially homogeneous)
------------------------------------------

:command:`floryConstChi`:
    kind of :command:`History` that records the changing value of spatially
    homogeneous Flory chi parameter


floryConstChi Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`interactionName` (string):
    string name for Interaction block needed to record chi value

    
Example floryConstChi Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For a simulation with the following Flory interaction block
::


  <Interaction EthyCarb>
    kind = flory
    scfields = [totEthyDens totCarbDens]
    printdebug = DBINTERACTION

    <STFunc chiramp>
      kind = chiCutExpression
      chi_lower = 0.18
      chi_upper = 0.22
      # This will add to the default chi above
      chi = 0.18 + 0.00001*t
    </STFunc>

  </Interaction>

this History block will record current chi value
::


  <History chiN_EC>
    kind = floryConstChi
    updatePeriodicity = 10
    interactionName = EthyCarb
  </History>



See also
~~~~~~~~~~

    - :ref:`history`
    - :ref:`interaction`
