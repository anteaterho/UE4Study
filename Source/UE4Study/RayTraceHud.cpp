// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Study.h"
#include "RayTraceHud.h"
#include "Engine.h"

void ARayTraceHud::PostRender()
{
	//Get Player
	const ULocalPlayer *LP = Cast<ULocalPlayer>(PlayerOwner->Player);
	FVector2D MousePos = LP->ViewportClient->GetMousePosition();

	UFont *SampleFont = GEngine->GetLargeFont();
	FVector2D Pos;

	float XL, YL;
	FString SampleText = TEXT("[ Screen XY ]");
	SampleText = SampleText.operator+=(FString::SanitizeFloat(MousePos.X));
	SampleText = SampleText.operator+=(" ");
	SampleText = SampleText.operator+=(FString::SanitizeFloat(MousePos.Y));

	Canvas->StrLen(SampleFont, SampleText, XL, YL);

	Pos.X = (Canvas->SizeX / 2) - XL / 2;
	Pos.Y = (Canvas->SizeY / 2);

	Canvas->SetDrawColor(FColor(20, 255, 10, 255));
	Canvas->DrawText(SampleFont, SampleText, Pos.X, Pos.Y);
}


