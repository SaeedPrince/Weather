// Fill out your copyright notice in the Description page of Project Settings.
#include "ActrMotionController.h"
#include "AnimHand.h"
#include "InterfacePickup.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AActrMotionController::AActrMotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hand = EControllerHand::Left;
	GripState = eGripState::VE_Open;
	TeleportLaunchVelocity = 900.0f;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	if (IsValid(Scene))
	{
		SetRootComponent(Scene);
		MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
		if (IsValid(MotionController))
		{
			MotionController->SetupAttachment(Scene);
			HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
			if (IsValid(HandMesh))
			{
				HandMesh->SetupAttachment(MotionController);
				HandMesh->CastShadow = false;
				if (IsValid(HandSkeletalMesh))
				{
					HandMesh->SetSkeletalMesh(HandSkeletalMesh);
				}
				HandMesh->SetWorldLocationAndRotation(FVector(-12.785664f, -0.028015f, -1.789026f), FRotator(0.0f, 0.0f, 90.0f));
				ArcDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ArcDirection"));
				if (IsValid(ArcDirection))
				{
					ArcDirection->SetupAttachment(HandMesh);
					ArcDirection->SetRelativeLocation(FVector(14.175764f, 0.859525f, -4.318897f));
					ArcDirection->SetArrowColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
					ArcDirection->ArrowSize = 0.2f;
				}
				ArcSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ArcSpline"));
				if (IsValid(ArcSpline))
				{
					ArcSpline->SetupAttachment(HandMesh);
					ArcSpline->SetRelativeLocation(FVector(12.532683f, -1.756977f, 2.553327f));
					ArcSpline->EditorSelectedSplineSegmentColor = FLinearColor(0.728f, 0.364f, 0.003f, 1.0f);
				}
				GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
				if (IsValid(GrabSphere))
				{
					GrabSphere->SetupAttachment(HandMesh);
					GrabSphere->SetRelativeLocation(FVector(14.286342f, 0.224237f, 1.484932f));
					GrabSphere->SetSphereRadius(10.0f);
					GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &AActrMotionController::GrabSphereOverlapped);
				}
			}
		}
		ArcEndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcEndPoint"));
		if (IsValid(ArcEndPoint))
		{
			ArcEndPoint->SetupAttachment(Scene);
			ArcEndPoint->SetAbsolute(true, true, true);
			ArcEndPoint->SetWorldScale3D(FVector(0.15f, 0.15f, 0.15f));
			if (IsValid(ArcEndPointMesh))
			{
				ArcEndPoint->SetStaticMesh(ArcEndPointMesh);
			}
			ArcEndPoint->SetGenerateOverlapEvents(false);
			ArcEndPoint->SetCollisionProfileName(TEXT("NoCollision"));
			ArcEndPoint->SetVisibility(false);
		}
		TeleportCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportCylinder"));
		if (IsValid(TeleportCylinder))
		{
			TeleportCylinder->SetupAttachment(Scene);
			TeleportCylinder->SetAbsolute(true, true, true);
			TeleportCylinder->SetWorldScale3D(FVector(0.75f, 0.75f, 1.00f));
			if (IsValid(TeleportCylinderMesh))
			{
				TeleportCylinder->SetStaticMesh(TeleportCylinderMesh);
			}
			TeleportCylinder->SetCollisionProfileName(TEXT("NoCollision"));
			Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
			if (IsValid(Ring))
			{
				Ring->SetupAttachment(TeleportCylinder);
				Ring->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.15f));
				if (IsValid(RingMesh))
				{
					Ring->SetStaticMesh(RingMesh);
				}
			}
			Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
			if (IsValid(Arrow))
			{
				Arrow->SetupAttachment(TeleportCylinder);
				if (IsValid(ArrowMesh))
				{
					Arrow->SetStaticMesh(ArrowMesh);
				}
				Arrow->SetCollisionProfileName(TEXT("NoCollision"));
				RoomScale = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomScale"));
				if (IsValid(RoomScale))
				{
					RoomScale->SetupAttachment(Arrow);
					if (IsValid(RoomScaleMesh))
					{
						RoomScale->SetStaticMesh(RoomScaleMesh);
					}
					RoomScale->SetAbsolute(false, true, true);
					RoomScale->SetGenerateOverlapEvents(false);
					RoomScale->SetCollisionProfileName(TEXT("NoCollision"));
				}
			}
		}
	}
	SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVRChaperone"));
}

