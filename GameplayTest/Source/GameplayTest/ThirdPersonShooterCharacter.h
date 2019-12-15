#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonShooterCharacter.generated.h"

UCLASS()
class GAMEPLAYTEST_API AThirdPersonShooterCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AThirdPersonShooterCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* pSpringArmComponent;
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return pSpringArmComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* pCameraComponent;
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return pCameraComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	float LastFrameUpValue;
	FORCEINLINE float GetLastFrameUpValue() const { return LastFrameUpValue; }

	float LastFrameRightValue;
	FORCEINLINE float GetLastFrameRightValue() const { return LastFrameRightValue; }

protected:
	virtual void BeginPlay() override;
	void MoveCharacterWithAxis(EAxis::Type AxisType, float Value);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

};
