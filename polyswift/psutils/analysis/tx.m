(************************************************************
 *
 * tx.m
 *
 * A collection of useful modules and definitions
 * This package is automatically loaded by placing in the
 * /usr/local/mathematica/Configuration/Kernel/init.m file
 *
 * General use modules for calculation and data I/O
 *
 * Author  - Scott W. Sides
 *           Tech-X - Boulder, CO
 * 
 * Updating and cleaning               - March 2011
 * Altering for use with the new 7.0   - ~01 July  2009
 * Altering for use with the new 6.0   - 15 June  2007
 * Revised for use at Sandia on sides1 - 5 November 1998
 ***********************************************************)


helpTx::usage="
 List of homemade modules in \"tx.m\"
 
    memuse
    rdist
    getColumns
    dropColumns
    cullList
    readFile
    writeFile
    writeLines
    readH5File
    directionCosines
    generateLineOut

 For more information type \"? command \" for help on \"command\" "

Clear[memuse, rdist, getColumns, cullList, readFile];
Clear[writeFile, writeLines, readH5File];

memuse::usage="memuse Prints out present memory usage in MB"
memuse := Print[N[MemoryInUse[]/1000000],"  MB in use"];

rdist::usage="rdist[ r1, r2 ]
   Returns distance between two points r1 and r2 in Cartesian space"
rdist[r1_List, r2_List] :=
 Module[
  {delr = 0.0}, delr = r2 - r1; Norm[delr]
  ]

cullList::usage="cullList[ list, spacer ]
  Trims list to small size by taking out every 'spacer' number of elements"
cullList[dat_List, spacer_Integer] :=
    Module[
      {tmp, len},
      len = Length[dat];
      tmp = Take[dat, {1, len, spacer}]
      ];


readFile::usage="readFile[ \"filename\", skiplines ]
   Opens an input stream and skips a specified number of header lines.
   After skipping the header, the numerical data is read,
   one line per element in the input list."
