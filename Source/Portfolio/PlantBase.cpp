// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBase.h"
#include "Soil.h"

// Sets default values
APlantBase::APlantBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	//Set the tick rate of the actor
	SetActorTickInterval(TimePerCycle);
}


float APlantBase::GetWaterRequiredToGrow() const
{
	return 1.0f;
}

float APlantBase::GetHarvestValue() const
{
	return HarvestValue/HarvestFrequency;
}

float APlantBase::GetCostUpKeep() const
{
	return CostUpKeep;
}

float APlantBase::GetWateringUpKeep() const
{
	return WateringUpKeep;
}

// Called when the game starts or when spawned
void APlantBase::BeginPlay()
{
	Super::BeginPlay();

	RemainingTimeToGrow = TimeTakenToGrow;
	TimeToGrowHalfwayPoint = TimeTakenToGrow / 2;
}

// Called every frame
void APlantBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If we haven'd found our owning Soil Actor then try find it
	if (!bHasSoilOwner)
	{
		SoilOwner = Cast<ASoil>(GetAttachParentActor());
		if (!SoilOwner)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find parent Soil Actor for: %s"), *this->GetActorNameOrLabel());
		}
	}

	if (!bIsPlantAnAdult)
	{
		GrowPlant();
	}
	else
	{
		HarvestPlant();
	}

	//If the plant has grown into an Adult but the flag in the Parent Soil Actor hasn't been set then set it to true
	if (bIsPlantAnAdult && !SoilOwner->GetPlantInSoilIsAdult())
	{
		SoilOwner->SetPlantInsideIsAdult();
	}
}

void APlantBase::GrowPlant()
{
	if (MeshComponent)
	{
		if (!bIsPlantAnAdult)
		{
			UpdateTimeTakenToGrow();
			UpdatePlantMesh();
		}
		else
		{
			bIsPlantAnAdult = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("StaticMesh Missing!"));
	}
}

void APlantBase::UpdateTimeTakenToGrow()
{
	if (!IsPlantFullyGrown())
	{
		++CurrentGrowTime;
		RemainingTimeToGrow = TimeTakenToGrow - CurrentGrowTime;
	}
}

void APlantBase::UpdatePlantMesh()
{
	if (IsPlantASeedling())
	{
		MeshComponent->SetStaticMesh(SeedlingMesh);
		return;
	}

	if (IsPlantASapling())
	{
		MeshComponent->SetStaticMesh(SaplingMesh);
		return;
	}

	if (IsPlantAnAdult())
	{
		MeshComponent->SetStaticMesh(PlantMesh);
		return;
	}
}

void APlantBase::HarvestPlant()
{
	if (IsPlantReadyToHarvest())
	{
		//When the plant is ready to harvest, reset the timer back to 0 and flag plant not ready to harvest
		CurrentTimeTillHarvest = 0;
		bIsPlantReadyToHarvest = false;
	}
}

void APlantBase::GetPlantStats(float& OutHarvestValue, float& OutCostUpKeep, float& OutWateringUpKeep)
{
	//Only check the plants stats if it's fully grown and is producing a yield
	if (bIsPlantAnAdult)
	{
		OutHarvestValue = GetHarvestValue();
		OutCostUpKeep  = GetCostUpKeep();
		OutWateringUpKeep = GetWateringUpKeep();
		//If want to retrieve more stats add them here
	}
}

bool APlantBase::IsPlantFullyGrown()
{
	if (RemainingTimeToGrow == 0)
	{
		bIsPlantAnAdult = true;
		return bIsPlantAnAdult;
	}
	else
	{
		return bIsPlantAnAdult;
	}
}

bool APlantBase::IsPlantReadyToHarvest() 
{
	//Check if time since last 
	if (CurrentTimeTillHarvest == HarvestFrequency)
	{
		bIsPlantReadyToHarvest = true;
		return bIsPlantReadyToHarvest;
	}
	else
	{
		++CurrentTimeTillHarvest;
		return bIsPlantReadyToHarvest;
	}
}

bool APlantBase::IsPlantASeedling() const
{
	if ((CurrentGrowTime < TimeToGrowHalfwayPoint) && (CurrentGrowTime > 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlantBase::IsPlantASapling() const
{
	if ((CurrentGrowTime >= TimeToGrowHalfwayPoint) && (CurrentGrowTime < TimeTakenToGrow))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlantBase::IsPlantAnAdult() const
{
	if (CurrentGrowTime == TimeTakenToGrow)
	{
		return true;
	}
	else
	{
		return false;
	}
}
