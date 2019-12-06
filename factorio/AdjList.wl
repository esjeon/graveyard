(* ::Package:: *)

BeginPackage["AdjList`"];
Module[{protected},

protected = Unprotect[AdjList, EdgeList, Graph];

EdgeListQ[edges:{(_->_) ...}] := True;
EdgeListQ[edges_] := False;

WeightedEdgeListQ[edges:{{_->_,_} ...}] := And @@ Map[NumberQ @* Last, edges];
WeightedEdgeListQ[edges_] := False;

AdjList[edges_? EdgeListQ] := AdjList @ Map[
  Association @* (Map @ (#[[2]] -> 1 &)), GroupBy[edges, First]];

AdjList[edges_? WeightedEdgeListQ] := AdjList @ Map[
  Association @* (Map @ (#[[1,2]] -> Last[#] &)), GroupBy[edges, First @* First]];

EdgeList[AdjList[map_Association]] := Join @@ KeyValueMap[
  Function[{src, dsts},
    Map[src -> # &, Keys[dsts]]X
  ], map];

WeightList[AdjList[map_Association]] := Join @@ Map[Values, Values[map]];

Graph[adjlist_AdjList, opts: OptionsPattern[]] :=
  Graph[EdgeList[adjlist], EdgeWeight -> WeightList[adjlist], opts];

Protect[protected];

];
EndPackage[];
