// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine minimal requirement
#include "CoreMinimal.h"

// Base class
#include "Animation/AnimInstance.h"

// Libraries
#include "Enums.h"

// Generated
#include "AnimHand.generated.h"

UCLASS()
class WEATHER_API UAnimHand : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// Constructor
	UAnimHand(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Setters")
		void SetHandGripState(eGripState inGripState);

protected:

	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		eGripState GripState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		float Grip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
		EControllerHand HandEnum;

};
