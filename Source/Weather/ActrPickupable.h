// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine minimal requirement
#include "CoreMinimal.h"

// Base class
#include "Engine/StaticMeshActor.h"

// Internal Libraries
#include "Enums.h"

// Interfaces
#include "InterfacePickup.h"

// Generated
#include "ActrPickupable.generated.h"

UCLASS()
class WEATHER_API AActrPickupable : public AStaticMeshActor, public IInterfacePickup
{
	GENERATED_BODY()

public:
	AActrPickupable();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup Interface")
		void Drop();
		virtual void Drop_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup Interface")
		void Pickup(class USceneComponent* inAttachTo);
		virtual void Pickup_Implementation(class USceneComponent* inAttachTo) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Core;
};
