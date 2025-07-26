#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UAimHackComponent;
class USceneComponent;

UCLASS()
class YOURGAME_API AWeapon : public AActor
{
    GENERATED_BODY()
    
public:
    AWeapon();

    /** Fires the weapon, using camera view for aiming */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Fire();

protected:
    virtual void BeginPlay() override;

    /** Auto-aim component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAimHackComponent* AimHackComponent;

    /** Muzzle location component for spawning projectiles or raycast origin */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* MuzzlePoint;

    /** Default shooting range if auto-aim target not found */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float DefaultRange = 10000.f;
};
