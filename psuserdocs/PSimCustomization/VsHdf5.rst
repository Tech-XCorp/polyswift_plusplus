.. _customization-vshdf5:

VsHdf5 User Guide
=================

VsHdf5 Introduction
-------------------

VsHdf5 is a collection of Python classes that provide an interface between
VizSchema and Hdf5. These classes make it easy to read and write
VizSchema-compliant files, which may be the result of simulation code
dumps or analysis scripts. VizSchema-compliant files may be visualized in
Composer, or other visualization software, such as VisIt.

Requirements
------------
VsHdf5 has been developed using Python 2.7. It works with Python 2.6 as well,
and probably Python 3.0 although that is currently not tested. VsHdf5 must
be able to import pytables and numpy. When importing VsHdf5, if these modules
cannot be loaded, an exception is thrown. VsHdf5 supports VizSchema
version 2.1.

Usage
-----
To use VsHdf5, first make sure that VsHdf5 (and numpy and pytables) are in
your PYTHONPATH. Then simply use:

::

  import VsHdf5

in your Python script.

Reading in VizSchema data from a file
-------------------------------------
To read an object from an existing VizSchema-compliant file in your Python
script, simply create the object and pass in the name of the file and the
name of the object in the file:

::

  obj = VsHdf5.Object(fileName=filename, name=objectname)

where Object could be one of:

- Dataset

  - Field
  - History
  - Particles

- Group

  - RunInfo
  - TimeGroup
  - Limits


- Mesh

  - StructuredMesh
  - UnstructuredMesh
  - UniformCartesianMesh
  - RectilinearMesh


So for instance, to read particles from a Vorpal dump file from a species
called electrons, you could write:

::

  e = VsHdf5.Particles(fileName='baseName_electrons_1.h5', name='electrons')

This will create an object called 'e' in Python, that contains both the
data in the dataset 'electrons' in the file 'baseName_electrons_1.h5',
as well as the attributes associated with that data. The data can be
accessed as

::

  e.dataset

or

::

  e[:]

both of which are a numpy array, and can be sliced and manipulated with the
usual numpy operators. You can get a list of the attributes that were
stored in the dataset in the file by invoking

::

  attrs = e.attributeList

which will return a dictionary with key/value pairs representing the
attributes.

For objects that do not have datasets (those that are Hdf5 groups with
only attributes in the file), then dataset = None.

Optionally, you can
specify a location in the Hdf5 file from which to read the data or group:

::

  e = VsHdf5.Particles(fileName='baseName_electrons_1.h5', name='electrons', location = '/fluids')

If not specified, the default location of 'root' is used ('/').

You can
also read in a dataset and attributes directly after the object has been
created:

::

  e = VsHdf5.Particles()
  array, attrs = e.readParticles('baseName_electrons_1.h5', 'electrons')

In this invocation, array will contain the numpy dataset, and attrs will
contain the dictionary of attributes.

Certain objects often only occur once in a given file, such as Limits,
Meshes, and RunInfo objects. For these types of objects, you can read
from a file without specifying the name of the group or dataset that you
want to read. VsHdf5 will search through the file until it finds a group
or object of the correct type, and read it in for you. So you could read
the (single) Limits group in a field file by invoking:

::

  l=VsHdf5.Limits(fileName='gamma2D01_universe_0.h5')

The name of the Limits group in the file is contained in the data member

::

  l.name

Required Attributes
~~~~~~~~~~~~~~~~~~~

Some objects require certain attributes to be VizSchema compliant.
Typically, if you read an object from a simulation dump file, then all of
the required attributes will be there. You can change individual attribute
by using the methods

::

  object.removeAttribute(attName)
  object.assignAttribute(attName, attValue)

There are additional optional attributes for VizSchema. See the VizSchema
documentation and VsHdf5.py docstrings for more details. The following
attributes are required for the objects below. In some cases, there are
default values.

Meshes (Hdf5 Group or Dataset):
+++++++++++++++++++++++++++++++

