.. _programming-concepts-scoping:

Scoping Rules
----------------

Symbols in txpp are *scoped*. This means that the effect of a symbol’s
definition is confined to the macro or block in which that symbol is
defined. Whenever txpp enters a macro or a new input file block, it
enters a new scope.

In the case in which :samp:`SYMBOL` is defined in multiple scopes, txpp
ignores the previously defined :samp:`SYMBOL` for the duration of the current
scope. In the case in which :samp:`SYMBOL` is defined more than once in the
current scope, the new value overrides the previous value defined in the
current scope.

This scope is closed once txpp leaves the block or macro. That is, the
symbol’s definition no longer has an effect once txpp has used the
symbol’s value in the macro or block where it was defined and then
proceeded to a different block or macro. Scoping allows the next block
or macro to be free to redefine the value of the symbol for its own
purposes.

