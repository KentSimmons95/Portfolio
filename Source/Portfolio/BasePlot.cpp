// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHarvestPlot.h"

// Sets default values
ABaseHarvestPlot::ABaseHarvestPlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(TempTickRate);
}


// Called when the game starts or when spawned
void ABaseHarvestPlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseHarvestPlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHarvestPlotData();
}

void ABaseHarvestPlot::GeneratePlot()
{
	//Check if an Actor has been selected in the editor, if nullptr return early and log message
	if (ActorToSpawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor To Spawn is Empty!"));
		return;
	}
	//Destroy the existing plants if the button pressed again
	for (auto CurrentSoil : SoilInPlot)
	{
		if (CurrentSoil)
		{
			CurrentSoil->Destroy();
		}
	}
	//Clean the array
	SoilInPlot.Empty();

	CurrentWorldLocation = GetActorLocation();
	CurrentWorldRotation = GetActorRotation();

	for (int i = 0; i < PlotHeight; ++i)
	{
		for (int j = 0; j < PlotWidth; ++j)
		{
			FVector ActorToSpawnLocation;
			FRotator ActorToSpawnRotation;
			GetSpawnLocationForPlant(i, j, ActorToSpawnLocation);
			ASoil* NewSoil = GetWorld()->SpawnActor<ASoil>(ActorToSpawn->GetClass(), ActorToSpawnLocation, ActorToSpawnRotation);

			//Attach the newly spawned Actors to the Plot Actor and keep its World Transform (
			NewSoil->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
			SoilInPlot.Add(NewSoil);
		}
	}
}

void ABaseHarvestPlot::UpdateHarvestPlotData()
{
	//Iterate through all of the Soil Actors in the Harvest Plot and check whether they have a Plant or not
	//If the Soil has a Plant in it AND it is Fully Grown - get their UpKeep/Value stats for the Harvest Plot
	if (SoilInPlot.Num() > 0)
	{
		for (ASoil* Soil : SoilInPlot)
		{
			if (Soil->bHasPlantInSoil && Soil->GetCurrentPlantInSoil()->IsPlantFullyGrown())
			{
				APlantBase* PlantInSoil = Soil->GetCurrentPlantInSoil();

				CurrentHarvestPlotWaterUpKeep += PlantInSoil->GetWateringUpKeep();
				CurrentHarvestPlotCostUpKeep +=  PlantInSoil->GetCostUpKeep();
				CurrentHarvestPlotScore += PlantInSoil->GetHarvestValue();
			}
		}
	}
}

void ABaseHarvestPlot::GetSpawnLocationForPlant(uint32 Height, uint32 Width, FVector& OutSpawnLocation)
{
	OutSpawnLocation.X = CurrentWorldLocation.X + (Height * 100);
	OutSpawnLocation.Y = CurrentWorldLocation.Y + (Width * 100);
}