// Called every frame
void AActrMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AttachedActor != nullptr || bWantsToGrip)
	{
		GripState = eGripState::VE_Grab;
	}
	else
	{
		if (_GetActorNearHand() != nullptr)
		{
			GripState = eGripState::VE_CanGrab;
		}
		else
		{
			GripState = eGripState::VE_Open;
		}
	}
	if (IsValid(HandMesh))
	{
		UAnimHand* AnimHand = Cast<UAnimHand>(HandMesh->GetAnimInstance());
		if (IsValid(AnimHand))
		{
			AnimHand->SetHandGripState(GripState);
		}
	}
	_ClearArc();
	if (bTeleporterActive)
	{
		TArray<FVector> TracePoints{};
		FVector NavMeshLocation = FVector();
		FVector TraceLocation = FVector();
		bValidTeleportDestination = _TraceTeleportDestination(TracePoints, NavMeshLocation, TraceLocation);
		if (IsValid(TeleportCylinder))
		{
			TeleportCylinder->SetVisibility(bValidTeleportDestination, true);
			TEnumAsByte<EObjectTypeQuery> theQ = EObjectTypeQuery::ObjectTypeQuery1;
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
			ObjectTypes.Add(theQ);
			TArray<AActor*> IgnoredActors {};
			FHitResult outHit;
			UKismetSystemLibrary::LineTraceSingleForObjects(this, NavMeshLocation, UKismetMathLibrary::Subtract_VectorVector(NavMeshLocation, FVector(0.0f, 0.0f, -200.0f)), ObjectTypes, false, IgnoredActors, EDrawDebugTrace::None, outHit, true);
			FVector NewLocation = (outHit.bBlockingHit ? outHit.ImpactPoint : NavMeshLocation);
			TeleportCylinder->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}
		if ((bValidTeleportDestination && !bLastFrameValidDestination) || (!bValidTeleportDestination && bLastFrameValidDestination))
		{
			RumbleController(0.3f);
		}
		bLastFrameValidDestination = bValidTeleportDestination;
		_UpdateArcSpline(bValidTeleportDestination, TracePoints);
		_UpdateArcEndpoint(TraceLocation, bValidTeleportDestination);
	}
	_UpdateRoomScaleOutline();
	if (IsValid(HandMesh))
	{
		if (GripState == eGripState::VE_Grab)
		{
			HandMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
// Getters
EControllerHand AActrMotionController::GetControllerHand()
{
	return Hand;
}
bool AActrMotionController::IsValidTeleportDestination()
{
	return bValidTeleportDestination;
}
bool AActrMotionController::IsTeleportActive()
{
	return bTeleporterActive;
}
void AActrMotionController::GetTeleportDestination(FVector& Location, FRotator& Rotation)
{
	if (IsValid(TeleportCylinder))
	{
		FVector DevicePos;
		FRotator DeviceRot;
		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRot, DevicePos);
		Rotation = TeleportRotation;
		Location = UKismetMathLibrary::Subtract_VectorVector(TeleportCylinder->GetComponentLocation(), UKismetMathLibrary::Quat_RotateVector(FQuat(Rotation), FVector(DevicePos.X, DevicePos.Y, 0.0f)));
	}
}
FRotator AActrMotionController::GetInitialHandControllerRotation()
{
	return InitialControllerRotation;
}
UMotionControllerComponent* AActrMotionController::GetHandMotionController()
{
	return MotionController;
}
// Setters
void AActrMotionController::SetControllerHand(EControllerHand inHand)
{
	Hand = inHand;
}
void AActrMotionController::SetHandTeleportRotation(const FRotator& inRotator)
{
	TeleportRotation = inRotator;
}
// Commands
void AActrMotionController::RumbleController(float intensity)
{
	if (IsValid(HapticFeedbackEffectBase))
	{
		APlayerController* plContl = UGameplayStatics::GetPlayerController(this, 0);
		if (IsValid(plContl))
		{
			plContl->PlayHapticEffect(HapticFeedbackEffectBase, Hand, intensity);
		}
	}
}
// Grabbing
void AActrMotionController::GrabActor()
{
	bWantsToGrip = true;
	if (IsValid(MotionController))
	{
		AActor* theActor = _GetActorNearHand();
		if (IsValid(theActor))
		{
			IInterfacePickup* theInterface = Cast<IInterfacePickup>(theActor);
			if (theInterface)
			{
				AttachedActor = theActor;
				IInterfacePickup::Execute_Pickup(theActor, MotionController);
				RumbleController(0.7f);
			}
		}
	}
}
void AActrMotionController::ReleaseActor()
{
	bWantsToGrip = false;
	if (IsValid(MotionController))
	{
		if (IsValid(AttachedActor))
		{
			IInterfacePickup* theInterface = Cast<IInterfacePickup>(AttachedActor);
			if (theInterface)
			{
				IInterfacePickup::Execute_Drop(AttachedActor);
				RumbleController(0.2f);
				AttachedActor = nullptr;
			}
		}
	}
}
// Teleportation
void AActrMotionController::ActivateTeleporter()
{
	bTeleporterActive = true;
	if (IsValid(TeleportCylinder))
	{
		TeleportCylinder->SetVisibility(true, true);
	}
	if (IsValid(RoomScale))
	{
		RoomScale->SetVisibility(IsRoomScale);
	}
	if (IsValid(MotionController))
	{
		InitialControllerRotation = MotionController->GetComponentRotation();
	}
}
void AActrMotionController::DisableTeleporter()
{
	if (bTeleporterActive)
	{
		bTeleporterActive = false;
		if (IsValid(TeleportCylinder))
		{
			TeleportCylinder->SetVisibility(false, true);
		}
		if (IsValid(ArcEndPoint))
		{
			ArcEndPoint->SetVisibility(false);
		}
		if (IsValid(RoomScale))
		{
			RoomScale->SetVisibility(false);
		}
	}
}
// Protected functions
// Called when the game starts or when spawned
void AActrMotionController::BeginPlay()
{
	Super::BeginPlay();
	_SetupRoomScaleOutline();
	if (IsValid(TeleportCylinder))
	{
		TeleportCylinder->SetVisibility(false, true);
	}
	if (IsValid(RoomScale))
	{
		RoomScale->SetVisibility(false);
	}
	if (IsValid(HandMesh))
	{
		if (Hand == EControllerHand::Left)
		{
			HandMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
		}
	}
}

void AActrMotionController::OnConstruction(const FTransform& inTransform)
{
	if (IsValid(MotionController))
	{
		MotionController->SetTrackingMotionSource(FName(UKismetStringLibrary::Conv_ByteToString((uint8)Hand)));
	}
}
// Events
void AActrMotionController::GrabSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherComp))
	{
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(OtherComp);
		if (IsValid(StaticMesh))
		{
			if (StaticMesh->IsSimulatingPhysics())
			{
				RumbleController(0.8f);
			}
		}
	}
}
// Private functions
// Grabbing
AActor* AActrMotionController::_GetActorNearHand()
{
	AActor* retActor = nullptr;
	float NearestOverlap = 10000.0f;
	if (IsValid(GrabSphere))
	{
		TArray<AActor*> Overs;
		GrabSphere->GetOverlappingActors(Overs);
		for (int i = 0; i < Overs.Num(); i++)
		{
			IInterfacePickup* theInterface = Cast<IInterfacePickup>(Overs[i]);
			if (theInterface)
			{
				FVector ActrLoc = Overs[i]->GetActorLocation();
				FVector SphrLoc = GrabSphere->GetComponentLocation();
				FVector Minus = UKismetMathLibrary::Subtract_VectorVector(ActrLoc, SphrLoc);
				float MinusLen = Minus.Size();
				if (MinusLen < NearestOverlap)
				{
					NearestOverlap = MinusLen;
					retActor = Overs[i];
				}
			}
		}
	}
	return retActor;
}
// Teleportation
void AActrMotionController::_ClearArc()
{
	for (int i = 0; i < SplineMeshes.Num(); i++)
	{
		SplineMeshes[i]->DestroyComponent();
	}
	SplineMeshes.Empty();
	if (IsValid(ArcSpline))
	{
		ArcSpline->ClearSplinePoints();
	}
}
bool AActrMotionController::_TraceTeleportDestination(TArray<FVector>& TracePoints, FVector& NavMeshLocation, FVector& TraceLocation)
{
	bool bol1 = false;
	bool bol2 = false;
	if (IsValid(ArcDirection))
	{
		float ProjectNavExtends = 500.0f;
		FHitResult outHit;
		TracePoints.Empty();
		FVector outDestin;
		TEnumAsByte<EObjectTypeQuery> theQ = EObjectTypeQuery::ObjectTypeQuery1;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
		ObjectTypes.Add(theQ);
		TArray<AActor*> IgnoredActores{};
		bol1 = UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(this, outHit, TracePoints, outDestin, ArcDirection->GetComponentLocation(), UKismetMathLibrary::Multiply_VectorFloat(ArcDirection->GetForwardVector(), TeleportLaunchVelocity), true, 0.0f, ObjectTypes, false, IgnoredActores, EDrawDebugTrace::None, 0.0f, 30.0f);
		TraceLocation = outHit.Location;
		UNavigationSystemV1* theNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (IsValid(theNavSystem))
		{
			FNavLocation ProjectedLocation;
			FVector theExtent = FVector(ProjectNavExtends, ProjectNavExtends, ProjectNavExtends);
			bol2 = theNavSystem->ProjectPointToNavigation(TraceLocation, ProjectedLocation, theExtent);
			NavMeshLocation = FVector(ProjectedLocation);
		}
	}
	return (bol1 && bol2);
}
void AActrMotionController::_UpdateArcSpline(bool bFoundValidLocation, const TArray<FVector>& SplinePoints)
{
	if (IsValid(ArcSpline) && IsValid(ArcDirection))
	{
		TArray<FVector> thePoints{};
		if (bFoundValidLocation)
		{
			thePoints = SplinePoints;
		}
		else
		{
			FVector ArcLoc = ArcDirection->GetComponentLocation();
			thePoints.Add(ArcLoc);
			thePoints.Add(UKismetMathLibrary::Add_VectorVector(ArcLoc, UKismetMathLibrary::Multiply_VectorFloat(ArcDirection->GetForwardVector(), 20.0f)));
		}
		for (int i = 0; i < thePoints.Num(); i++)
		{
			ArcSpline->AddSplinePoint(thePoints[i], ESplineCoordinateSpace::Local);
		}
		ArcSpline->SetSplinePointType(thePoints.Num() - 1, ESplinePointType::CurveClamped);
		for (int i = 0; i <= ArcSpline->GetNumberOfSplinePoints() - 2; i++)
		{
			UClass* theContext = USplineMeshComponent::StaticClass();
			USplineMeshComponent* newComp = Cast<USplineMeshComponent>(this->AActor::AddComponent(FName(EName::NAME_Actor, i), true, FTransform(), theContext));
			if (IsValid(newComp))
			{
				SplineMeshes.Add(newComp);
				newComp->SetStartAndEnd(thePoints[i], ArcSpline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local), thePoints[i + 1], ArcSpline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local));
			}
		}
	}
}
void AActrMotionController::_UpdateArcEndpoint(const FVector& NewLocation, bool bFoundValidLocation)
{
	if (IsValid(ArcEndPoint))
	{
		ArcEndPoint->SetVisibility(bFoundValidLocation && bTeleporterActive);
		ArcEndPoint->SetWorldLocation(NewLocation);
	}
	FRotator rot;
	FVector vec;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(rot, vec);
	FRotator newRot = UKismetMathLibrary::ComposeRotators(TeleportRotation, FRotator(0.0f, rot.Yaw, 0.0f));
	if (IsValid(Arrow))
	{
		Arrow->SetWorldRotation(newRot);
	}
	if (IsValid(RoomScale))
	{
		RoomScale->SetWorldRotation(newRot);
	}
}
// Room-scale
void AActrMotionController::_SetupRoomScaleOutline()
{
	float ChaperoneMeshHeight = 70.0f;
	if (IsValid(SteamVRChaperone))
	{
		FVector outRectVect;
		FRotator outRectRot;
		float outSideLengthX;
		float outSideLengthY;
		UKismetMathLibrary::MinimumAreaRectangle(this, SteamVRChaperone->GetBounds(), FVector(0.0f, 0.0f, 1.0f), outRectVect, outRectRot, outSideLengthX, outSideLengthY);
		IsRoomScale = !(UKismetMathLibrary::NearlyEqual_FloatFloat(outSideLengthX, 100.0f, 0.01f) && UKismetMathLibrary::NearlyEqual_FloatFloat(outSideLengthY, 100.0f, 0.01f));
		if (IsRoomScale && IsValid(RoomScale))
		{
			RoomScale->SetWorldScale3D(FVector(outSideLengthX, outSideLengthY, ChaperoneMeshHeight));
			RoomScale->SetRelativeRotation(outRectRot);
		}
	}
}
void AActrMotionController::_UpdateRoomScaleOutline()
{
	if (IsValid(RoomScale))
	{
		if (RoomScale->IsVisible())
		{
			FRotator rot;
			FVector vec;
			UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(rot, vec);
			FRotator newRot = FRotator();
			newRot.Yaw = rot.Yaw;
			RoomScale->SetRelativeLocation(UKismetMathLibrary::Quat_UnrotateVector(FQuat(newRot), UKismetMathLibrary::Divide_VectorFloat(FVector(vec.X, vec.Y, 0.0f), (-1.0f))));
		}
	}
}