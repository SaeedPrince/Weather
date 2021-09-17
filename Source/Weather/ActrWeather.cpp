// Fill out your copyright notice in the Description page of Project Settings.
#include "ActrWeather.h"

// Sets default values
AActrWeather::AActrWeather()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (IsValid(SceneComponent))
	{
		SetRootComponent(SceneComponent);
		Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
		if (IsValid(Sphere))
		{
			Sphere->SetupAttachment(SceneComponent);
			Sphere->CastShadow = false;
		}
	}
}

// Called when the game starts or when spawned
void AActrWeather::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActrWeather::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* AActrWeather::GetMesh()
{
	return Sphere;
}

void AActrWeather::SetWeather(eWeather inWeather)
{
	UMaterialInstanceDynamic* matRef = Sphere->CreateDynamicMaterialInstance(0);
	matRef->SetVectorParameterValue(TEXT("AppliedColor"), MapWeatherToColor.FindRef(inWeather));
}
