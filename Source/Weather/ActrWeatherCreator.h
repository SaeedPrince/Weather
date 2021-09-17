// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine minimal requirement
#include "CoreMinimal.h"

// Base Class
#include "GameFramework/Actor.h"

// Internal Libraries
#include "Enums.h"

// Components
#include "Engine/TextRenderActor.h"

// Generated
#include "ActrWeatherCreator.generated.h"

class AActrWeather;

UCLASS()
class WEATHER_API AActrWeatherCreator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActrWeatherCreator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		TMap<int32, eWeather> MapNumberToWeather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		TSubclassOf<AActor> TextRenderClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		TSubclassOf<class AActrWeather> ActorWeatherBP;

};
