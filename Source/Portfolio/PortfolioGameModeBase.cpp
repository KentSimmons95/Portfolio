// Copyright Epic Games, Inc. All Rights Reserved.


#include "PortfolioGameModeBase.h"


void APortfolioGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetHarvestPlotsInLevel();
}

float APortfolioGameModeBase::GetTotalWaterOnFarm() const
{
	return TotalWaterOnFarm;
}

float APortfolioGameModeBase::GetCurrentWaterAvailable()
{
	return GetTotalWaterOnFarm() - GetCurrentWaterUsage();
}

float APortfolioGameModeBase::GetTotalMoneyOnFarm() const
{
	return TotalMoneyOnFarm;
}

float APortfolioGameModeBase::GetCurrentMoneyAvailable()
{
	return GetTotalMoneyOnFarm() - GetCostUpKeepOnFarm();
}

float APortfolioGameModeBase::GetCurrentWaterUsage() const
{
	return CurrentWaterUsageOnFarm;
}

float APortfolioGameModeBase::GetCostUpKeepOnFarm() const
{
	return CurrentCostUpKeepOnFarm;
}

float APortfolioGameModeBase::GetCurrentHarvestValueOnFarm() const
{
	return CurrentHarvestValueOnFarm;
}

bool APortfolioGameModeBase::HaveEnoughResources(float& InPlantWaterCost, float& InPlantMoneyCost)
{
	//Check if we have enough resources available to plant
	//If there is enough Money AND Water then plant
	bool HasEnoughWater = false;
	bool HasEnoughMoney = false;
	
	if (GetCurrentWaterAvailable() > InPlantWaterCost)
	{
		HasEnoughWater = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough water to plant! - You are: %f water short"), InPlantWaterCost - GetCurrentWaterAvailable());
		return HasEnoughWater;
	}

	if (GetCurrentMoneyAvailable() > InPlantMoneyCost)
	{
		HasEnoughMoney = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough money to plant! - You are: %f money short"), InPlantMoneyCost - GetCurrentMoneyAvailable());
		return HasEnoughMoney;
	}

	if (HasEnoughWater && HasEnoughMoney)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APortfolioGameModeBase::UpdateFarmStats()
{
	//If we have 1 or more harvest plots then we can get their stats
	if (HarvestPlots.Num() > 0)
	{
		float TempCurrentWaterUpKeep = 0.f;
		float TempCurrentPlotUpKeep = 0.f;
		float TempCurrentHarvestValue = 0.f;

		for (ABaseHarvestPlot* CurrentHarvestPlot : HarvestPlots)
		{
			//Total all the Values in Temporary Variables - then assign them to the total farm scores 
			TempCurrentWaterUpKeep += CurrentHarvestPlot->GetCurrentWaterUpKeep();
			TempCurrentPlotUpKeep += CurrentHarvestPlot->GetCurrentPlotCostUpKeep();
			TempCurrentHarvestValue += CurrentHarvestPlot->GetCurrentHarvestPlotScore();
		}

		CurrentWaterUsageOnFarm = TempCurrentWaterUpKeep;
		CurrentCostUpKeepOnFarm = TempCurrentPlotUpKeep;
		CurrentHarvestValueOnFarm = TempCurrentHarvestValue;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find Harvest Plots in Level!"));
	}
}

void APortfolioGameModeBase::GetHarvestPlotsInLevel()
{
	if (ClassToFind)
	{
		//Get all the Harvest actors  into an Array of AActors
		GameplayStatic->GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

		//Once we have all the Harvest Actors cast them into a proper Array type conainer of AHarvestPlot
		for (AActor* Actor : FoundActors)
		{
			
			HarvestPlots.Add(Cast<ABaseHarvestPlot>(Actor));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode missing class to find!"));
	}
}
