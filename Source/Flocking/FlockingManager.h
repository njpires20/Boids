
#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

		void Init(UWorld* world, UStaticMeshComponent* mesh);
	void Flock();

private:
	UWorld* World;
	bool initialized;
	TArray<class AAgent*> Agents;
	FVector rule1(AAgent* Agent);
	FVector rule2(AAgent* Agent);
	FVector rule3(AAgent* Agent);
	FVector bound_position(AAgent* Agent, float distance);

	void limit_velocity(AAgent* Agent, float vlim);

};