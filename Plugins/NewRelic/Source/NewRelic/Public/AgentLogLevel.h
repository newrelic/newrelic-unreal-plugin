// Copyright (c) 2024 NewRelic. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AgentLogLevel.generated.h"

UENUM(BlueprintType)
enum class AgentLogLevel : uint8 {
	INFO       UMETA(DisplayName="Info"),
	ERROR        UMETA(DisplayName="Error"),
	VERBOSE        UMETA(DisplayName="Verbose"),
	WARNING        UMETA(DisplayName="Warning"),
	DEBUG        UMETA(DisplayName="Debug")
};