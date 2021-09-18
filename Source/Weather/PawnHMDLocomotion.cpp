// Fill out your copyright notice in the Description page of Project Settings.
#include "PawnHMDLocomotion.h"
// Public
// Sets default values
APawnHMDLocomotion::APawnHMDLocomotion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RotationLengthThreshold = 100.0f;
	FadeOutDuration = 0.1f;
	FadeInDuration = 0.2f;
	TeleportFadeColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
	DefaultPlayerHeight = 180.0f;
	UseGamePad = true;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	if (IsValid(Scene))
	{
		SetRootComponent(Scene);
		VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
		if (IsValid(VROrigin))
		{
			VROrigin->SetupAttachment(Scene);
			Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
			if (IsValid(Camera))
			{
				Camera->SetupAttachment(VROrigin);
				TraceDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TraceDirection"));
				if (IsValid(TraceDirection))
				{
					TraceDirection->SetupAttachment(Camera);
					TraceDirection->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));
				}
			}
		}
		TeleportPin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportPin"));
		if (IsValid(TeleportPin))
		{
			TeleportPin->SetupAttachment(Scene);
			TeleportPin->SetAbsolute(true, true, false);
			TeleportPin->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
			if (IsValid(TeleportPinMesh))
			{
				TeleportPin->SetStaticMesh(TeleportPinMesh);
			}
			Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
			if (IsValid(Ring))
			{
				Ring->SetupAttachment(TeleportPin);
				Ring->SetAbsolute(false, true, false);
				Ring->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.15f));
				if (IsValid(RingMesh))
				{
					Ring->SetStaticMesh(RingMesh);
				}
			}
			Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
			if (IsValid(Arrow))
			{
				Arrow->SetupAttachment(TeleportPin);
				Arrow->SetAbsolute(false, true, false);
				if (IsValid(Arrow))
				{
					Arrow->SetStaticMesh(ArrowMesh);
				}
			}
		}
	}
}
// Called when the game starts or when spawned
void APawnHMDLocomotion::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void APawnHMDLocomotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called to bind functionality to input
void APawnHMDLocomotion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}