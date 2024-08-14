.. _florywall:

Flory Wall Interactions
------------------------------

:command:`floryWall`:
    kind of :command:`Interaction` specifies the Flory interaction between 
    density field and a wall

    
floryWall Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`chi` (float):
    Flory interaction chi parameter

:option:`wallField` (string):
    Name of field that corresponds to constraint field that is modeling a wall

    
Example floryWall Block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    <Interaction EthyWall>
       kind = floryWall
       chi = 0.01
       scfields = [totWall totEthyDens]
       wallField = totWall
    </Interaction>
 

See also
~~~~~~~~~~
    - :ref:`updater`
    - :ref:`physfields`
    - :ref:`flory`
