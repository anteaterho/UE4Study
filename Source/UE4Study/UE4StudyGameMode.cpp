// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Study.h"
#include "UE4StudyGameMode.h"
#include "RayTraceHud.h"

AUE4StudyGameMode::AUE4StudyGameMode()
{
	HUDClass = ARayTraceHud::StaticClass();
}


