#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonShooterCharacter.generated.h"


/*
	Bow State Enum
		=> BOW DOWN
			- High Velocity
			- No possibility of aiming
		=> AIMING
			- Medium Velocity
			- Can see the aim and prepare to shoot
		=> SHOOTING
			- Not possible not move
			- Can actually shoot when in this state
*/

UENUM(BlueprintType)
enum class EBowStatus : uint8 {
	EBS_BowDown				UMETA(DisplayName = "BowDown"),
	EBS_BowAiming			UMETA(DisplayName = "BowAiming"),
	EBS_FastShot			UMETA(DisplayName = "FastShot"),
	EBS_HeavyShot			UMETA(DisplayName = "HeavyShot"),
	EBS_MAX					UMETA(DisplayName = "DefautMAX")
};

UCLASS()
class GAMEPLAYTEST_API AThirdPersonShooterCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AThirdPersonShooterCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player State")
	EBowStatus CurrentBowStatus;
	FORCEINLINE EBowStatus GetCurrentBowStatus() const { return CurrentBowStatus; }
	void SetBowStatus(EBowStatus BowStatus);

	// Movement States Values
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement States")
	float BowDownPlayerVelocity = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement States")
	float BowAimingPlayerVelocity = 450.0f;

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

	// Animation Related Values
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation Values")
	float LastFrameUpValue;
	FORCEINLINE float GetLastFrameUpValue() const { return LastFrameUpValue; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation Values")
	float LastFrameRightValue;
	FORCEINLINE float GetLastFrameRightValue() const { return LastFrameRightValue; }

	// USER INTERFACE
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player HUD")
	TSubclassOf<class UUserWidget> HUDAimOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player HUD")
	UUserWidget* pHUDAimOverlay;
	*/

	/*
		ANIMATION MONTAGES
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting Animation Montages")
	class UAnimMontage* pPrimaryFireSlowMontage;

protected:
	virtual void BeginPlay() override;
	void MoveCharacterWithAxis(EAxis::Type AxisType, float Value);
	void ToggleDrawBowState();
	void StartShooting();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Arrow Shooting Handling
	// Fast Shooting

	UFUNCTION(BlueprintCallable)
	void EndShooting();

};
