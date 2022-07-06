// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIPawn.generated.h"

/** Add Comments to Explain the purpose of this class 
* Here
* Here
*/

UCLASS()
class PORTFOLIO_API AAIPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* AIPerception;

	
};
