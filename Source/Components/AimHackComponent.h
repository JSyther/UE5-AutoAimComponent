#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimHackComponent.generated.h"

/**
 * UAimHackComponent
 * 
 * This component provides optional auto-aim functionality that can be toggled via bToggleAimHack.
 * It calculates the nearest visible enemy within a cone and provides the "head" bone location as the aim target.
 * Designed to be used by weapon or character systems externally via GetAutoAimLocation().
 */
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class YOURGAME_API UAimHackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAimHackComponent();

    /** Enables or disables auto-aim functionality */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Aim")
    bool bToggleAimHack = false;

    /** Maximum aiming distance (used in ray tracing and cone detection) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Auto Aim")
    float AimRange = 10000.f;

    /** Half-cone angle in degrees used to limit auto-aim search direction */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Auto Aim")
    float AimConeAngle = 30.f;

    /**
     * Attempts to find an auto-aim target.
     * @param EyeLocation - Start location of the view.
     * @param ViewDirection - The normalized direction the owner is facing.
     * @param OutTargetLocation - Output: Location to aim at (e.g., head bone).
     * @return true if an auto-aim target was found.
     */
    bool GetAutoAimLocation(const FVector& EyeLocation, const FVector& ViewDirection, FVector& OutTargetLocation);

protected:
    virtual void BeginPlay() override;

private:
    /**
     * Internal helper to find the nearest valid enemy target within view cone.
     */
    AActor* FindNearestEnemyHeadInView(const FVector& EyeLocation, const FVector& ViewDirection) const;
};
