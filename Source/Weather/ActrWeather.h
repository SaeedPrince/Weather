// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
// Engine minimal requirement
#include "CoreMinimal.h"

// Base Class
#include "GameFramework/Actor.h"

// Internal Libraries
#include "Enums.h"

// Components
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

// Generated
#include "ActrWeather.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class WEATHER_API AActrWeather : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActrWeather();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Sphere subobject **/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
		class UStaticMeshComponent* GetMesh();

	UFUNCTION(BlueprintCallable, Category = "Setters")
		void SetWeather(eWeather inWeather);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		TMap<eWeather, FColor> MapWeatherToColor;

};