readFile[datafile_String,skip_]:=
  Module[{datlist=temp=temp2={},columns=slen=i=0},
	datstr = OpenRead[datafile];
		
	Skip[datstr,String,skip];
	col1 = Read[datstr,String];
	slen = StringLength[col1]+2;
        temp  = StringInsert[col1," ",1];
        temp2 = StringInsert[temp," ",slen];
        temp = StringPosition[temp2," "];
	temp2 =Transpose[temp][[1]];
	temp = Partition[temp2,2,1];
        temp2 = Select[temp,(Part[#,2]-Part[#,1] != 1)&];
        columns = Length[temp2];
	SetStreamPosition[datstr,0];
		
	Skip[datstr,String,skip];
        (* datlist = ReadList[datstr,Table[Number,{i,1,columns}]]; *)
        datlist = ReadList[datstr,Table[Number,{i,1,columns}]];
	Close[datstr];
        Clear[col1];
	datlist
	]


writeLines::usage = "writeLines[ \"filename\", lineList ]
  Opens an input stream and writes each string line in lineList
  on a single line of the file then puts a carriage return."

writeLines[datafile_String, lineList_List] :=
  Module[
    {len, n = 0, linestr, datstr, str},
    
    len = Length[lineList];
    datstr = OpenWrite[datafile];
    
    Do[
      linestr = lineList[[n]];
      str = StringJoin[linestr, "\n"];
      Print[str];
      WriteString[datstr, str],
      {n, 1, len}
      ];
    
    Close[datstr];
    
    Print[len, " lines written to file  ", datafile]
    ]


writeFile::usage = "writeFile[\"filename\", dataList, appendFlag]
  Opens an input stream and writes each element in dataList
  on a single line of the file then puts a carriage return.
  appendFlag = 0 opens new file,
  appendFlag = 1 opens old file,
Multiple elements on 2nd level of list are put in separate columns."

writeFile[datafile_String, datlist_List, appendFlag_Integer] :=
  Module[
    {str, datstr, a, temp = coord = coordstr = {},
     columns = slen = n = 0},
    
    sform[a_] := ToString[StringForm["   ``  ",
                   Sign[a]*AccountingForm[Abs[a]]      ]];
    slen = Length[datlist];
    columns = Length[First[datlist]];
    
    If[appendFlag == 0,
      datstr = OpenWrite[datafile],
      datstr = OpenAppend[datafile]
      ];
    
    Do[
      coord = datlist[[n]];
      coordstr = Map[sform[#] &, coord];
      str = StringJoin[coordstr, "\n"];
      WriteString[datstr, str],
      {n, 1, slen}
      ];
    
    Close[datstr];
    Print["Data file written with ", columns, " columns"]
]


getColumns::usage="getColumns[ object, {column1,column2,....} ]
  Reads a list of multi-column data and separates into specified
  columns: column1, column2,... is a list of integers."

getColumns[datfile_List,clist_List]:=
  Module[{trans = {},i=len=0,list={}},
	trans = Transpose[datfile];
	len = Length[clist];

        If[len > 1,

	  Do[col[i]=trans[[clist[[i]]]],{i,1,len}];
	  list = Transpose[Table[col[i],{i,1,len}]];
	  Clear[col],

          list = trans[[clist[[1]]]]

         ];

        list
        ]

dropColumns::usage="dropColumns[ object, {column1, column2,... }]
  Reads a list of multi-column data and drops specified
  columns: column1, column2,... is a list of integers"

dropColumns[datfile_List,droplist_List]:=
  Module[{trans = {},i=len=0,allcolumns=list={},numcolumns,droplen},
	trans = Transpose[datfile];
        droplen = Length[droplist];
        numcolumns = Length[trans];
        allcolumns = Table[i,{i,1,numcolumns}];
        clist = Complement[allcolumns,droplist];
	len = Length[clist];

        If[droplen >= numcolumns,
       
          Print["Dropping equal or greater number of total columns"],

          Do[col[i]=trans[[clist[[i]]]],{i,1,len}];
	  list = Transpose[Table[col[i],{i,1,len}]];
	  Clear[col]

         ];

        list
        ]
        
        
readH5File::usage="readH5File[ fileName, Optional( dsetName, cellSize_List)]
  Reads HDF5 file data and parses into list of form { {x1,y1,z1, f1},{x2,y2,z2,f2}...}
  If string dsetName is specified then a dataset with that name in
  root directory is input. If cellSize list is specified then the
  coordinate data is scaled by these values. Note: if the optional
  arguments are not given, defaults are set."

readH5File[fileName_String, dsetName_String: "MonomerDensity", 
           cellSizes_List:{} ]:=
 Module[
  {
   inputParams,
   h5Info, h5Anno, h5Func, h5Flat, h5datAll, fnames,
   tst = {},
   dx, dy, dz,
   lastDat
   },
  
  inputParams = {"Datasets", StringJoin["/", dsetName]};
  Print["Working in directory ", Directory[]];
  
  fnames = FileNames[];
  If[MemberQ[fnames, fileName],
   
      Print["Found datafile ", fileName];
      h5Info = Import[fileName, {"HDF5", "Elements"}];
      h5Anno = Import[fileName, "Annotations"],
      Return[Print["Datafile ", fileName, " not found"]]
   ];
  
  (* If GridSizes found.. it replaces defaults *)
  gFound = Check[
      tst = Import[fileName, {"Datasets","GridSizes"}];
      True,
      False
      ];
  If[gFound,
     Print["GridSizes dataset found"];
     dr = Flatten[tst],
     dr = cellSizes;
     Print["No GridSizes dataset found, using default, or argument"]
    ];

  Print["cellSizes = ",dr];
    
  h5datAll = Import[fileName, inputParams];
  h5Func[x_, y_, z_] := h5datAll[[x]][[y]][[z]] [[1]];
  dims = Dimensions[h5datAll];
  
  If[ Length[dims] != 4, 
   Return[Print["Format of data not supported"]] ];
  If[ dims[[4]] != 1,       
   Return[Print["Non-scalar data not supported"]] ];
  
  dx = dr[[1]]; dy = dr[[2]]; dz = dr[[3]];
  Nx = dims[[1]]; Ny = dims[[2]]; Nz = dims[[3]];
  
  (* Generate list of coordinates (more efficient than using Append[] *)
  Print["   ... processing data"];
  tmp = Table[{dx*x, dy*y, dz*z, h5Func[x, y, z]},
    {x, 1, Nx}, {y, 1, Ny}, {z, 1, Nz}];
  
  (* Format table to flat list {{x,y,z,f},.... *)
  h5Dat = Flatten[tmp, 2];
                      
  Print["Dataset ", dsetName, " in h5Dat "];

  (* Kludge for 2D data format *)
  lastDat = Last[h5Dat];
  If [ lastDat[[3]] == dz,
     Print["Assuming 2D data"];
     tmp = getColumns[h5Dat,{1,2,4}];
     h5Dat = tmp;
     Print["  in format: {{x1,y1,f1},{x2,y2,f2}...}"],
                       
     Print["  in format: {{x1,y1,z1,f1},{x2,y2,z2,f2}...}"]
  ]

 ]


directionCosines::usage = "directionCosines[ r1_List, r2_List ]
    Calculates direction cosines for a line drawn through the points in 2D/3D
    r1 and r2. Also generates a parametric equation for a line using
    these direction cosines. Generates a function d3d[ ] that calculates Euclidian
    distance for a point from this line. Also generates a function rrNorm
    for use in lineout plots \n"

directionCosines[r1_List, r2_List] :=
  Module[
   {
    delr = 0.0,
    dist = 0.0, dCos = {}
    },
   
   Clear[paraLineEqn];
   
   (* Direction cosines *)
   delr = r2 - r1;
   dist = rdist[r1, r2];
   dCos = delr/dist;
   
   (* Use dir cosines for line equations *)
   
   paraLineEqn[t_] := r1 + dCos*t;
   
   (* Kludge for 2D/3D *)
   If[Length[r1] < 3,
    d3d[r0_List] := 
     Norm[Cross[Append[r0, 0] - Append[r1, 0], 
       Append[r0, 0] - Append[r2, 0]]]/
     Norm[Append[r2, 0] - Append[r1, 0]],
    d3d[r0_List] := Norm[Cross[r0 - r1, r0 - r2]]/Norm[r2 - r1];
    ];
   
   Clear[rrNorm];
   rrNorm[dat_List] :=
    Module[
     {dir, r0, dd},
     
     (* Parse data --> assumes {x,y,[z], f} *)
     
     r0 = Drop[dat, -1];
     dens = Last[dat];
     
     (* Get distance from center point and direction of unit vec *)
  
        dir = Sign[Normalize[r0 - r1].Normalize[r2 - r1]];
     dd = dir*rdist[r0, r1];
     {dd, dens}
     ];
   
   Print["Parametric line eqn --> paraLineEqn[t]"];
   Print["Distance of r0 from line defined by r1,r2 --> d3d[r0]"];
   Print["Map (r0,f} to {dist to r0 along paraLineEqn,f} ---> \
rrNorm[{r0,f}]"]
   ];

   

generateLineOut::usage = "generateLineOut[ dat_List, linePts_List, outName_String]
    Takes a data list of form { {r0, f0}, {r1, f1}.... } where r0 ...
    can be 2D or 3D and generates line out data for a line specified ...
    by the two points (2D/3D) in linePts list. The lineout data will ...
    be in list named outName. This data will be in form
    {{xx0, f0}, {xx1, f1}....} where xx is distance along the defining line.
    NOTE: need the method directionCosines[] \n"

generateLineOut[dat_List, linePts_List, outName_String] :=
 Module[
  {
   threshold = 0.05, pt1 = pt2 = {}
   },
  
  pt1 = linePts[[1]];
  pt2 = linePts[[2]];
  
  (* Generates the paraLine, d3d and rrNorm functions *)
  
  Print["Running module directionCosines[] to obtain solid geometry \
info/functions"];
  directionCosines[pt1, pt2];
  
  (* Select raw lineout data *)
  
  Print["\n ... selecting raw linout data"];
  tmpD = Select[dat, (d3d[Drop[#, -1]] < threshold) &];
  
  (* Map linout data to r vs \[Phi] *)
  
  Print[" ... converting raw lineout to dist vs f "];
  lineOut[outName] = Map[rrNorm[#] &, tmpD];
  
  Print["\n Lineout data {{r1, f1},{r2, f2}....} in lineOut[ ", 
   outName, " ] \n"]
  ]

