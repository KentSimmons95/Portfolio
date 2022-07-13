// Fill out your copyright notice in the Description page of Project Settings.


#include "Soil.h"
#include "PortfolioGameModeBase.h"

// Sets default values
ASoil::ASoil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

APlantBase* ASoil::GetCurrentPlantInSoil() const
{
	return CurrentPlantInSoil;
}

// Called when the game starts or when spawned
void ASoil::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<APortfolioGameModeBase>(GameplayStatic->GetGameMode(GetWorld()));

	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find PortfolioGameMode"));
		return;
	}
}

// Called every frame
void ASoil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoil::GrowPlant()
{
	//Early return if we already have a plant in the soil
	if (bHasPlantInSoil)
	{
		UE_LOG(LogTemp, Warning, TEXT("Plant already exists in current soil!"));
		return;
	}

	//Find the Plant to Spawn in PlantInfo Struct and get the price on how much it costs
	float PlantCost;
	for (int i = 0; i < PlantsInfo.Num(); ++i)
	{
		if (ActorToSpawn->GetClass() == PlantsInfo[i].PlantType->GetClass())
		{
			PlantCost = PlantsInfo[i].PlantCost;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ActorToSpawn Actor Class not found!"));
			return;
		}
	}

	//If there is enough gold available to plant then plant and subtract gold
	if (GameMode->EnoughGoldToPlant(PlantCost))
	{
		GameMode->CalcPlantingCost(PlantCost);
		SpawnPlant();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Enough gold to Plant!"));
	}
}

void ASoil::SpawnPlant()
{
	//Spawn a Plant Actor from the Soil Actor
	//First check if the PlantToSpawn is valid then spawn the Plant on the Soil Actor's Location and Rotation & Attach 
	if (ActorToSpawn)
	{
		FVector ActorToSpawnLocation = GetActorLocation();
		FRotator ActorToSpawnRotation = GetActorRotation();

		CurrentPlantInSoil = GetWorld()->SpawnActor<APlantBase>(ActorToSpawn, ActorToSpawnLocation, ActorToSpawnRotation);
		
		if (CurrentPlantInSoil)
		{
			CurrentPlantInSoil->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			bHasPlantInSoil = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to AttachToActor! Parent Actor: %s"), *this->GetActorNameOrLabel());
		}
	}
}

void ASoil::RefundPlant()
{
	//Check to see if we have a plant in the soil before destroying the actor
	if (CurrentPlantInSoil)
	{
		//Reset the current soil properties to empty since the plant is being removed
		CurrentPlantWaterRequirement = 0;
		CurrentPlantHarvestValue = 0;
		CurrentPlantGoldUpKeepCost = 0;

		CurrentPlantInSoil->Destroy();

		bHasPlantInSoil = false;
	}
}

float ASoil::GetPlantWaterRequiredToGrow()
{
	if (bHasPlantInSoil)
	{
		CurrentPlantWaterRequirement = CurrentPlantInSoil->GetWaterRequiredToGrow();
	}

	return CurrentPlantWaterRequirement;
}

float ASoil::GetPlantHarvestValue()
{
	if (bHasPlantInSoil)
	{
		CurrentPlantHarvestValue = CurrentPlantInSoil->GetHarvestValue();
	}
	
	return CurrentPlantHarvestValue;
}

float ASoil::GetPlantGoldUpKeepCost()
{
	if (bHasPlantInSoil)
	{
		CurrentPlantGoldUpKeepCost = CurrentPlantInSoil->GetCostUpKeep();
	}
	
	return CurrentPlantGoldUpKeepCost;
}

