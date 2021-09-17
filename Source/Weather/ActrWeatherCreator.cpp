// Fill out your copyright notice in the Description page of Project Settings.
#include "ActrWeatherCreator.h"
#include "ActrWeather.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AActrWeatherCreator::AActrWeatherCreator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MapNumberToWeather.Add(0, eWeather::VE_Windy);
	MapNumberToWeather.Add(1, eWeather::VE_Sunny);
	MapNumberToWeather.Add(2, eWeather::VE_Cloudy);
	MapNumberToWeather.Add(3, eWeather::VE_PartialCloudy);
}

// Called when the game starts or when spawned
void AActrWeatherCreator::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(TextRenderClass) && IsValid(ActorWeatherBP))
	{
		TArray<AActor*> TextRenders;
		TArray<ATextRenderActor*> Cities;
		UGameplayStatics::GetAllActorsOfClass(this, TextRenderClass, TextRenders);
		for (int i = 0; i < TextRenders.Num(); i++)
		{
			ATextRenderActor* textRender = Cast<ATextRenderActor>(TextRenders[i]);
			if (IsValid(textRender))
			{
				UTextRenderComponent* txComp = textRender->GetTextRender();
				if (IsValid(txComp))
				{
					if (txComp->ComponentTags[0] == TEXT("City"))
					{
						Cities.Add(textRender);
					}
				}
			}
		}
		for (int i = 0; i < Cities.Num(); i++)
		{
			FTransform SpTr = FTransform();
			FVector SpLoc = Cities[i]->GetActorLocation();
			SpLoc.Z += 250.0f;
			SpTr.SetLocation(SpLoc);
			AActrWeather* ActorWeather = GetWorld()->SpawnActorDeferred<AActrWeather>(ActorWeatherBP, SpTr, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (IsValid(ActorWeather))
			{
				ActorWeather->SetWeather(MapNumberToWeather.FindRef(i));
				UGameplayStatics::FinishSpawningActor(ActorWeather, SpTr);
			}
		}
	}
}

// Called every frame
void AActrWeatherCreator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}