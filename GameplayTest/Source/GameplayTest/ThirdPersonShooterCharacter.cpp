#include "ThirdPersonShooterCharacter.h"
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

	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	// Let's not rotate the character when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
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

	FVector LastFrameMovementInput = GetLastMovementInputVector();

	if (LastFrameMovementInput.X != 0 || LastFrameMovementInput.Y != 0) {
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	} else {
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
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

