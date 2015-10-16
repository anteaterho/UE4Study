// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "RayTraceHud.generated.h"

/**
 * 
 */
UCLASS()
class UE4STUDY_API ARayTraceHud : public AHUD
{
	GENERATED_BODY()


	virtual void PostRender() override;
	
public:
	FVector GetMouseWorldPosition();

private:
	FVector MouseWorldPosition;
	
};
