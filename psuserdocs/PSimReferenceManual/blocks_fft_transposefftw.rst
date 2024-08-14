.. _transposefftw:

FFTW (transpose decomposition)
------------------------------

:command:`transposefftw`:
    kind of :command:`FFT` that defines a transpose plan layout for the FFTW
    package. The plan layout determines how the k-space results are organized
    on the input simulation grid. The tranpose type returns results for a slab
    decomposition along the y-direction for Cartesian grids.

    
transposefftw Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

See also
~~~~~~~~~~

    - :ref:`decomp`
    - :ref:`grid`
