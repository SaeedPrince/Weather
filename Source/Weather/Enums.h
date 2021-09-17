#pragma once
// Engine minimal requirement
#include "CoreMinimal.h"

// Generated
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class eGripState : uint8
{
	VE_Open				UMETA(DisplayName = "Open"),
	VE_CanGrab			UMETA(DisplayName = "CanGrab"),
	VE_Grab				UMETA(DisplayName = "Grab")
};

UENUM(BlueprintType)
enum class eWeather : uint8
{
	VE_NoWeather		UMETA(DisplayName = "No Weather"),
	VE_Sunny			UMETA(DisplayName = "Sunny"),
	VE_Cloudy			UMETA(DisplayName = "Cloudy"),
	VE_PartialCloudy	UMETA(DisplayName = "Partial Cloudy"),
	VE_Windy			UMETA(DisplayName = "Windy")
};
