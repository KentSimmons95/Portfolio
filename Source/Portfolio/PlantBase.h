// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantBase.generated.h"

/** Plant base class
*	It is an Abstract class and must be derived from to be able to place into the level
*	
*	Explain what the plant class has and what each stat does
*/

UENUM()
enum EPlantFamilies
{
	Amaryllidaceae	UMETA(DisplayName = "Amaryllidaceae"),
	Brassicas		UMETA(DisplayName = "Brassicas"),
	Chenopodiaceae	UMETA(DisplayName = "Chenopodiaceae"),
	Compositae		UMETA(DisplayName = "Compositae"),
	Convolvulaceae	UMETA(DisplayName = "Convolvulaceae"),
	Cucurbitaceae	UMETA(DisplayName = "Cucurbitaceae"),
	Fabaceae		UMETA(DisplayName = "Fabaceae"),
	Poaceae			UMETA(DisplayName = "Poaceae"),
	Solanaceae		UMETA(DisplayName = "Solanaceae"),
	Umbelliferae	UMETA(DisplayName = "Umbelliferae")
};

UCLASS(Abstract)
class PORTFOLIO_API APlantBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APlantBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Getters for the plants properties
	UFUNCTION()
	float GetWaterRequiredToGrow() const;
	UFUNCTION()
	float GetHarvestValue() const;
	UFUNCTION()
	float GetCostUpKeep() const;
	UFUNCTION()
	float GetWateringUpKeep() const;

	//Check if the plant is fully grown - BaseTimeTakenToGrow - CurrentGrowTime
	UFUNCTION()
	bool IsPlantFullyGrown();

private:

	/* Each plant has 3 meshes that need to be assigned 
	* SeedMesh		- When the plant has just started growing it will appear as seeds on the ground (Looks the same as the other SeedMeshes)
	* SaplingMesh	- When the plant has started to develop and looks like a sapling/growing plant (Looks the same as the other SaplingMeshes)
	* PlantMesh		- When the plant has completed growing (Has a unique Mesh that is distinguishable to other plants i.e Corn vs Tomato)
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Appearances")
	UStaticMesh* SeedlingMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Appearances")
	UStaticMesh* SaplingMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Appearances")
	UStaticMesh* PlantMesh;

	UStaticMeshComponent* MeshComponent;

	//Set to false at begin
	bool bIsPlantFullyGrown = false;
	bool bIsPlantReadyToHarvest = false;

	float CurrentGrowTime;
	float RemainingTimeToGrow;
	float TimeToGrowHalfwayPoint;

	float CurrentTimeToHarvest;
	float RemainingTimeToHarvest;

	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float PlantingCost = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float WaterRequiredToGrow = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float TimeTakenToGrow = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float HarvestTime = 6;
	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float HarvestValue = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float CostUpKeep = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float WateringUpKeep = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Plant Stats")
	float TimePerCycle = 1.0;
	
	//Start growing the plant when there is enough water available && it is not fully grown
	UFUNCTION()
	void StartGrowingPlant();
	//Update the remaining time to grow for the plant
	UFUNCTION()
	void UpdateTimeTakenToGrow();
	//Update which mesh to use when the plant is growing / grown
	UFUNCTION()
	void UpdatePlantMesh();
	//Harvest the plant when it is ready
	UFUNCTION()
	void HarvestPlant();
	//Get the plants overall stats 
	UFUNCTION()
	void GetPlantStats(float& OutHarvestValue, float& OutCostUpKeep, float& OutWateringUpKeep);

	//Check if there is enough water available in the plot
	UFUNCTION()
	bool IsThereEnoughWater();
	//Check to see if the plant is ready to harvest
	UFUNCTION()
	bool IsPlantReadyToHarvest();

	/*Set of functions to determine which mesh to use as the plant grows
	* If the plant is less than half grown it is a shown as a seed
	* If the plant is over halfway grown but not fully grown then it is a shown as a Sapling
	* If the plant is fully grown then it is shown as a fully grown plant
	*/
	UFUNCTION()
	bool IsPlantASeedling() const;
	UFUNCTION()
	bool IsPlantASapling() const;
	UFUNCTION()
	bool IsPlantAFullPlant() const;

	//Calculate the amount of water that is required to grow each tick - BaseWaterRequiredToGrow / BaseTimeTakenToGrow
	UFUNCTION()
	float WaterRequiredToGrowEachCycle();

	
};
