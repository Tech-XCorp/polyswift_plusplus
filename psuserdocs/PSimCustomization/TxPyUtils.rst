.. _txpyutils-user-guide:

TxPyUtils User Guide
========================

TxPyUtils Introduction
-------------------------

TxPyUtils is a collection of Python classes providing middleware functionality. The most relevant
to users of PSim is the TxOptParse class, for handling the input to analysis scripts and the
interaction with the PSimComposer analysis tab.

Requirements
--------------
TxPyUtils has been developed and is tested using Python 2.7. It is known to work with Python 2.6.
It works in the embedded Python environment provided by the analysis tab. It depends on the
(now deprecated) optparse standard Python module and the readPre module. TxPyUtils in its current
form is not compatible with Python 3, due to the deprecation of optparse.

Usage
---------
TxPyUtils is included in PSimComposer's embedded Python environment. Outside this environment,
to use TxPyUtils, ensure TxPyUtils.py, optparse.py and readPre.py are in your PYTHONPATH.
Then simply use:

::

  import TxPyUtils

in your Python script.

Typical usage would involve the setting up of a list of options and passing them to the TxOptParse
class:

::

    options = []
    options.append(('-s','--simulationName','Base simulation name, i.e., the input \
                    file name without extension.', 'string', None, True))
    options.append(('-S','--speciesName', 'Name of the species to analyze', \
                    'string', None, True))
    options.append(('-T','--threshold','Particle densities below this will be \
                    ignored','float',None,False))
    ops = TxPyUtils.TxOptParse(desc=description, ops=options, hasOverwriteOption=True)

If you are using a script written for PSim 1.0 or before, please note that TxOptParse now expects
to be told if you do not wish to include an option in your analysis script to overwrite any previous
data that it finds with the same filename. You may then read the variables into your script as follows:

::

    baseName  = ops['simulationName']
    speciesName = ops['speciesName']
    threshold = float(ops['threshold'])
    overwrite = ops['overwrite']

For each argument we want to read from the command line or from input boxes in the analysis pane into its script,
we set up six options set up for TxOptParse as follows.

.. tabularcolumns:: |p{2.0cm}|p{3.2cm}|p{3.6cm}|p{4.8cm}|

+------------+-----------------------+------------------------------+-----------------------------------+
| parameter  | parameter purpose     | example                      | allowed values                    |
| number     |                       |                              |                                   |
+============+=======================+==============================+===================================+
| 1          | short argument name   | '-s'                         | string with a single alphanumeric |
|            |                       |                              | character following a hyphen      |
+------------+-----------------------+------------------------------+-----------------------------------+
| 2          | long argument name    | '--simulationName            | string starting with two hyphens, |
|            |                       |                              | no spaces and only alphanumeric   |
|            |                       |                              | characters                        |
+------------+-----------------------+------------------------------+-----------------------------------+
| 3          | description           | 'Base simulation name'       | any string delimited by single or |
|            |                       |                              | double quotes, without escape     |
|            |                       |                              | characters                        |
+------------+-----------------------+------------------------------+-----------------------------------+
| 4          | argument type, alows  | 'string'                     | only one of those values          |
|            | for limited parsing by| 'float'                      | mentioned to the left here        |
|            | our analysis pane     | 'int'                        |                                   |
+------------+-----------------------+------------------------------+-----------------------------------+
| 5          | default value         | 2, '_history', None          | See examples. Should be of the    |
|            |                       |                              | correct type                      |
+------------+-----------------------+------------------------------+-----------------------------------+
| 6          | is value required?    | True or False (ie Boolean)   | Only True or False                |
+------------+-----------------------+------------------------------+-----------------------------------+

Users are encouraged to check the scripts provided by Tech-X for further examples of usage.

This interface is likely to be replaced in future releases of PSim, so please use with care.

