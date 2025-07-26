#include "AimHackComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

UAimHackComponent::UAimHackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAimHackComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UAimHackComponent::GetAutoAimLocation(const FVector& EyeLocation, const FVector& ViewDirection, FVector& OutTargetLocation)
{
    if (!bToggleAimHack || !GetOwner())
    {
        return false;
    }

    AActor* Target = FindNearestEnemyHeadInView(EyeLocation, ViewDirection);

    if (Target)
    {
        if (USkeletalMeshComponent* TargetMesh = Target->FindComponentByClass<USkeletalMeshComponent>())
        {
            if (TargetMesh->DoesSocketExist(FName("head")))
            {
                OutTargetLocation = TargetMesh->GetSocketLocation(FName("head"));

#if WITH_EDITOR
                DrawDebugSphere(GetWorld(), OutTargetLocation, 12.0f, 12, FColor::Yellow, false, 1.0f);
#endif

                return true;
            }
        }
    }

    return false;
}

AActor* UAimHackComponent::FindNearestEnemyHeadInView(const FVector& EyeLocation, const FVector& ViewDirection) const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    TArray<AActor*> AllCharacters;
    UGameplayStatics::GetAllActorsOfClass(World, ACharacter::StaticClass(), AllCharacters);

    AActor* BestTarget = nullptr;
    float ClosestAngle = AimConeAngle;

    for (AActor* Candidate : AllCharacters)
    {
        if (!Candidate || Candidate == GetOwner()) continue;

        FVector ToTarget = Candidate->GetActorLocation() - EyeLocation;
        float Distance = ToTarget.Size();
        FVector DirectionToTarget = ToTarget.GetSafeNormal();

        float AngleBetween = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ViewDirection, DirectionToTarget)));
        if (AngleBetween <= ClosestAngle && Distance <= AimRange)
        {
            if (USkeletalMeshComponent* Mesh = Candidate->FindComponentByClass<USkeletalMeshComponent>())
            {
                if (Mesh->DoesSocketExist(FName("head")))
                {
                    // Optional: Do a visibility check to head
                    FHitResult Hit;
                    FCollisionQueryParams Params;
                    Params.AddIgnoredActor(GetOwner());
                    Params.AddIgnoredActor(Candidate);

                    FVector HeadLoc = Mesh->GetSocketLocation(FName("head"));
                    bool bHit = World->LineTraceSingleByChannel(Hit, EyeLocation, HeadLoc, ECC_Visibility, Params);

                    if (!bHit || Hit.GetActor() == Candidate)
                    {
                        BestTarget = Candidate;
                        ClosestAngle = AngleBetween;
                    }
                }
            }
        }
    }

    return BestTarget;
}
