.. _programming-concepts-using-macros-in-input-files:

Using Macros in Input Files
-----------------------------

A macro is a mechanism to abstract complex input files sequences into
(parameterized) tokens. In its simplest form, a macro provides a way to
substitute a code snippet from an input file:

::

    <macro snippet>
        line1
        line2
        line3
     </macro>
    
    
In this example, every occurrence of the code named snippet in the input
file will now be replaced by the three lines defined between the
:samp:`<macro>` and :samp:`</macro>` tags.



.. _macro-parameters:

Macro Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Macros can take parameters, allowing variables to be passed into and
used by the macro. Parameters are listed in parentheses after the macro
name in the macro declaration.
Once a macro is defined, it can be used by calling it and providing
values or symbols for the parameters. The macro will substitute the
parameter values into the body provided.
    
.. note::
   The parameter substitution happens in the scope of the caller. 
   Parameters do not have scope outside of the macro in which they are 
   defined.


.. _macro-overloading:

Macro Overloading
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As with symbols, macros can be overloaded within a scope. The particular
instance of a macro that is used is determined by the number of
parameters provided at the time of instantiation. This enables the user
to write macros with different levels of parameterization:

::

       <macro circle(x0, y0, r)>
            r^2 - ((x-x0)^2 + (y-y0)^2)
       </macro>

       <macro circle(r)>
            circle(0, 0, r)
       </macro>
    
    
Looking in the example above, whenever the macro circle is used with a
single parameter, it creates a circle around the origin; if you use the
macro with 3 parameters, you can specify the center of the circle.

The macro substitution does not occur until the macro instantiation is
actually made. This means that you do not have to define the 3-parameter
circle prior to defining the 1-parameter circle, even though the
1-parameter circle refers to the 3-parameter circle. It is only
necessary that the first time the 1-parameter circle is instantiated,
that 3-parameter circle has already been defined, otherwise you will
receive an error.


.. _programming-concepts-defining-functions-using-macros:

Defining Functions Using Macros
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Macros can be particularly useful for defining complex mathematical
expressions, such as defining functions in STFunc blocks with 
:samp:`kind = expression`.

Consider a macro that should simplify the setup of a Gaussian. One could
define the following macro:

::

       <macro badGauss(A, x, sigma)>
            A * exp(-x^2/sigma)
       </macro>
    
    
While this is a legitimate macro, an instantiation of the macro via:

::

     badGauss(A0+5, x-3, 2*sigma)
    
    
will result in:

::

     A0+5*exp(-x+3^3/2*sigma)
    
    
which is probably not the expected result. One alternative is to put
parentheses around the parameters whenever they are used in the macro.

::

       <macro betterGauss(A, x, sigma)>
            ((A) * exp(-(x)^2/(sigma)))
       </macro>
    
    
This will ensure that the expressions in parameters will not cause any
unexpected side effects. The downside of this approach, however, is that
the macro text is hard to read due to all the parentheses. To overcome
this issue, txpp provides a mechanism to automatically introduce the
parentheses around arguments by using a function block

::

       <function goodGauss(A, x, sigma)>
            A * exp(-x^2/sigma)
       </function>
    
    
The previous example will produce the same output as the badGauss macro,
but without requiring the additional parentheses in the macro text.


.. _programming-concepts-importing-files:

Importing Files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Txpp allows input files to be split into individual files, thus enabling
macros to be encapsulated into separate libraries. For example, physical
constant definitions or commonly-used geometry setups can be stored in
files that can then be used by many PSim simulations. Input files can
be nested to arbitrary depth.

Files are imported via the import keyword:

::

        $ import FILENAME
    
    
where FILENAME represents the name of the file to be included. txpp
applies the standard rules for token substitution to any tokens after
the import token. Quotes around the filename are optional and, computed
filenames are possible.





.. _programming-concepts-recursion:

Recursion
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Macros can be called recursively. E.g. the following computes the
Fibonacci numbers:

::

        <macro fib(a)>
            $ if a < 2
                a
            $ else 
                fib(a-1)+fib(a-2)
            $ endif
        </macro>
        fib(7)
    
    
.. note::
    There is nothing preventing you from creating infinitely recursive 
    macros; if terminal conditions are not given for the recursion, infinite 
    loops can occur. 



.. _programming-concepts-requires:

'Requires' Directive
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When writing reusable macros, best practices compel macro authors to
help ensure that the user can be prevented from making obvious mistakes.
One such mechanism is the requires directive, which terminates
translation if one or more symbols are not defined at the time. This
allows users to write macros that depend on symbols that are not passed
as parameters. For example, the following code snippet will not be
processed if the symbol :samp:`NDIM` has not been previously defined:

::

        <macro circle(r)>
            $requires NDIM
            $if NDIM == 2 r^2 - x^2 - y^2
            $endif
            $if NDIM == 3 r^2 - x^2 - y^2 - z^2
            $endif
        </macro>
    
    
.. _programming-concepts-string-concatenation:

String Concatenation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

One task that is encountered often when creating PSim simulations is
naming groups of similar blocks, e.g. similar species. Macros can allow
us to concatenate strings to make this process more clean and simple.
However, based on the white-spacing rules, strings will always be
concatenated with a space between them. For example,

::

        $a = hello
        $b = world
        a b


will result in

::

        hello world
    
    
However, we can get around this rule to get the desired output with the
following:

::

        <macro  concat(a, b)>
            $ tmp = 'a tmp b'
        </macro>
    
    
Now when calling

::

       concat(hello, world)
    
    
the result will be:

::

     helloworld
    
    
The first line appends a single quote to :samp:`a` and stores the result 
in :samp:`tmp`. The next line then puts the token :samp:`a` together 
with the token :samp:`b`. As they are now no longer two strings; they 
will be concatenated without a space. The final evaluation of the 
resulting string then removes the quotes around it, resulting in the 
desired output. 
