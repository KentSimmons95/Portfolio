// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseHarvestPlot.h"
#include "Kismet/GameplayStatics.h"
#include "PortfolioGameModeBase.generated.h"

/** The Game Mode class will hold important information about the game
 *	such as Resources availibility and current up keep requirements.
 *  The Game Mode will be able to send this information into the UI so the player 
 *  can visually see what is happening in the game.
 */

UCLASS()
class PORTFOLIO_API APortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	virtual void StartPlay() override;

	//Harvest Plots can get the game mode and add themselves
	UFUNCTION()
	void RegisterHarvestPlot(ABaseHarvestPlot* PlotToRegister);

	//Getters for the games water
	UFUNCTION()
	float GetTotalWaterOnFarm() const;
	UFUNCTION()
	float GetCurrentWaterUsage() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentWaterAvailable();

	//Getters for the games money
	UFUNCTION()
	float GetTotalMoneyOnFarm() const; 
	UFUNCTION()
	float GetCostUpKeepOnFarm() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentMoneyAvailable();
	
	//Getters for the games harvest
	UFUNCTION()
	float GetCurrentHarvestValueOnFarm() const;

	//Check that there is enough resources available to plant
	bool HaveEnoughResources(float &InPlantMoneyCost);

	//Calculations for Net-Resource values
	void CalcCurrentAvailableWaterOnFarm();
	void CalcCurrentGoldUpKeepOnFarm();

	//Calculate resources when planting
	void CalcPlantingCost(float PlantCost);
	bool EnoughGoldToPlant(float PlantCost);

	void CollectHarvestPlotGoldIncome(float& CurrentHarvestPlotGoldScore);

protected:

private:
	//List of Harvest plots in the game
	TArray<ABaseHarvestPlot*> HarvestPlots;

	UPROPERTY(EditDefaultsOnly, Category = "Harvest Plot Actors")
	TSubclassOf<ABaseHarvestPlot> ClassToFind;
	UPROPERTY(VisibleAnywhere, Category = "Game Info")
	TArray<AActor*> FoundActors;
	
	UGameplayStatics* GameplayStatic;

	//The water that is available at the start of the game
	UPROPERTY(EditDefaultsOnly, Category = "Farm Resources")
	float StartingWaterOnFarm = 250;
	//The total water that is available in the game (Starting water + water gained over time) - set to StartingWaterOnFarm at BeginPlay
	UPROPERTY(VisibleAnywhere, Category = "Farm Resources")
	float TotalWaterOnFarm;
	//The gold that is available at the start of the game
	UPROPERTY(EditDefaultsOnly, Category = "Farm Resources")
	float StartingGoldOnFarm = 500;
	//The total gold that is available in the game (Starting gold + gold gained over time) - set to StartingGoldOnFarm at BeginPlay
	UPROPERTY(VisibleAnywhere, Category = "Farm Resources")
	float TotalGoldOnFarm;

	float CurrentWaterUsageOnFarm;
	float CurrentAvailableWaterOnFarm;

	float CurrentGoldUpKeepOnFarm;
	float CurrentAvailableGoldOnFarm;

	float CurrentHarvestValueOnFarm;

	//Go through each Harvest Plot in the game and get their stat totals to update the Game Mode information
	UFUNCTION()
	void UpdateFarmStats();

};
