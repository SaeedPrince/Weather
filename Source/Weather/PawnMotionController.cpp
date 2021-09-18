// Fill out your copyright notice in the Description page of Project Settings.
#include "PawnMotionController.h"
#include "ActrMotionController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Public/TimerManager.h"
// Public
// Sets default values
APawnMotionController::APawnMotionController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FadeOutDuration = 0.1f;
	FadeInDuration = 0.2f;
	TeleportFadeColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ThumbDeadZone = 0.7f;
	DefaultPlayerHeight = 180.0f;
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
			}
		}
	}
}
// Called every frame
void APawnMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(LeftController))
	{
		if (LeftController->IsTeleportActive())
		{
			LeftController->SetHandTeleportRotation(_GetRotationFromInput(ThumbLeft_Y, ThumbLeft_X, LeftController));
		}
	}
	if (IsValid(RightController))
	{
		if (RightController->IsTeleportActive())
		{
			RightController->SetHandTeleportRotation(_GetRotationFromInput(ThumbRight_Y, ThumbRight_X, RightController));
		}
	}
}
// Called to bind functionality to input
void APawnMotionController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Action Mappings
	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &APawnMotionController::_GrabLeftPressed);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &APawnMotionController::_GrabLeftReleased);
	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &APawnMotionController::_GrabRightPressed);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &APawnMotionController::_GrabRightReleased);
	PlayerInputComponent->BindAction("TeleportLeft", IE_Pressed, this, &APawnMotionController::_TeleportLeftPressed);
	PlayerInputComponent->BindAction("TeleportLeft", IE_Released, this, &APawnMotionController::_TeleportLeftReleased);
	PlayerInputComponent->BindAction("TeleportRight", IE_Pressed, this, &APawnMotionController::_TeleportRightPressed);
	PlayerInputComponent->BindAction("TeleportRight", IE_Released, this, &APawnMotionController::_TeleportRightReleased);
	// Axis Mappings
	PlayerInputComponent->BindAxis("MotionControllerThumbLeft_Y", this, &APawnMotionController::MotionControllerThumbLeft_Y);
	PlayerInputComponent->BindAxis("MotionControllerThumbLeft_X", this, &APawnMotionController::MotionControllerThumbLeft_X);
	PlayerInputComponent->BindAxis("MotionControllerThumbRight_Y", this, &APawnMotionController::MotionControllerThumbRight_Y);
	PlayerInputComponent->BindAxis("MotionControllerThumbRight_X", this, &APawnMotionController::MotionControllerThumbRight_X);
}
// Protected
// Called when the game starts or when spawned
void APawnMotionController::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(VROrigin))
	{
		FName DevName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();
		if (DevName == TEXT("OculusHMD") || DevName == TEXT("SteamVR"))
		{
			UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
		}
		else
		{
			if (DevName == TEXT("PSVR"))
			{
				UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
				VROrigin->AddLocalOffset(FVector(0.0f, 0.0f, DefaultPlayerHeight));
				UseControllerRollToRotate = true;
			}
		}
		if (IsValid(MotionControllerBP))
		{
			FTransform SpTr = FTransform();
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
			LeftController = GetWorld()->SpawnActorDeferred<AActrMotionController>(MotionControllerBP, SpTr, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (IsValid(LeftController))
			{
				LeftController->SetControllerHand(EControllerHand::Left);
				UGameplayStatics::FinishSpawningActor(LeftController, SpTr);
				LeftController->AttachToComponent(VROrigin, AttachmentTransformRules);
			}
			RightController = GetWorld()->SpawnActorDeferred<AActrMotionController>(MotionControllerBP, SpTr, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (IsValid(RightController))
			{
				RightController->SetControllerHand(EControllerHand::Right);
				UGameplayStatics::FinishSpawningActor(LeftController, SpTr);
				RightController->AttachToComponent(VROrigin, AttachmentTransformRules);
			}
		}
	}
}
// Private
// Calculations
FRotator APawnMotionController::_GetRotationFromInput(float UpAxis, float RightAxis, AActrMotionController* inController)
{
	FRotator retRotator;
	if (IsValid(inController))
	{
		FTransform FrTr = FTransform(inController->GetInitialHandControllerRotation());
		UMotionControllerComponent* theMotionController = inController->GetHandMotionController();
		if (IsValid(theMotionController))
		{
			FTransform SeTr = theMotionController->GetComponentTransform();
			FTransform ThTr = UKismetMathLibrary::ConvertTransformToRelative(FrTr, SeTr);
			FRotator A = FRotator(FQuat(ThTr.GetRotation()));
			FRotator B = FRotator(0.0f, A.Roll * 3.0f + GetActorRotation().Yaw, 0.0f);
			FRotator ActorRot = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
			FVector NormalVector = FVector(UpAxis, RightAxis, 0.0f);
			UKismetMathLibrary::Vector_Normalize(NormalVector);
			FVector RotateVector = UKismetMathLibrary::Quat_RotateVector(FQuat(ActorRot), NormalVector);
			FRotator RotFromXVec = UKismetMathLibrary::MakeRotFromX(RotateVector);
			retRotator = UKismetMathLibrary::SelectRotator(UKismetMathLibrary::SelectRotator(RotFromXVec, GetActorRotation(), (abs(UpAxis) + abs(RightAxis)) >= ThumbDeadZone), B, UseControllerRollToRotate);
		}
	}
	return retRotator;
}
// Events
void APawnMotionController::_ExecuteTeleportation(AActrMotionController* inMotionController)
{
	if (IsValid(inMotionController))
	{
		if (!IsTeleporting)
		{
			if (inMotionController->IsValidTeleportDestination())
			{
				IsTeleporting = true;
				APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
				if (IsValid(CamManager))
				{
					CamManager->StartCameraFade(0.0f, 0.1f, FadeOutDuration, TeleportFadeColor, false, true);
					DelayedMotionController = inMotionController;
					FTimerHandle TimerDelay;
					GetWorld()->GetTimerManager().SetTimer(TimerDelay, this, &APawnMotionController::_AfterTeleportationDelay, FadeOutDuration);
				}
			}
			else
			{
				inMotionController->DisableTeleporter();
			}
		}
	}
}
// Delays
void APawnMotionController::_AfterTeleportationDelay()
{
	if (IsValid(DelayedMotionController))
	{
		DelayedMotionController->DisableTeleporter();
		FVector DestLoc;
		FRotator DestRot;
		DelayedMotionController->GetTeleportDestination(DestLoc, DestRot);
		AActor::TeleportTo(DestLoc, DestRot);
		APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if (IsValid(CamManager))
		{
			CamManager->StartCameraFade(1.0f, 0.0f, FadeInDuration, TeleportFadeColor, false, false);
		}
	}
	IsTeleporting = false;
}
// Inputs
void APawnMotionController::_GrabLeftPressed()
{
	if (IsValid(LeftController))
	{
		LeftController->GrabActor();
	}
}
void APawnMotionController::_GrabLeftReleased()
{
	if (IsValid(LeftController))
	{
		LeftController->ReleaseActor();
	}
}
void APawnMotionController::_GrabRightPressed()
{
	if (IsValid(RightController))
	{
		RightController->GrabActor();
	}
}
void APawnMotionController::_GrabRightReleased()
{
	if (IsValid(RightController))
	{
		RightController->ReleaseActor();
	}
}
void APawnMotionController::_TeleportLeftPressed()
{
	if (IsValid(LeftController) && IsValid(RightController))
	{
		LeftController->ActivateTeleporter();
		RightController->DisableTeleporter();
	}
}
void APawnMotionController::_TeleportLeftReleased()
{
	if (IsValid(LeftController))
	{
		if (LeftController->IsTeleportActive())
		{
			_ExecuteTeleportation(LeftController);
		}
	}
}
void APawnMotionController::_TeleportRightPressed()
{
	if (IsValid(RightController) && IsValid(LeftController))
	{
		RightController->ActivateTeleporter();
		LeftController->DisableTeleporter();
	}
}
void APawnMotionController::_TeleportRightReleased()
{
	if (IsValid(RightController))
	{
		if (RightController->IsTeleportActive())
		{
			_ExecuteTeleportation(RightController);
		}
	}
}
void APawnMotionController::MotionControllerThumbLeft_Y(float inAxis)
{
	ThumbLeft_Y = inAxis;
}
void APawnMotionController::MotionControllerThumbLeft_X(float inAxis)
{
	ThumbLeft_X = inAxis;
}
void APawnMotionController::MotionControllerThumbRight_Y(float inAxis)
{
	ThumbRight_Y = inAxis;
}
void APawnMotionController::MotionControllerThumbRight_X(float inAxis)
{
	ThumbRight_X = inAxis;
}