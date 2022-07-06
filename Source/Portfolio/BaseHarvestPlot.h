// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantBase.h"
#include "Soil.h"
#include "BaseHarvestPlot.generated.h"

/** Plot base class
*	It is an Abstract class and must be derived from 
* 
*	Explain what the plot class has and what each stat does
*/

UCLASS(Abstract)
class PORTFOLIO_API ABaseHarvestPlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseHarvestPlot();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	float TempTickRate = 1.0;

	//Size properties of how large the plot will be (number of plots that will be ceated is Height * Width)
	UPROPERTY(EditAnywhere, Category = "Plot Size")
	int32 PlotHeight;
	UPROPERTY(EditAnywhere, Category = "Plot Size")
	int32 PlotWidth;

	UPROPERTY(EditAnywhere, Category = "Plot Details")
	float CurrentHarvestPlotAvailableWater;
	UPROPERTY(VisibleAnywhere, Category = "Plot Details")
	float CurrentHarvestPlotWaterRequirement = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Plot Details")
	float CurrentHarvestPlotWaterUpKeep = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Plot Details")
	float CurrentHarvestPlotCostUpKeep = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Plot Details")
	float CurrentHarvestPlotScore = 0.f;

	FVector CurrentWorldLocation;
	FRotator CurrentWorldRotation;

	AActor* ParentActor = this;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ASoil* ActorToSpawn;
	
	UPROPERTY(VisibleAnywhere, Category = "Plot Details")
	TArray<ASoil*> SoilInPlot;

	//You can create the plot in the editor
	UFUNCTION(CallInEditor, Category = "Plot Setup")
	void GeneratePlot();

	//Function that is called every tick/event timer of the game to get the current resources being used/generated from the Plants in the Harvest Plot
	UFUNCTION()
	void UpdateHarvestPlotData();
	UFUNCTION()
	void GetSpawnLocationForPlant(uint32 Height, uint32 Width, FVector &OutSpawnLocation);

	//TODO: When a plant is deleted recalculate the resources available to harvest plot/game score
};
