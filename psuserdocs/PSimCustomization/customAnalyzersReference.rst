.. _analysisFunctionReference:

Custom Analyzers Reference & Examples
--------------------------------------

This section will provide more details of what you might need to write your own analyze() function. In the :ref:`fileio` section below, several VsHdf5 class functions which you will be able to use to import, read, and write data if you inherit from the VpAnalyzer class (:ref:`Write a Class that Inherits from VpAnlayzer <custom-analyzer-tutorial-step2>`).

In :ref:`analysis-Examples` are examples of code you may find in the analyze() function.

.. _fileio:

How to Import, Read, and Write a Data Group with VsHdf5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The VpAnalyzer class uses VsHdf5 Python class functions for PSim file reading and writing. VsHdf5
ensures that metadata describing the type of datasets and other simulation information is
added to simulation data so that it can be correctly visualized in Composer. PSim dumps
simulation data in Hdf5 format with the extension ".h5". It is good practice to make analyzers
create files with the extension ".vsh5" in order to differentiate datasets that are simulation
results versus those that are analysis results. This will protect against accidentally deleting
simulation results that are not recoverable unless the simulation is re-run.

There are a number of convenient functions included in VpAnalyzer that handle VizSchema file
reading and writing, and these should be used in your analyze() function. There should not
be any need to import VsHdf5 directly into your analysis module, nor should you need to import
PyTables. There three types of functions. First, declaring Objects returns an empty object,
(either a group or dataset) with the specified name. This is useful if you are creating a
new dataset or group that will be written out to a file.

::

  def History(name=None)
  def Field(name=None)
  def Limits(name=None)
  def TimeGroup(name=None)
  def Mesh(name=None, kind='uniformCartesian')
  def RunInfo(name=None)
  def Group(name=None)
  def Dataset(name=None)

Second are functions for reading VsHdf5 objects from an existing file. This is useful for loading
in PSim dump data.

::

  def getHistory(fileName=None, historyName=None, name=None, location='/')
  def getField(fileName=None, fieldName=None, name=None, location='/')
  def getLimits(fileName=None, name=None)
  def getTimeGroup(fileName=None, name=None)
  def getMesh(fileName=None, name=None)
  def getRunInfo(fileName=None, name=None)
  def getGroup(fileName=None, groupName=None, name=None)
  def getDataset(fileName=None, datasetName=None, name=None)
  def getParticles(fileName=None, particlesName=None, name=None)

Finally, there are functions for writing VsHdf5 objects into a new or existing file.

::

  def writeHistory(fileName=None, historyName=None, data=None, meshName=None, overwrite=False, location='/')
  def writeField(fileName=None, data=None, fieldName=None, meshName=None, limitsName=None,    timeGroupName=None, offset='nodal', dumpTime=0.0, overwrite=False)
  def writeLimits(fileName=None, limitsName=None, overwrite=False)
  def writeTimeGroup(fileName=None, timeGroupName=None, dumpTime=None, dumpStep=None, overwrite=False)
  def writeMesh(fileName=None, mesh=None, overwrite=False)
  def writeRunInfo(fileName=None, runInfo=None, overwrite=False)
  def writeGroup(fileName=None, groupName=None, overwrite=False)
  def writeDataset(fileName=None, datasetName=None, overwrite=False)


.. _analysis-Examples:

Examples
~~~~~~~~~~~

Example 1: Create a 1D Data Set
`````````````````````````````````

Create a 1-D structured mesh and assign frequency values to that mesh's dataset. Then define
a History with values for each mesh point (frequency), and write that History to a file. Note that in this
example, "self.overwrite" with be True if the :samp:`--overwrite` or -w flag was passed as a command line argument,
or False otherwise. Similarly, :samp:`--simulationName` is a command line argument

::

  fileName = self.simulationName + '_SParameters.vsh5'
  mesh = self.Mesh(name='SParameters', kind='structured')
  mesh.assignDataset(Freq)
  self.writeMesh(fileName=fileName, mesh=mesh, overwrite=self.overwrite)

  pointDataHist = self.History(name='S11')
  pointDataHist.assignDataset(S11)
  pointDataHist.assignAttribute('vsCentering', 'nodal')
  pointDataHist.writeHistory(fileName=fileName, meshName=mesh.name, overwrite=self.overwrite)

Example 2: Read Time Step and Dump Periodicity 
`````````````````````````````````````````````````

Read a TimeGroup from a PSim dump file, and extract the simulation time and dump step
from that group. Note that all of the get functions return both the object, and the attributes of
that object.

::

  timeGroup, attrs = self.getTimeGroup(fileName=speciesFileName)
  time = timeGroup.getDumpTime()
  step = timeGroup.getDumpStep()

Example 3: Extract Data from a History
```````````````````````````````````````

Read a History from a PSim History dump file, and extract the data array from that
History. Note that "hist" is a VsHdf5 object, not the actual dataset. To extract the dataset
from the object, one can use the usual slicing operators, as in the example below, or by direct
reference to the dataset, that is an attribute of the object: "data = hist.dataset". data is
a possibly multi-dimensional numpy array.

::

  fileName = self.simulationName + '_History.h5'
  hist, attrs = self.getHistory(fileName=fileName, historyName=self.historyName)
  data = hist[:]
