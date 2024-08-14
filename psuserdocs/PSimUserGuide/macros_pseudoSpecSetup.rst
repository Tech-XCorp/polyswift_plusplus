.. _programming-concepts-polyswift-macros-pssetup-macro:

Setup for Pseudospectral Solver Macro
----------------------------------------

:command:`pseudoSpecSetup`:
    The pseudo-spectral Setup macro is used with the flexPseudoSpec models
    for solving the flexible Gaussian chain models for block copolymers.
    The :file:`pseudoSpecSetup.mac` macro is used with all current PSim
    input files.

    
.. _programming-concepts-polyswift-macros-pssetup-macro-adiupdaters:
 
setupPS
^^^^^^^^^^^^^^

:command:`setupPS` (nx, ny, nz, dx, dy, dz, debug):

    The FFTW implementation of the pseudo-spectral method
    requires a transposed grid to save communication time.
    This macro sets up the unifrom Cartesian grids and the
    transposed grid with the appropriate decompositions
    The communication block is the MPI required by FFTW
    and the FFTW objects assoicated with the correct grid
    and decomp objects are created as well.

    - nx: number of cells in x-direction
    - ny: number of cells in y-direction
    - nz: number of cells in z-direction

    - dx: size of each cell in x-direction [radius of gyration]
    - dy: size of each cell in y-direction [radius of gyration]
    - dz: size of each cell in z-direction [radius of gyration]

    - debug: one of 'on' or 'off'.  Turn on or off debugging statements
