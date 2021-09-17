// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine minimal requirement
#include "CoreMinimal.h"

// Base class
#include "GameFramework/Pawn.h"

// Internal Libraries
#include "Enums.h"

// Components
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

// Generated
#include "PawnVR.generated.h"

class UStaticMesh;
class UCameraComponent;
class UArrowComponent;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS()
class WEATHER_API APawnVR : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnVR();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* TeleportPinMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* RingMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Variables|Mesh")
		class UStaticMesh* ArrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UArrowComponent* TraceDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* TeleportPin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Ring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Arrow;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		FVector PinnedLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bLocationFound;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bLocationPinned;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		FRotator PinnedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float RotationLengthThreshold;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		FVector CurrentLookAtLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bCurrentLocationValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float FadeOutDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float FadeInDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		FLinearColor TeleportFadeColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float DefaultPlayerHeight;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		class UMaterialInstanceDynamic* RingGlowMatInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		bool UseGamePad;

};
