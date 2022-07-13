// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantBase.h"
#include "PortfolioGameModeBase.h"

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
	return HarvestValue;
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

	GrowPlant();
}

void APlantBase::GrowPlant()
{
	if (MeshComponent)
	{
		if (!bIsPlantFullyGrown)
		{
			UpdateTimeTakenToGrow();
			UpdatePlantMesh();
		}
		else
		{
			bIsPlantFullyGrown = true;
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

	if (IsPlantAFullPlant())
	{
		MeshComponent->SetStaticMesh(PlantMesh);
		return;
	}
}

void APlantBase::HarvestPlant()
{
	if (IsPlantReadyToHarvest())
	{
		//TODO: Get scoreboard and add points to score - then reset plant to not ready to harvest
		
		//Reset the remaining time to harvest back to the base time
		//Reset the Current time to harvest counter to 0
		RemainingTimeToHarvest = HarvestTime;
		CurrentTimeToHarvest = 0;

		bIsPlantReadyToHarvest = false;
	}
}

void APlantBase::GetPlantStats(float& OutHarvestValue, float& OutCostUpKeep, float& OutWateringUpKeep)
{
	//Only check the plants stats if it's fully grown and is producing a yield
	if (bIsPlantFullyGrown)
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
		bIsPlantFullyGrown = true;
		return bIsPlantFullyGrown;
	}
	else
	{
		return bIsPlantFullyGrown;
	}
}

bool APlantBase::IsPlantReadyToHarvest() 
{
	if (HarvestTime == CurrentTimeToHarvest)
	{
		bIsPlantReadyToHarvest = true;
		return bIsPlantReadyToHarvest;
	}
	else
	{
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

bool APlantBase::IsPlantAFullPlant() const
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
