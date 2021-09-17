// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimHand.h"
#include "ActrMotionController.h"
#include "Kismet/KismetMathLibrary.h"

// Constructor
UAnimHand::UAnimHand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GripState = eGripState::VE_Open;
	InterpSpeed = 7.0f;
	Grip = 0.0f;
	HandEnum = EControllerHand::Left;
}

// Setters
void UAnimHand::SetHandGripState(eGripState inGripState)
{
	GripState = inGripState;
}

// Protected
void UAnimHand::NativeInitializeAnimation()
{
	AActrMotionController* actrCont = Cast<AActrMotionController>(GetOwningActor());
	if (IsValid(actrCont))
	{
		HandEnum = actrCont->GetControllerHand();
	}
}

void UAnimHand::NativeUpdateAnimation(float DeltaSeconds)
{
	int32 i = UKismetMathLibrary::Conv_ByteToInt((uint8)GripState);
	float j = UKismetMathLibrary::Conv_IntToFloat(i);
	Grip = UKismetMathLibrary::FInterpTo_Constant(Grip, j / 2.0f, DeltaSeconds, InterpSpeed);
}
