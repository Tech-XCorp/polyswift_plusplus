(* ::Package:: *)

(* ::Title:: *)
(*Parse data files to visualize/analyze*)
(*geometry results*)


(* ::Input:: *)
<<"/Users/swsides/mathematica/mathModules/sides.m"


(* ::Input:: *)
SetDirectory["/Users/swsides/psall/polyswift/psutils/python"]
FileNames[]


(* ::Input:: *)
tmp=readFile["geometry.dat",1];
geoDat=getColumns[tmp,{1,2,4}];

(* ::Input:: *)
fig=ListPlot3D[geoDat,AxesLabel->{"x","y"},ImageSize->800];
Export["geometry.png",fig]



