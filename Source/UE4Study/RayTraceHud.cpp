// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Study.h"
#include "RayTraceHud.h"
#include "PawnWithCamera.h"
#include "Engine.h"

void ARayTraceHud::PostRender()
{
	//Get Player
	const ULocalPlayer *LP = Cast<ULocalPlayer>(PlayerOwner->Player);
	FVector2D MousePos = LP->ViewportClient->GetMousePosition();

	//Get Player Pawn(PawnWithCamera)
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	APawnWithCamera *MyPawn = Cast<APawnWithCamera>(MyController->GetPawn());
	float dist = MyPawn->GetDistanceWithTwoPoints();
	bool bShowLog = MyPawn->GetShowLog();

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

	//Show Mouse position(screen coordinate)
	Canvas->SetDrawColor(FColor(20, 255, 10, 255));
	Canvas->DrawText(SampleFont, SampleText, Pos.X, Pos.Y - 30.f);

	//Parameters for Trace
	FString TraceLocDebug = TEXT("[ Hit XYZ ]");
	FString TraceActorDebug = TEXT("[ Hit Actor ]");
	FString TraceDistance = TEXT("[ Hit Distance ]");

	FVector WorldOrigin, WorldDirection, TraceEnd;
	FHitResult TraceHitResult;
	FCollisionObjectQueryParams TraceObjectParam;
	FCollisionQueryParams TraceParam;

	TraceObjectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	TraceObjectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	TraceObjectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	TraceParam.bTraceComplex = true;
	TraceParam.bTraceAsyncScene = true;
	TraceParam.bReturnPhysicalMaterial = false;
	
	//Draw Info
	//Do Trace
	if (Canvas != NULL && PlayerOwner != NULL)
	{
		//Screen to World
		Canvas->Deproject(MousePos, WorldOrigin, WorldDirection);
		TraceEnd = WorldOrigin + WorldDirection * 6536.0f;

		//Just Trace
		GetWorld()->LineTraceSingle(TraceHitResult, WorldOrigin, TraceEnd, TraceParam, TraceObjectParam);
		MouseWorldPosition = TraceHitResult.ImpactPoint;

		TraceLocDebug = TraceLocDebug.operator+=(" ");
		TraceLocDebug = TraceLocDebug.operator+=(FString::FromInt(int(MouseWorldPosition.X)));
		TraceLocDebug = TraceLocDebug.operator+=(" , ");
		TraceLocDebug = TraceLocDebug.operator+=(FString::FromInt(int(MouseWorldPosition.Y)));
		TraceLocDebug = TraceLocDebug.operator+=(" , ");
		TraceLocDebug = TraceLocDebug.operator+=(FString::FromInt(int(MouseWorldPosition.Z)));

		Canvas->DrawText(SampleFont, TraceLocDebug, Pos.X, Pos.Y - 10.0f);

		if (TraceHitResult.GetActor() != NULL && bShowLog == true)
		{
			SampleFont->SetFontScalingFactor(10.f);
			TraceActorDebug = TraceActorDebug.operator+=(" ");
			//Get Hit Actors Name
			TraceActorDebug = TraceActorDebug.operator+=(TraceHitResult.GetActor()->GetName());

			//World to screen
			FVector TargetScreenPos = Canvas->Project(TraceEnd);
			FVector2D MiddlePoint = Pos + ((FVector2D::FVector2D(TargetScreenPos) - Pos) * 0.5f);

			Canvas->DrawText(SampleFont, TraceActorDebug, TargetScreenPos.X + 10.f, TargetScreenPos.Y);

			TraceDistance = TraceDistance.operator+=(" ");
			TraceDistance = TraceDistance.operator+=(FString::FromInt(int(dist)));

			Canvas->DrawText(SampleFont, TraceDistance, MiddlePoint.X, MiddlePoint.Y);
		}
	}
	else if (Canvas == NULL || PlayerOwner == NULL)
	{
		MouseWorldPosition = FVector(0.f, 0.f, 0.f);
		TraceLocDebug = TraceLocDebug.operator+=(TEXT("Canvas or PC NONE"));
	}

}

FVector ARayTraceHud::GetMouseWorldPosition()
{
	return MouseWorldPosition;
}


