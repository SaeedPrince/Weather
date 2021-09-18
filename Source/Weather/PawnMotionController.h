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

// Generated
#include "PawnMotionController.generated.h"

class UCameraComponent;
class AActrMotionController;

UCLASS()
class WEATHER_API APawnMotionController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnMotionController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool bShowChaperone;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		class AActrMotionController* LeftController;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		class AActrMotionController* RightController;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		class AActrMotionController* DelayedMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float FadeOutDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float FadeInDuration;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool IsTeleporting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		FLinearColor TeleportFadeColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float ThumbDeadZone;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool RightStickDown;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool LeftStickDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Init")
		float DefaultPlayerHeight;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool UseControllerRollToRotate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables|Classes")
		TSubclassOf<class AActrMotionController> MotionControllerBP;

	UPROPERTY(BlueprintReadWrite, Category = "Variables|InputAxis")
		float ThumbLeft_Y;

	UPROPERTY(BlueprintReadWrite, Category = "Variables|InputAxis")
		float ThumbLeft_X;

	UPROPERTY(BlueprintReadWrite, Category = "Variables|InputAxis")
		float ThumbRight_Y;

	UPROPERTY(BlueprintReadWrite, Category = "Variables|InputAxis")
		float ThumbRight_X;

private:
	UFUNCTION(BlueprintCallable, Category = "Calculations", meta = (AllowPrivateAccess = "true"))
		FRotator _GetRotationFromInput(float UpAxis, float RightAxis, class AActrMotionController* inController);

	UFUNCTION(BlueprintCallable, Category = "Events", meta = (AllowPrivateAccess = "true"))
		void _ExecuteTeleportation(class AActrMotionController* inMotionController);

	UFUNCTION(BlueprintCallable, Category = "Delays", meta = (AllowPrivateAccess = "true"))
		void _AfterTeleportationDelay();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _GrabLeftPressed();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _GrabLeftReleased();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _GrabRightPressed();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _GrabRightReleased();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _TeleportLeftPressed();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _TeleportLeftReleased();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _TeleportRightPressed();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void _TeleportRightReleased();

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void MotionControllerThumbLeft_Y(float inAxis);

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void MotionControllerThumbLeft_X(float inAxis);
	
	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void MotionControllerThumbRight_Y(float inAxis);

	UFUNCTION(BlueprintCallable, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
		void MotionControllerThumbRight_X(float inAxis);

};
