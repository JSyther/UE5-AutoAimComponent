#include "Kismet/GameplayStatics.h" // Zaten include edilmiş olduğundan emin ol

void AWeapon::Fire()
{
    if (!GetOwner()) return;

    FVector CameraLocation;
    FVector CameraForward;

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;

    APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
    if (!PC) return;

    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
    CameraForward = CameraRotation.Vector();

    FVector AimLocation;
    bool bHasAutoAimTarget = false;

    if (AimHackComponent && AimHackComponent->bToggleAimHack)
    {
        bHasAutoAimTarget = AimHackComponent->GetAutoAimLocation(CameraLocation, CameraForward, AimLocation);
    }

    if (!bHasAutoAimTarget)
    {
        AimLocation = CameraLocation + CameraForward * DefaultRange;
    }

    FVector MuzzleLocation = MuzzlePoint ? MuzzlePoint->GetComponentLocation() : GetActorLocation();
    FVector ShotDirection = (AimLocation - MuzzleLocation).GetSafeNormal();

    FHitResult Hit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, MuzzleLocation, MuzzleLocation + ShotDirection * DefaultRange, ECC_Visibility, QueryParams);

    DrawDebugLine(GetWorld(), MuzzleLocation, MuzzleLocation + ShotDirection * DefaultRange, bHit ? FColor::Green : FColor::Red, false, 1.5f, 0, 2.0f);

    if (bHit && Hit.GetActor())
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *Hit.GetActor()->GetName());

        float DamageAmount = 20.f; // İstersen değiştirilebilir yap

        // Hasar uygulama
        UGameplayStatics::ApplyDamage(
            Hit.GetActor(),        // Hasar uygulanacak aktör
            DamageAmount,          // Hasar miktarı
            PC,                    // Hasar veren oyuncu kontrolcüsü
            this,                  // Hasar veren aktör (silah)
            nullptr                // Hasar tipi (default)
        );
    }
}
