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

	MovementSpeedX = pCharacter->GetLastFrameRightValue();
	MovementSpeedY = pCharacter->GetLastFrameUpValue();
}

