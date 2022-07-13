// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantBase.h"
#include "Soil.generated.h"

/** Struct that store information about the different types of plants and their
*	cost/refund values 
*/
USTRUCT(BlueprintType)
struct FPlantIfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Plant Info")
	TSubclassOf<APlantBase> PlantType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Plant Info")
	float PlantCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Plant Info")
	float PlantRefund;
};

/** Soil class
*	The Soil Class is the connection between the Plant and HarvestPlot Class.
*	A Plant must be in Soil to grow and there has to be a Harvest Plot for the Soil 
*	to be placed inside.
*	
*	The Soil class can keep track of it's current conditions such as how much Water is in the Soil,
*	Fertilizer levels and overall Soil quality.
*/
UCLASS()
class PORTFOLIO_API ASoil : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoil();

	//Get the current plant in the soil
	APlantBase* GetCurrentPlantInSoil() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	float GetPlantWaterRequiredToGrow(); //TODO Remove OR Implement better (Water growing system)
	UFUNCTION()
	float GetPlantHarvestValue();
	UFUNCTION()
	float GetPlantGoldUpKeepCost();

	UPROPERTY(VisibleAnywhere)
	bool bHasPlantInSoil = false;

private:
	UStaticMeshComponent* Mesh;

	float CurrentPlantWaterRequirement = 0;
	float CurrentPlantHarvestValue = 0;
	float CurrentPlantGoldUpKeepCost = 0;

	UGameplayStatics* GameplayStatic;
	class APortfolioGameModeBase* GameMode;

	//The type of Plant we want to Spawn - TODO: Called on UI input
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlantBase> ActorToSpawn;

	//Stores the information about different plant child classes (PlantCost/RefundValue)
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Plant Types")
	TArray<FPlantIfo> PlantsInfo;
	
	UPROPERTY(VisibleAnywhere, Category = "Soil Properties")
	APlantBase* CurrentPlantInSoil;
	
	UPROPERTY(EditDefaultsOnly, Category = "Soil Properties")
	float WaterLevels;
	UPROPERTY(EditDefaultsOnly, Category = "Soil Properties")
	float FertilizerLevels;
	UPROPERTY(EditDefaultsOnly, Category = "Soil Properties")
	float SoilQuality;

	//Spawn a new plant into the world
	UFUNCTION(CallInEditor, Category = "Soil Properties")
	void GrowPlant();
	void SpawnPlant();

	UFUNCTION()
	void RefundPlant();
};
