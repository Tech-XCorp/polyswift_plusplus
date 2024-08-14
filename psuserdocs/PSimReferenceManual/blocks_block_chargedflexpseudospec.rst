.. _chargedflexpseudospec:

Flexible-Charged Block Model
------------------------------

:command:`chargedFlexPseudoSpec`:
    kind of :command:`Block` that defines a flexible charged chain model
    and pseudospectral solution method. Note: not yet fully implemented.

    
chargedFlexPseudoSpec Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`za` (float):
    charged strength in polyelectrolyte models

:option:`alpha` (float):
    charge distribution parameter

:option:`chargefield` (string):
    object name of a PhysField that defines the charged field associated
    with block

    
Example chargedFlexPseudoSpec Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <Block blockA>
      kind = chargeFlexPseudoSpec
      scfield = totStyrDens
      chargefield = totChargeDens
      za = 0.4
      alpha = 0.02

      ds = 0.1
      lengthfrac = 0.5
      headjoined = [freeEnd]
      tailjoined = [blockB]
    </Block>


See also
~~~~~~~~~~
    - :ref:`physfields`
    - :ref:`polymer`
