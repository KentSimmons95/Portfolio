// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantBase.h"
#include "Soil.generated.h"

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
	float GetPlantWaterRequiredToGrow();
	UFUNCTION()
	float GetPlantHarvestValue();
	UFUNCTION()
	float GetPlantBaseUpKeepCost();

	UPROPERTY(VisibleAnywhere)
	bool bHasPlantInSoil = false;

private:
	UStaticMeshComponent* Mesh;

	float CurrentPlantWaterRequirement = 0;
	float CurrentPlantHarvestValue = 0;
	float CurrentPlantUpKeepCost = 0;

	//The type of Plant we want to Spawn - TODO: Called on UI input
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlantBase> ActorToSpawn;
	

	UPROPERTY(VisibleAnywhere, Category = "Soil Properties")
	APlantBase* CurrentPlantInSoil;
	
	AActor* ParentActor = this;

	UPROPERTY(EditDefaultsOnly, Category = "Soil Properties")
	float WaterLevels;
	UPROPERTY(EditDefaultsOnly, Category = "Soil Properties")
	float FertilizerLevels;
	UPROPERTY(EditDefaultsOnly, Category = "Soil Properties")
	float SoilQuality;

	//Function that will begin growing a new Plant, needs to check if enough resources are available to start growing
	UFUNCTION(CallInEditor, Category = "Soil Properties")
	void SpawnPlantToGrow();
	UFUNCTION()
	void DestroyPlant();
};
