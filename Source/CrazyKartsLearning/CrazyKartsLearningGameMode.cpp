// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CrazyKartsLearningGameMode.h"
#include "CrazyKartsLearningPawn.h"
#include "CrazyKartsLearningHud.h"

ACrazyKartsLearningGameMode::ACrazyKartsLearningGameMode()
{
	DefaultPawnClass = ACrazyKartsLearningPawn::StaticClass();
	HUDClass = ACrazyKartsLearningHud::StaticClass();
}
