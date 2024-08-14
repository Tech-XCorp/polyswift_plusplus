.. _developer-notes-polyswift:
    
Developer Notes: Refactoring PolySwift++ by Example
-----------------------------------------------------
    
Welcome to PolySwift++, the parallel, numerical self-consistent field
theory code for copolymer systems. PolySwift++ has algorithms that allow
simlations of confined block copolymers and nanocomposite mixtures that
uses a hybrid-SCFT method for incorporating particles in a block copolymer
matrix.

Polyswift++ is an object-oriented (OO) code that exploits many of the advanced
design principles of the C++ language. This OO design allows developers to
extended the capabilities of PolySwift++ without re-writing the entire code.
This *refactoring* procedure is documented below for a number of case studies.

    
Case: 1 Implementing Chompst Solvers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Create branch for new work. Check svn address, then use branches directory to
to make a development repo.
 
::

	polymer:polyswift swsides$ svn info
	Path: .
	URL: https://ice.txcorp.com/svnrepos/code/polyswift++/trunk
	Repository Root: https://ice.txcorp.com/svnrepos/code/polyswift++
	Repository UUID: 7af40706-f99d-470f-9261-b3b3ee8f4290
	Revision: 1626
	Node Kind: directory
	Schedule: normal
	Last Changed Author: swsides
	Last Changed Rev: 1626
	Last Changed Date: 2012-08-30 10:30:08 -0600 (Thu, 30 Aug 2012)

Then copy current trunk to a branch

::

	polymer:polyswift swsides$ svn copy https://ice.txcorp.com/svnrepos/code/polyswift++/trunk https://ice.txcorp.com/svnrepos/code/polyswift++/branches/ps-chompst-1

this brings up a commit message 

::

	Development branch for implementing Chompst Library

	--This line, and those below, will be ignored--

	A    https://ice.txcorp.com/svnrepos/code/polyswift++/branches/ps-chompst-1
