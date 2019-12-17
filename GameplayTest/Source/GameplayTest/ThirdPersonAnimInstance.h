#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ThirdPersonAnimInstance.generated.h"

UCLASS()
class GAMEPLAYTEST_API UThirdPersonAnimInstance : public UAnimInstance {
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "Animation Properties")
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementDirectionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementDirectionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float VelocityLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class AThirdPersonShooterCharacter* pCharacter;


	
};
