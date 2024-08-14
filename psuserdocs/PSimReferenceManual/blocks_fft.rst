.. _fft:

FFT
---------------------

:command:`FFT`:

    blocks define interface for Fast-Fourier transform implementations.


FFT Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:option:`kind`:
    one of:

    :option:`normalfftw`:
        Implementation of FFT with the FFTW (Fastest Fourier Transform in 
	the West) library set for NORMAL result layout

    :option:`transposefftw`:
        Implementation of FFT with the FFTW (Fastest Fourier Transform in 
	the West) library set for TRANSPOSE result layout
