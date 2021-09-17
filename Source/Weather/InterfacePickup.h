// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// Engine minimal requirement
#include "CoreMinimal.h"

// Base class
#include "UObject/Interface.h"

// Libraries
#include "Enums.h"

// Generated
#include "InterfacePickup.generated.h"

class USceneComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfacePickup : public UInterface
{
	GENERATED_BODY()
};

class WEATHER_API IInterfacePickup
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup Interface")
		void Drop();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup Interface")
		void Pickup(class USceneComponent* inAttachTo);

};
