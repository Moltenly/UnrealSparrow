#include "ThirdPersonAnimInstance.h"
#include "ThirdPersonShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UThirdPersonAnimInstance::NativeInitializeAnimation() {
	if (pCharacter == nullptr) {
		APawn* pOwnerPawn = TryGetPawnOwner();

		if (pOwnerPawn) {
			pCharacter = Cast<AThirdPersonShooterCharacter>(pOwnerPawn);
		}
	}
}

void UThirdPersonAnimInstance::UpdateAnimationProperties() {
	if (pCharacter == nullptr) {
		return;
	}

	MovementDirectionX = pCharacter->GetLastFrameRightValue();
	MovementDirectionY = pCharacter->GetLastFrameUpValue();

	const FVector CharacterVelocity = pCharacter->GetVelocity();
	const FVector LateralVelocity = FVector(CharacterVelocity.X, CharacterVelocity.Y, 0.0f);
	VelocityLength = LateralVelocity.Size();
	bIsInAir = pCharacter->GetCharacterMovement()->IsFalling();
}

