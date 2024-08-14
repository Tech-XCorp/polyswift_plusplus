
.. _input-file-syntax:


Input File Syntax
-------------------

The most important part of the PSim simulation process, which we will
examine in :ref:`the-simulation-process`, is creating an input file. You
define the simulation and its variables in the input file, which has a
.pre suffix.

This section discusses the syntax used in pre files.

An input file consists of:

    - Comments
    - Variables
    - Top-level simulation parameters
    - Parameters and vectors of parameters organized into input blocks


Comments
^^^^^^^^^^^^

You can enter comments in either of two ways:

    - Following a pound sign (#) either on a new line or a continuation of
      a current line
    - Between the opening and closing comment tags <Comment> </Comment>

    .. note::
       Tech-X recommends that you always update your comments when you make
       changes to an input file. The reasoning behind a change may become 
       unclear if you do not provide comments that explain why you made the 
       change. Input files with old, out-of-date comments are difficult to 
       work with.


Variables
^^^^^^^^^^^^^^^^^

Each line defining a variable begins with a dollar sign ($).

.. _SimulationParameter:

Parameters
^^^^^^^^^^^^^^^^^

Parameters can be integers, floating-point numbers, or text strings.

The format of the parameter value determines the type of parameter. For
example:

    - x = 10 indicates an integer
    - x = 10.0 indicates a floating-point number
    - x = ten indicates a text string

Some parameters accept any text string (within reason). Other parameters
accept only a choice of text strings.

If PSim can parse a value as an integer, it will do so. For example:

::

    42

If PSim cannot parse the value as an integer, it will attempt to parse
it as a floating-point number – for example, any of the following:

::

    42.
    3.14159
    1.60217646e-19


If PSim cannot parse the value as either an integer or
a floating-point number, it will parse the value as a string of text,
for example, either of the following:

::

    4o. (4 and lowercase O) or
    4O (4 and uppercase O).


Use a decimal point to specify a floating point number. You must write
floating-point numbers with a decimal point so that PSim will not
interpret them as integers. If you want to assign an integer value to a
floating-point parameter, make sure you write it as :samp:`3`. (with a
decimal point :samp:`.`) rather than only the numeral :samp:`3` (without a decimal
point). If you write the number as an integer (without the decimal
point), PSim will interpret it as an integer. This will likely produce
unexpected results.

Check that you have correctly defined parameter values. If you
incorrectly define a parameter that has a default value, PSim will use
the default value and give you unsatisfactory results. If you
incorrectly define a parameter that does not have a default value,
the PolySwift++ engine may crash, fail to compute the physics of the 
simulation, or ignore the incorrectly defined parameter and give you 
unsatisfactory results.

Do not specify a parameter twice. If you do, PSim will use the second
occurrence of the parameter in the processed input file produced from the 
input file with the .pre extension. Although PSim allows you to specify 
parameters and input blocks in many different sequences, if you follow the 
recommendations in this user guide, you should not have a problem with 
specifying parameters twice.


Vectors of Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^

Vectors of parameters are enclosed by brackets :samp:`[ ]` with white space
used as separators. For example:

    - :samp:`x = [10 10 10]` indicates a vector of integers
    - :samp:`x = [10. 10. 10.]` indicates a vector of floats


Input Blocks
^^^^^^^^^^^^^^^^^

Input blocks are used to create simulation objects. The block is
enclosed by opening and closing tags such as:

::

    <Grid globalGrid>
      . 
      .
      . 
    </Grid>


The tag determines:

    - object type:
        indicated by an initial capital letter, for example, :samp:`Grid`

    - object name:
        indicated by an initial lowercase letter, for example, :samp:`globalGrid`

You use the object name to refer to the object in other input blocks.
For example, in the input block for a copolymer Block object, you may refer to
the name of another Block object in the same Polymer block. These references
are used in the 'headjoined' and 'tailjoined' parameters to specify the 
connectivity of the block copolymer.

Input blocks can be nested. For example, input blocks for Interaction and 
Updater objects are nested within the input block for an Hamiltonian model, 
EffHamil.

    .. note:
       Tech-X recommends that when you nest input blocks, use an
       appropriate amount of indentation to improve the readability of the 
       input file.
