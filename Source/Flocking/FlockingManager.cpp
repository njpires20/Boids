// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingManager.h"

#define AGENT_COUNT 10

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));

    World = world;
    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f;

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    FVector v1, v2, v3, v4;
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        v1 = rule1(Agents[i]);
        v2 = rule2(Agents[i]);
        v3 = rule3(Agents[i]);
        v4 = bound_position(Agents[i], 100);

        Agents[i]->Velocity += v1 + v2 + v3 + v4;
        limit_velocity(Agents[i], 20);
    }
}

FVector UFlockingManager::rule1(AAgent* Agent)
{
    FVector returnVector = FVector();
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        if (Agents[i] == Agent)
            continue;
        returnVector += Agents[i]->GetActorLocation();
    }
    returnVector /= (AGENT_COUNT - 1);
    returnVector = (returnVector - Agent->GetActorLocation()) / 100; // 100 is scale
    return returnVector;
}

FVector UFlockingManager::rule2(AAgent* Agent)
{
    FVector returnVector = FVector();
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        if (Agents[i] != Agent) {
            if ((Agents[i]->GetActorLocation() - Agent->GetActorLocation()).GetAbs().Size() < 100) { // 100 is the distance between boids
                returnVector -= (Agents[i]->GetActorLocation() - Agent->GetActorLocation());
            }
        }
    }
    return returnVector;
}

FVector UFlockingManager::rule3(AAgent* Agent)
{
    FVector returnVector = FVector();
    for (int i = 0; i < AGENT_COUNT; i++)
    {
        if (Agents[i] == Agent)
            continue;
        returnVector += Agents[i]->Velocity;
    }
    returnVector /= (AGENT_COUNT - 1);
    returnVector = (returnVector - Agent->Velocity) / 8; // 8 is the speed matching factor
    return returnVector;
}

FVector UFlockingManager::bound_position(AAgent* Agent, float distance)
{
    int Xmin = -500, Xmax = 500, Ymin = -500, Ymax = 500, Zmin = 0, Zmax = 1000;
    FVector returnVector = FVector();

    if (Agent->GetActorLocation().X < Xmin)
        returnVector.X = distance;
    else if (Agent->GetActorLocation().X > Xmax)
        returnVector.X = -distance;
    if (Agent->GetActorLocation().Y < Ymin)
        returnVector.Y = distance;
    else if (Agent->GetActorLocation().Y > Ymax)
        returnVector.Y = -distance;
    if (Agent->GetActorLocation().Z < Zmin)
        returnVector.Z = distance;
    else if (Agent->GetActorLocation().Z > Zmax)
        returnVector.Z = -distance;

    return returnVector;
}

void UFlockingManager::limit_velocity(AAgent* Agent, float vlim)
{
    if (Agent->Velocity.GetAbs().Size() > vlim) {
        Agent->Velocity = (Agent->Velocity / Agent->Velocity.GetAbs().Size()) * vlim;
    }
}
