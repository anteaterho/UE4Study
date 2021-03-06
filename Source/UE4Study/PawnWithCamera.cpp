// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Study.h"
#include "PawnWithCamera.h"
#include "DrawDebugHelpers.h"
#include "RayTraceHud.h"


// Sets default values
APawnWithCamera::APawnWithCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 40.0f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);

	DistanceWithTwoPoints = 0.5f;
	bShowLog = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();

	//Show mouse cursor
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	MyController->bShowMouseCursor = true;
}

// Called every frame
void APawnWithCamera::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}

	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	{
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);
	}

	{
		if (!MovementInput.IsZero())
		{
			MovementInput = MovementInput.SafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}

	//Get Key Input
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::X))
	{
		bShowLog = true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased(EKeys::X))
	{
		bShowLog = false;
	}
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);
	InputComponent->BindAction("GetActor", IE_Pressed, this, &APawnWithCamera::GetActor);

	InputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &APawnWithCamera::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APawnWithCamera::YawCamera);
}

void APawnWithCamera::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APawnWithCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APawnWithCamera::ZoomIn()
{
	bZoomingIn = true;
}

void APawnWithCamera::ZoomOut()
{
	bZoomingIn = false;
}

bool APawnWithCamera::GetShowLog()
{
	return bShowLog;
}

float APawnWithCamera::GetDistanceWithTwoPoints()
{
	return DistanceWithTwoPoints;
	UE_LOG(LogClass, Warning, TEXT("Distance %f"), DistanceWithTwoPoints);
}

void APawnWithCamera::GetActor()
{
	UE_LOG(LogClass, Warning, TEXT("Pressed Button!!"));
	//Get RayTraceHud
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	ARayTraceHud *Hudref = Cast<ARayTraceHud>(MyController->GetHUD());
	FVector WorldMousePos = Hudref->GetMouseWorldPosition();

	FVector Start = GetActorLocation();

	DistanceWithTwoPoints = FVector::Dist(Start, WorldMousePos);

	DrawDebugLine(GetWorld(), Start, WorldMousePos, FColor(255, 0, 0), true, 0.5f, 0, 5.0f);
}

