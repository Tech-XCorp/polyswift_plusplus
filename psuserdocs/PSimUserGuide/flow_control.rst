
.. _programming-concepts-flow-control-and-repetition:

Flow Control and Repetition
-------------------------------

The PSim preprocessor includes both flow control and conditional
statements, similar to other scripting languages. These features allow
the user a great deal of flexibility when creating input files.



.. _programming-concepts-conditionals:

Conditionals
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A conditional takes either the form:

::

       $ if COND
             ...
       $ endif

       or

       $ if COND 
             ...
       $ else
             ...
       $ endif
    
    
Conditionals can be arbitrarily nested. All the tokens following the 
:samp:`if` token are interpreted following the expression evaluation 
procedure (see above) and if they evaluate to true, the text following 
the if statement is inserted into the output. If the conditional 
statement evaluates to false, the text after the :samp:`else` is 
inserted (if present). Note that :samp:`true` and :samp:`false` in 
preprocessor macros are evaluated by Python – in addition to evaluating 
conditional statements such as :samp:`x == 1`, other tokens can be 
evaluated. The most common use of this is using :samp:`0` for false and 
:samp:`1` for true. Empty strings are also evaluated to false. For more 
detailed information, consult the Python documentation. 



.. _programming-concepts-repetition:

Repetition
^^^^^^^^^^^^^^^^^

For repeated execution, txpp provides while loops; these take the form:

::

        $ while COND
            .
            .
            .
        $ endwhile
    
    
which repeatedly inserts the loop body into the output. For example, to
create 10 stacked circles using the circle macro from above, you could
use:

::

        $ n = 10
        $ while n > 0 circle(n)
        $   n = n - 1
        $ endwhile