Meshes are important types of objects for VizSchema, and deserve more
attention here. Meshes may be datasets or groups in a Hdf5 file, depending
on the kind of mesh. Currently, VsHdf5 supports reading and writing of mesh
objects called StructuredMesh, UnstructuredMesh, UniformCartesianMesh, and
RectilinearMesh. Each of these kinds require different attributes (see below).
There is also a generic mesh object that makes it easier to read a mesh
from a file. If you invoke:

::

  m = VsHdf5.Mesh(fileName='gamma2D01_nodalE_0.h5')

for instance, then VsHdf5 will search for the (single) group or dataset
that is a mesh in the file, will determine the kind of mesh, and will cast
itself into that particular kind. So here, for instance, since the gamma2D01
simulation was in Cylindrical coordinates (ZR), we find that the mesh
object is a RectilinearMesh.

::

  In [5]: m = VsHdf5.Mesh(fileName='gamma2D01_nodalE_0.h5')
  In [6]: m
  Out[6]: <VsHdf5.RectilinearMesh instance at 0x104b88170>

Rectilinear Meshes (Hdf5 Group)
```````````````````````````````

:command:`meshName`
  Name of the group to be written in the file. Default = objectName
:command:`axis0data`
  Dataset representing the coordinates of the mesh in direction 0
:command:`axis1data`
  Dataset representing the coordinates of the mesh in direction 1. Required if simulation is 2-Dimensional.
:command:`axis2data`
  Dataset representing the coordinates of the mesh in direction 2. Required if simulation is 3-Dimensional.
:command:`axis0Name`
  Name of the dataset corresponding to the first axis. Default = axis0
:command:`axis1Name`
  Name of the dataset corresponding to the second axis. Default = axis1
:command:`axis2Name`
  Name of the dataset corresponding to the third axis. Default = axis2
:command:`limits`
  Name of a limits group that gives the spatial limits of the simulation

UniformCartesian Meshes (Hdf5 Group)
````````````````````````````````````

:command:`meshName`
  Name of the group to be written in the file. Default = objectName
:command:`lowerBounds`
  Array of physical coordinates representing the left, bottom, back corner of the simulation domain
:command:`upperBounds`
  Array of physical coordinates representing the right, top, front corner of the simulation domain
:command:`numCells`
  Array of integers giving the number of computational cells in each direction
:command:`startCell`
  Cell index of lowerBounds, default = [0,0,0]

Structured Meshes (Hdf5 Dataset)
````````````````````````````````

:command:`meshName`
  Name of the group to be written in the file. Default = objectName

Unstructured Meshes (Hdf5 Group)
````````````````````````````````

:command:`meshName`
  Name of the group to be written in the file. Default = objectName
:command:`pointsData`
  Dataset of physical coordinates of vertexes of the mesh (reals)
:command:`edgesData`
  Dataset of connectivity between vertexes of the mesh (ints)
:command:`facesData`
  Dataset of connectivity between vertexes of the mesh (ints)
:command:`polygonsData`
  Dataset of connectivity between vertexes of the mesh (ints)
:command:`pointsName`
  Name of the dataset containing vertex coordinates. Default = "points"
:command:`polygonsName`
  Name of the dataset containing vertex connectivity. Default = "polygons"

.. note::

   Only one of edgesData, facesData, or polygonsData is needed. pointsData is always required.


Fields (Hdf5 Dataset)
~~~~~~~~~~~~~~~~~~~~~

:command:`fieldName`
  Name of the dataset to be written in the file. Default = objectName
:command:`mesh`
  Name of a mesh group that the field is defined upon
:command:`limits`
  Name of a limits group that gives the spatial limits of the simulation
:command:`timeGroup`
  Name of a time group that indicates the simulation time and step
:command:`offset`
  Centering of the data. default = 'none'
:command:`dumpTime`
  Simulation time when the data was dumped. default = 0.0

Histories (Hdf5 Dataset)
~~~~~~~~~~~~~~~~~~~~~~~~

:command:`historyName`
  Name of the dataset to be written in the file. Default = objectName
:command:`mesh`
  Name of a mesh group that the field is defined upon, typically a 1-Dimensional time series

Particles (Hdf5 Dataset)
~~~~~~~~~~~~~~~~~~~~~~~~

:command:`particlesName`
  Name of the dataset to be written in the file. Default = objectName
:command:`charge`
  The charge of the particle species
:command:`mass`
  The mass of the particle species
:command:`numPtclsInMacro`
  The number of physical particles per simulation particle
:command:`limits`
  Name of a limits group that gives the spatial limits of the simulation
:command:`timeGroup`
  Name of a time group that indicates the simulation time and step
:command:`numSpatialDims`
  Number of spatial dimensions. default = 3
:command:`vsNumSpatialDims`
  Number of spatial dimensions. default = 3
:command:`dumpTime`
  Simulation time when the data was dumped. default = 0.0

RunInfo (Hdf5 Group)
~~~~~~~~~~~~~~~~~~~~

:command:`runInfoName`
  Name of the group to be written in the file. Default = objectName


TimeGroups (Hdf5 Group)
~~~~~~~~~~~~~~~~~~~~~~~

:command:`timeGroupName`
  Name of the group to be written in the file. Default = objectName
:command:`dumpTime`
  Simulation time when the data was dumped.


Reading in an entire file
-------------------------

It is also possible to parse an entire VizSchema-compliant file. To do
this, invoke:

::

  f = VsHdf5.VsFileReader(fileName=fileName)

This will create an object that contains all of the datasets and groups
in the file that have VizSchema attributes. This does not guarantee that
the file is VizSchema compliant however. In particular,

::

  f.objectDict

is a Python dictionary with key equal to the name of the object (Field,
Mesh, etc.) and value equal to a VsHdf5 object of the appropriate type.
As with reading in individual objects, the objects in the dictionary will
contain both datasets and attributes as appropriate to the type of object.

Now you can read in a group by calling it's name explicitly

::

  electrons = f.objectDict['electrons']
  limits = f.objectDict['globalGridGlobalLimits']


External links
---------------

VsHdf5 supports reading and writing of external links from/to files.

To write a link to an external file, use:

::

  object.writeExternalLink(fileName, name, target, location='/')

:command:`fileName`
  is the name of the file to write the link into
:command:`name`
  is the name of the link in that file
:command:`target`
  is the name of the external file that contains the actual dataset or group
:command:`Location`
  is an optional parameter that specifies the location of the link "name"
  in the file "fileName", in case this is not the root group.

"object" is any VsHdf5 class

Note that the target file may or may not exist. So for instance, if you invoked:

::

  mesh.writeExternalLink('outVsHdf5.vsh5', 'goodLink', 'ww02_nodalExternalB_99.h5', '/globalGridGlobal')

This would create an external link in the file outVsHdf5.vsh5 called
goodLink, that would point to the group /globalGridGlobal in the file
ww02_nodalExternalB_99.h5. Subsequently, accessing the Hdf5 node goodLink
would be as if the group /globalGridGlobal was actually in the file
outVsHdf5.vsh5.

To read an external link from a file, simply read it as if it were a
group or dataset object in the target file, e.g.

::

  emesh = VsHdf5.Mesh(fileName='outVsHdf5.vsh5', name='goodLink')

will transparently dereference the external link called "goodLink" in the
file "outVsHdf5.vsh5", and create the VsHdf5 Mesh object (appropriately
cast to the proper type) as if the mesh group was in the file
"outVsHdf5.vsh5". If the link is broken, namely if it points to a file with
a fully qualified path that does not exist, then VsHdf5 will look for a
file with the same name in the current directory. If that file in turn does
not exist, or the group or dataset can not be read from an existing target
file, then an error is reported and the object is not read.


Special Accessor methods
------------------------

Some attributes are commonly used in analysis scripts, and so they have
their own accessor methods. The value of any attribute can be found by

::

  att = object.attribute(key)

where key is the name of the attribute. If the attribute does not exist in
the object, then att = None.

For most objects (dataset and group types), where applicable:

::

  object.getType(): return the value of 'vsType'.
  object.getKind(): return the value of 'vsKind'.
  object.getLowerBounds(): return the value of 'vsLowerBounds'.
  object.getUpperBounds(): return the value of 'vsUpperBounds'.
  object.getNumCells(): return the value of 'vsNumCells'.
  object.getStartCell(): return the value of 'vsStartCell'.
  object.getCentering(): return the value of 'vsCentering'.
  object.getDumpTime(): return the value of 'time'.

For TimeGroup objects:

::

  object.getTime(): return the value of 'vsTime'.
  object.getDumpStep(): return the value of 'vsStep'.

For Particles objects:

::

  object.getCharge(): return the value of 'charge'.
  object.getMass(): return the value of 'mass'.
  object.getNumPtclsInMacro(): return the value of 'numPtclsInMacro'.
  object.getNumSpatialDims(): return the value of 'vsNumSpatialDims'.


Writing a VizSchema-compliant file from data
--------------------------------------------

To write VizSchema-compliant data into a file, simply call the VsHdf5
objects write method, such as:

::

  e.writeParticles(filename)

Similarly for fields (writeField), meshes (writeMesh), etc. See below.

Some objects require that certain attributes be defined. In this case, if
you try to write the object to a file without defining those attributes,
then a warning will be written to stdout and the object will not be written
to a file. Similarly, if a dataset or group with the same name as the
object to be written already exists in the file, it will not be
overridden, and a warning will be printed.

If you have created a dataset that you would like to write into a file in
a VizSchema-compliant fashion, you can do this by constructing the needed
objects by hand (as opposed to reading them in from an existing file).
Consider the following example:

::

  lb=numpy.double(0.)
  ub=numpy.double(1.)
  nc=numpy.int(100)
  ts = VsHdf5.UniformCartesianMesh(name='timeSeries')
  ts.writeMesh('outputFile.vsh5', lowerBounds=lb, upperBounds=ub, numCells=nc)

  array = numpy.zeros(100)
  d = VsHdf5.History(name='myHistory')
  d.assignDataset(array)
  #[add any required attributes, e.g.]
  d.assignAttribute('vsType','variable')
  d.assignAttribute('vsMesh','timeSeries')
  d.writeHistory('outputFile.vsh5')

This will create a file called 'outputFile.vsh5' with a 10x10 dataset
called 'myHistory', with the attributes vsType=variable and vsMesh=timeSeries.
Notice that we first created a mesh object that is the 1-Dimensional
time series on which the history is defined. Also note that arrays and
scalar numerical values that are passed to write methods must be numpy
objects, not Python lists.


Example of Writing a VizSchema-compliant file from existing data
----------------------------------------------------------------

::

  import VsHdf5

  # read in the file
  f=VsHdf5.VsFileReader(fileName='ECDriftQuad_electrons_1.h5')

  #print file contents
  print f.objectDict

  electrons = f.objectDict['electrons']
  limits = f.objectDict['globalGridBlobalLimits']
  runInfo = f.objectDict['runInfo']
  timeGroup = f.objectDict['time']

  #print time group attributes
  print timeGroup.attributeList

  #print the dump time and dump step from the time group
  print timeGroup.getDumpTime()
  print timeGroup.getDumpStep()

  #overwrite the attribute "vsStep" with a new value of 1
  timeGroup.assignAttribute('vsStep',1)

  #overwrite the attribute "vsTime" with a new value of 1e-1
  timeGroup.assignAttribute('vsTime', 1e-1)

  #print new time group attributes
  print timeGroup.attributeList

  #give the name of the new output file
  of = 'outfile.vsh5'

  #write the new timeGroup data to a file and copy the rest of the file as old
  electrons.writeParticles(of,timeGroup='time')
  runInfo.writeRunInfo(of)
  limits.writeLimits(of)
  timeGroup.writeTimeGroup(of)
