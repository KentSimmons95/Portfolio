// Copyright Epic Games, Inc. All Rights Reserved.


#include "PortfolioGameModeBase.h"


void APortfolioGameModeBase::StartPlay()
{
	Super::StartPlay();

	TotalWaterOnFarm = StartingWaterOnFarm;
	TotalGoldOnFarm = StartingGoldOnFarm;
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
	return TotalGoldOnFarm;
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
	return CurrentGoldUpKeepOnFarm;
}

float APortfolioGameModeBase::GetCurrentHarvestValueOnFarm() const
{
	return CurrentHarvestValueOnFarm;
}

bool APortfolioGameModeBase::HaveEnoughResources(float& InPlantMoneyCost)
{
	//Check if we have enough resources available to plant
	//If there is enough Money AND Water then plant
	bool HasEnoughMoney = false;

	if (GetCurrentMoneyAvailable() >= InPlantMoneyCost)
	{
		HasEnoughMoney = true;
		return HasEnoughMoney;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough money to plant! - You are: %f money short"), InPlantMoneyCost - GetCurrentMoneyAvailable());
		return HasEnoughMoney;
	}
}

void APortfolioGameModeBase::CalcCurrentAvailableWaterOnFarm()
{
	CurrentAvailableWaterOnFarm = TotalWaterOnFarm - CurrentWaterUsageOnFarm;
}

void APortfolioGameModeBase::CalcCurrentGoldUpKeepOnFarm()
{
	CurrentAvailableGoldOnFarm = TotalGoldOnFarm - CurrentGoldUpKeepOnFarm;
}

void APortfolioGameModeBase::CalcPlantingCost(float PlantingCost)
{
	TotalGoldOnFarm -= PlantingCost;
}

bool APortfolioGameModeBase::EnoughGoldToPlant(float PlantCost)
{
	bool CanPlant = false;
	if (PlantCost <= TotalGoldOnFarm)
	{
		CanPlant = true;
		return CanPlant;
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
			TempCurrentHarvestValue += CurrentHarvestPlot->GetCurrentHarvestPlotGoldScore();
		}

		CurrentWaterUsageOnFarm = TempCurrentWaterUpKeep;
		CurrentGoldUpKeepOnFarm = TempCurrentPlotUpKeep;
		CurrentHarvestValueOnFarm = TempCurrentHarvestValue;

		UE_LOG(LogTemp, Warning, TEXT("CurrentWaterUsageOnFarm: %f"), CurrentWaterUsageOnFarm);
		UE_LOG(LogTemp, Warning, TEXT("CurrentCostUpKeepOnFarm: %f"), CurrentGoldUpKeepOnFarm);
		UE_LOG(LogTemp, Warning, TEXT("CurrentHarvestValueOnFarm: %f"), CurrentHarvestValueOnFarm);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find Harvest Plots in Level!"));
	}
}

void APortfolioGameModeBase::RegisterHarvestPlot(ABaseHarvestPlot* PlotToRegister)
{
	if (PlotToRegister)
	{
		HarvestPlots.Add(PlotToRegister);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to register Harvest Plot!"));
	}
}
