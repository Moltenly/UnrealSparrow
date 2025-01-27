#include "ThirdPersonShooterCharacter.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

AThirdPersonShooterCharacter::AThirdPersonShooterCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	// Creating the Spring Arm Component
	pSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	pSpringArmComponent->SetupAttachment(GetRootComponent());
	pSpringArmComponent->TargetArmLength = 150; // This can be changed on the blueprint, there is no need on creating a variable for this value
	pSpringArmComponent->SocketOffset = FVector(0.0f, 60.0f, 50.0f); // This can also be changed on blueprint, this value was reached with experimentation
	pSpringArmComponent->bUsePawnControlRotation = true; // We want to rotate it based on the controller

	// Setting up the camera
	pCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	pCameraComponent->SetupAttachment(pSpringArmComponent, USpringArmComponent::SocketName);
	pCameraComponent->bUsePawnControlRotation = false;

	// Handling Rotation on the character
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true; // on bow down state character will orient rotation to movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // at this rate

	// Let's not rotate the character when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Movement Values (all these values were reached with experimentation)
	GetCharacterMovement()->MaxAcceleration = 1024.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 256.0f;

	// Jump Values (also experimentation)
	GetCharacterMovement()->MaxJumpApexAttemptsPerSimulation = 4;
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	// Handling User Interface Stuff...
	/*
	if (HUDAimOverlay) {
		pHUDAimOverlay = CreateWidget<UUserWidget>(this, HUDAimOverlay);

		if (pHUDAimOverlay) {
			pHUDAimOverlay->AddToViewport();
			// Initially it is hidden, because it is not shown on "BowDown" State
			pHUDAimOverlay->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	*/
}

void AThirdPersonShooterCharacter::BeginPlay() {
	Super::BeginPlay();

}

void AThirdPersonShooterCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AThirdPersonShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (PlayerInputComponent != nullptr) {
		// Binding Player Jump
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

		// Binding Movement
		PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonShooterCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonShooterCharacter::MoveRight);

		// Binding Camera Rotation
		PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
		
		// TODO do this /\ for controllers (I don't have a controller to test it currently feels bad)

		// Binding Draw Bow Action
		PlayerInputComponent->BindAction("DrawBow", EInputEvent::IE_Pressed, this, &AThirdPersonShooterCharacter::ToggleDrawBowState);

		// Shooting Bind Action
		PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AThirdPersonShooterCharacter::StartShooting);
	}

}

void AThirdPersonShooterCharacter::MoveForward(float Value) {
	LastFrameUpValue = Value;
	MoveCharacterWithAxis(EAxis::X, Value);
}

void AThirdPersonShooterCharacter::MoveRight(float Value) {
	LastFrameRightValue = Value;
	MoveCharacterWithAxis(EAxis::Y, Value);
}

void AThirdPersonShooterCharacter::MoveCharacterWithAxis(EAxis::Type AxisType, float Value) {
	if (Controller != nullptr && Value != 0.0f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(AxisType);
		AddMovementInput(Direction, Value);
	}
}


void AThirdPersonShooterCharacter::TurnAtRate(float Rate) {
	// TODO
	// this will be used for controller input
}

void AThirdPersonShooterCharacter::LookUpAtRate(float Rate) {
	// TODO
	// this will be used for controller input
}

/*
	Set the Current Bow Status for the Player Character
	- The only reason this function is not FORCEINLINE is that we might want to execute some behavior when going to some states
*/
void AThirdPersonShooterCharacter::SetBowStatus(EBowStatus BowStatus) {
	if (BowStatus == EBowStatus::EBS_BowDown) {
		GetCharacterMovement()->MaxWalkSpeed = BowAimingPlayerVelocity;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
	else if (BowStatus == EBowStatus::EBS_BowAiming) {
		GetCharacterMovement()->MaxWalkSpeed = BowDownPlayerVelocity;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}

	CurrentBowStatus = BowStatus;
}

void AThirdPersonShooterCharacter::ToggleDrawBowState() {
	if (CurrentBowStatus == EBowStatus::EBS_BowDown) {
		SetBowStatus(EBowStatus::EBS_BowAiming);
	} else if (CurrentBowStatus == EBowStatus::EBS_BowAiming) {
		SetBowStatus(EBowStatus::EBS_BowDown);
	}
}

void AThirdPersonShooterCharacter::StartShooting() {
	UAnimInstance* pCharacterAnimInstance = GetMesh()->GetAnimInstance();

	if (pCharacterAnimInstance != nullptr && pPrimaryFireSlowMontage != nullptr) {
		pCharacterAnimInstance->Montage_Play(pPrimaryFireSlowMontage, 2.0f);
		pCharacterAnimInstance->Montage_JumpToSection(FName("Default"), pPrimaryFireSlowMontage);
		DisableInput(GetWorld()->GetFirstPlayerController());
	}
}

void AThirdPersonShooterCharacter::EndShooting() {
	EnableInput(GetWorld()->GetFirstPlayerController());
}
