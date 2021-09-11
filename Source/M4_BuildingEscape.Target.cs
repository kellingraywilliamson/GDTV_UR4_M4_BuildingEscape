// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class M4_BuildingEscapeTarget : TargetRules
{
	public M4_BuildingEscapeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "M4_BuildingEscape" } );
	}
}
