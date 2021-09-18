// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine minimal requirement
#include "CoreMinimal.h"

// Base class
#include "GameFramework/Actor.h"

// Internal Libraries
#include "Enums.h"

// Components
#include "MotionControllerComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "SteamVRChaperoneComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

// Generated
#include "ActrMotionController.generated.h"

class USceneComponent;
class UMotionControllerComponent;
class USkeletalMesh;
class USkeletalMeshComponent;
class UArrowComponent;
class USplineComponent;
class USphereComponent;
class UStaticMesh;
class UStaticMeshComponent;
class USteamVRChaperoneComponent;
class UHapticFeedbackEffect_Base;

UCLASS()
class WEATHER_API AActrMotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActrMotionController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		EControllerHand GetControllerHand();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		bool IsValidTeleportDestination();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		bool IsTeleportActive();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		void GetTeleportDestination(FVector& Location, FRotator& Rotation);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		FRotator GetInitialHandControllerRotation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		class UMotionControllerComponent* GetHandMotionController();

	UFUNCTION(BlueprintCallable, Category = "Setters")
		void SetControllerHand(EControllerHand inHand);

	UFUNCTION(BlueprintCallable, Category = "Setters")
		void SetHandTeleportRotation(const FRotator& inRotator);

	UFUNCTION(BlueprintCallable, Category = "Commands")
		void RumbleController(float intensity);

	UFUNCTION(BlueprintCallable, Category = "Grabbing")
		void GrabActor();

	UFUNCTION(BlueprintCallable, Category = "Grabbing")
		void ReleaseActor();

	UFUNCTION(BlueprintCallable, Category = "Teleportation")
		void ActivateTeleporter();

	UFUNCTION(BlueprintCallable, Category = "Teleportation")
		void DisableTeleporter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& inTransform) override;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void GrabSphereOverlapped(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class USkeletalMesh* HandSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* ArcEndPointMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* TeleportCylinderMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* RingMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* ArrowMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* RoomScaleMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Haptic")
		class UHapticFeedbackEffect_Base* HapticFeedbackEffectBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* ArcDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USplineComponent* ArcSpline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* GrabSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* ArcEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* TeleportCylinder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Ring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* RoomScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USteamVRChaperoneComponent* SteamVRChaperone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		EControllerHand Hand;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		class AActor* AttachedActor;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bTeleporterActive;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		TArray<class USplineMeshComponent*> SplineMeshes;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bLastFrameValidDestination;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool IsRoomScale;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		FVector TeleportDestination;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bValidTeleportDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		eGripState GripState;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bWantsToGrip;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		FRotator TeleportRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float TeleportLaunchVelocity;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		FRotator InitialControllerRotation;

private:
	UFUNCTION(BlueprintCallable, Category = "Grabbing", meta = (AllowPrivateAccess = "true"))
		class AActor* _GetActorNearHand();

	UFUNCTION(BlueprintCallable, Category = "Teleportation", meta = (AllowPrivateAccess = "true"))
		void _ClearArc();

	UFUNCTION(BlueprintCallable, Category = "Teleportation", meta = (AllowPrivateAccess = "true"))
		bool _TraceTeleportDestination(TArray<FVector>& TracePoints, FVector& NavMeshLocation, FVector& TraceLocation);

	UFUNCTION(BlueprintCallable, Category = "Teleportation", meta = (AllowPrivateAccess = "true"))
		void _UpdateArcSpline(bool bFoundValidLocation, const TArray<FVector>& SplinePoints);

	UFUNCTION(BlueprintCallable, Category = "Teleportation", meta = (AllowPrivateAccess = "true"))
		void _UpdateArcEndpoint(const FVector& NewLocation, bool bFoundValidLocation);

	UFUNCTION(BlueprintCallable, Category = "Room-scale", meta = (AllowPrivateAccess = "true"))
		void _SetupRoomScaleOutline();

	UFUNCTION(BlueprintCallable, Category = "Room-scale", meta = (AllowPrivateAccess = "true"))
		void _UpdateRoomScaleOutline();

};