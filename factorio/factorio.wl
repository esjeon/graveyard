(* ::Package:: *)

BeginPackage["factorio`"];


ImportRecipes[filename_String] := Map[Recipe, Import[filename,"JSON"]];


RecipeAssociationQ[map_Association] := And @@ Map[
  KeyExistsQ[map,#]&, {"name", "duration", "ingredients", "results"}
]


Recipe[map? RecipeAssociationQ] := Recipe[
  map[["name"]],
  map[["duration"]],
  map[["ingredients"]],
  map[["results"]],
]


EndPackage[];
