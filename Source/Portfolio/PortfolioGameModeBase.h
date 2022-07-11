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
	bool HaveEnoughResources(float &InPlantWaterCost, float &InPlantMoneyCost);

protected:

private:

	//List of Harvest plots in the game
	TArray<ABaseHarvestPlot*> HarvestPlots;

	UPROPERTY(EditDefaultsOnly, Category = "Harvest Plot Actors")
	TSubclassOf<ABaseHarvestPlot> ClassToFind;
	UPROPERTY(VisibleAnywhere, Category = "Game Info")
	TArray<AActor*> FoundActors;
		
	UGameplayStatics* GameplayStatic;

	UPROPERTY(EditDefaultsOnly, Category = "Farm Resources")
	float TotalWaterOnFarm;
	UPROPERTY(EditDefaultsOnly, Category = "Farm Resources")
	float TotalMoneyOnFarm;
	float CurrentWaterUsageOnFarm;
	float CurrentCostUpKeepOnFarm;
	float CurrentHarvestValueOnFarm;

	//Go through each Harvest Plot in the game and get their stat totals to update the Game Mode information
	UFUNCTION()
	void UpdateFarmStats();
	//Function that is called when the level is loaded/begin play is called to get all of the harvest plots in the level.
	UFUNCTION()
	void GetHarvestPlotsInLevel();
};
