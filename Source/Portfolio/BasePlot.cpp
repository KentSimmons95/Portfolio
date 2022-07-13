// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHarvestPlot.h"
#include "PortfolioGameModeBase.h"


// Sets default values
ABaseHarvestPlot::ABaseHarvestPlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(TempTickRate);
}

float ABaseHarvestPlot::GetCurrentWaterUpKeep() const
{
	return CurrentHarvestPlotWaterUpKeep;
}

float ABaseHarvestPlot::GetCurrentPlotCostUpKeep() const
{
	return CurrentHarvestPlotCostUpKeep;
}

float ABaseHarvestPlot::GetCurrentHarvestPlotGoldScore() const
{
	return CurrentHarvestPlotGoldScore;
}

// Called when the game starts or when spawned
void ABaseHarvestPlot::BeginPlay()
{
	Super::BeginPlay();

	//Add the Harvest Plot to the game mode
	GameMode = Cast<APortfolioGameModeBase>(GameplayStatic->GetGameMode(GetWorld()));
	GameMode->RegisterHarvestPlot(this);
}

// Called every frame
void ABaseHarvestPlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFarmData();
}

void ABaseHarvestPlot::GeneratePlot()
{
	//Check if an Actor has been selected in the editor, if nullptr return early and log message
	if (!ActorToSpawn)
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
			ASoil* NewSoil = GetWorld()->SpawnActor<ASoil>(ActorToSpawn, ActorToSpawnLocation, ActorToSpawnRotation);

			//Attach the newly spawned Actors to the Plot Actor and keep its World Transform
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
		float TempWaterUpKeep = 0.f;
		float TempCostUpKeep = 0.f;
		float TempHarvestGoldValue = 0.f;

		for (ASoil* Soil : SoilInPlot)
		{
			if (Soil->bHasPlantInSoil)
			{
				if (Soil->GetCurrentPlantInSoil()->IsPlantFullyGrown())
				{
					APlantBase* PlantInSoil = Soil->GetCurrentPlantInSoil();

					//Total all the Values in Temporary Variables - then assign them to the plot scores before 
					//They get reset on the next UpdateHarvestPlotData() call
					TempWaterUpKeep += PlantInSoil->GetWateringUpKeep();
					TempCostUpKeep += PlantInSoil->GetCostUpKeep();
					TempHarvestGoldValue += PlantInSoil->GetHarvestValue();
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Plant not found? TODO - Register Soil with Plant to Plot"));
			}
		}

		CurrentHarvestPlotWaterUpKeep = TempWaterUpKeep;
		CurrentHarvestPlotCostUpKeep = TempCostUpKeep;
		CurrentHarvestPlotGoldScore = TempHarvestGoldValue;
	}
}

void ABaseHarvestPlot::GetSpawnLocationForPlant(uint32 Height, uint32 Width, FVector& OutSpawnLocation)
{
	OutSpawnLocation.X = CurrentWorldLocation.X + (Height * 100);
	OutSpawnLocation.Y = CurrentWorldLocation.Y + (Width * 100);
}

void ABaseHarvestPlot::UpdateFarmData()
{
	UpdateHarvestPlotData();
	
}



