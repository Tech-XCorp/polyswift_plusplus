.. _vrm_introduction:

Overview
-----------------

The *PSim Reference Manual* is a quick-reference manual for PSim
users to look up specific features and code block syntax for the 
compuational engine, PolySwift++.  To learn about the complete PSim
simulation process, including details regarding input file format and
the PSim tutorials, or see examples of using PSim to simulate
real-world physics models, please refer to *PSim Quick Start* or 
*PSim In Depth*.

In the following section, we introduce input file basics that 
are helpful for understanding the descriptions of PSim input file  
syntax explained in the rest of the *PSim Reference Manual*. In the sections
following *Input File Basics*, we describe the various blocks permitted
in a PSim input file.  The general format of each block’s description is:

    - Block name
    - Summary of the block’s purpose
    - List of the block’s parameters
    - Example of the block as used in a PSim input file

We note whenever a block can or should contain another block. For the 
purpose of this document, a :samp:`kind` or a :samp:`function` can be 
considered a parameter of a block if the kind or function can be used to 
complete the  description of the block or modify block characteristics. 
   
Wherever possible, we describe all of the parameters in the same section 
as the block. While this requires that we repeat some information for 
different blocks that use the same parameters, you do not need to go 
elsewhere to find the rest of the block’s description. In some cases, 
where a block’s parameters are themselves blocks, we list the 
parameters’ names, however we completely describe the sub-block or 
block that is nested in its own section. 
