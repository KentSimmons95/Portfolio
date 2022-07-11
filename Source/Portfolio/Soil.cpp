// Fill out your copyright notice in the Description page of Project Settings.


#include "Soil.h"

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

}

// Called every frame
void ASoil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoil::SpawnPlantToGrow()
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
			CurrentPlantInSoil->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
			bHasPlantInSoil = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to AttachToActor! Parent Actor: %s"), *ParentActor->GetActorNameOrLabel());
		}
	}
}

void ASoil::DestroyPlant()
{
	//Check to see if we have a plant in the soil before destroying the actor
	if (CurrentPlantInSoil)
	{
		//Reset the current soil properties to empty since the plant is being removed
		CurrentPlantWaterRequirement = 0;
		CurrentPlantHarvestValue = 0;
		CurrentPlantUpKeepCost = 0;

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

float ASoil::GetPlantBaseUpKeepCost()
{
	if (bHasPlantInSoil)
	{
		CurrentPlantUpKeepCost = CurrentPlantInSoil->GetCostUpKeep();
	}

	return CurrentPlantUpKeepCost;
}

