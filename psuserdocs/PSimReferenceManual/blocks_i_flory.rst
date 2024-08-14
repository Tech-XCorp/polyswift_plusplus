.. _flory:

Flory Interaction
------------------------------

:command:`flory`:
    kind of :command:`Interaction` specifies the Flory interaction between 
    density fields

    
flory Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`chi` (float):
    Flory interaction chi parameter

:option:`STFunc` (object):
    can hold an object for specifying chi parameters that vary in space and time

    
Example flory Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <Interaction StyrEthy>
       kind = flory
       chi = 0.12
       scfields = [totStyrDens totEthyDens]
    </Interaction>
 

See also
~~~~~~~~~~
    - :ref:`updater`
    - :ref:`physfields`
    - :ref:`stfunc`
