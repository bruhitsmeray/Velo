// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChr.h"

#include "CSL_Window.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerChr::APlayerChr()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.1f;

	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	Camera->SetWorldLocation(FVector(0,0,70));

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Camera);
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 12.0f;

	USpotLightComponent* InnerLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("InnerLight"));
	InnerLight->SetupAttachment(SpringArm);
	InnerLight->AttenuationRadius = 2500.0f;
	InnerLight->InnerConeAngle = 16.0f;
	InnerLight->OuterConeAngle = 24.0f;
	InnerLight->SetVisibility(false);

	USpotLightComponent* OuterLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("OuterLight"));
	OuterLight->SetupAttachment(SpringArm);
	OuterLight->Intensity = 2500.0f;
	OuterLight->AttenuationRadius = 2500.0f;
	OuterLight->InnerConeAngle = 32.0f;
	OuterLight->OuterConeAngle = 48.0f;
	OuterLight->SetVisibility(false);
	
//	NOTE: In previous versions of this plugin, the cable was spawned alongside the other components, from now on
//	the user has to create the cable component manually in Blueprints as it is easier to customize.	It's functionality
//	however stays inside the code so that the user doesn't need to worry about that stuff.	
}

// Called when the game starts or when spawned
void APlayerChr::BeginPlay()
{
	Super::BeginPlay();
	
}

bool APlayerChr::CanJumpInternal_Implementation() const
{
	return true;
}

// Called every frame
void APlayerChr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GrappleTick();
}

// Called to bind functionality to input
void APlayerChr::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(MoveForwardBindName, this, &APlayerChr::MoveForward);
	PlayerInputComponent->BindAxis(MoveSideBindName, this, &APlayerChr::MoveSide);
	PlayerInputComponent->BindAxis(VerticalLookBindName, this, &APlayerChr::VerticalLook);
	PlayerInputComponent->BindAxis(VerticalLookOnControllerBindName, this, &APlayerChr::VerticalLookOnController);
	PlayerInputComponent->BindAxis(HorizontalLookBindName, this, &APlayerChr::HorizontalLook);
	PlayerInputComponent->BindAxis(HorizontalLookOnControllerBindName, this, &APlayerChr::HorizontalLookOnController);

	PlayerInputComponent->BindAction(CrouchBindName, IE_Pressed, this, &APlayerChr::BeginCrouch);
	PlayerInputComponent->BindAction(CrouchBindName, IE_Released, this, &APlayerChr::EndCrouch);
	PlayerInputComponent->BindAction(GrappleBindName, IE_Pressed, this, &APlayerChr::Grapple);
	PlayerInputComponent->BindAction(GrappleBindName, IE_Released, this, &APlayerChr::GrappleStop);
}

void APlayerChr::MoveForward(float Value)
{
	if(Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerChr::MoveSide(float Value)
{
	if(Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerChr::VerticalLook(float Axis)
{
	AddControllerPitchInput(Axis * MouseSensitivity);
}

void APlayerChr::VerticalLookOnController(float Axis)
{
	AddControllerPitchInput(Axis * SensitivityY * GetWorld()->GetDeltaSeconds());
}

void APlayerChr::HorizontalLook(float Axis)
{
	AddControllerYawInput(Axis * MouseSensitivity);
}

void APlayerChr::HorizontalLookOnController(float Axis)
{
	AddControllerYawInput(Axis * SensitivityZ * GetWorld()->GetDeltaSeconds());
}

void APlayerChr::BeginJump()
{
	this->Jump();
}

void APlayerChr::EndJump()
{
	this->StopJumping();
}

void APlayerChr::BeginCrouch()
{
	this->Crouch();
}

void APlayerChr::EndCrouch()
{
	this->UnCrouch();
}

void APlayerChr::noclip(int Mode)
{
	if(bCanUseCheats)
	{
		if(!Mode && !bDevWalk || Mode == 1)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			SetActorEnableCollision(false);
			UCSL_Window::PrintToConsole("Character", "Warning", "noclip is now ON.");
		} else if(!Mode && bDevWalk || Mode == 0) {
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			SetActorEnableCollision(true);
			UCSL_Window::PrintToConsole("Character", "Warning", "noclip is now OFF.");
		}	
	}
}

void APlayerChr::AllowCheats(int Mode)
{
	if(Mode == 1)
	{
		bCanUseCheats = true;
		UCSL_Window::PrintToConsole("Character", "Notice", FString::Printf(TEXT("AllowCheats changed to %i"), Mode));
	} else if(Mode == 0) {
		bCanUseCheats = false;
		UCSL_Window::PrintToConsole("Character", "Notice", FString::Printf(TEXT("AllowCheats changed to %i"), Mode));
	}
}

bool APlayerChr::Trace(float Distance)
{
	if(UWorld* World = GetWorld())
	{
		APlayerCameraManager* ChrCamera = World->GetFirstPlayerController()->PlayerCameraManager;
		FVector CamWorldLocation = ChrCamera->GetCameraLocation();
		FVector CamForwardVec = ChrCamera->GetActorForwardVector() * Distance + CamWorldLocation;

		FCollisionQueryParams TraceParams;
		FHitResult HitResult;
		
		bool bDidTrace = GetWorld()->LineTraceSingleByChannel(HitResult, CamWorldLocation, CamForwardVec, ECC_Visibility, TraceParams);
		if(bDidTrace)
		{
			GrappleLocation = HitResult.Location;
			return HitResult.GetComponent()->IsSimulatingPhysics();
		} else {
			GrappleLocation = FVector(0,0,0);
			UCSL_Window::PrintToConsole("Character", "Error", "No hit registered.");
			return false;
		}
	}
	return nullptr;
}

void APlayerChr::Grapple()
{
	if(bCanUseHook)
	{
		Trace(GrappleDistance);
		if(!Trace(GrappleDistance))
		{
			bIsGrappling = true;
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			SetGrappleVisibility(true);
		}
	}
}

void APlayerChr::GrappleTick()
{
	if(bCanUseHook && bIsGrappling)
	{
		GetCharacterMovement()->AddForce(UKismetMathLibrary::Normal(UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), GrappleLocation) + ((GetInputAxisValue(MoveSideBindName) * GetActorRightVector()) * 0.5f)) * 300000.0f);
		UCableComponent* Cable = FindComponentByClass<UCableComponent>();
		Cable->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), GrappleLocation);
		if((UKismetMathLibrary::Vector_Distance(GrappleLocation, GetActorLocation()) <= 256.0f) || (UKismetMathLibrary::Vector_Distance(GrappleLocation, GetActorLocation()) >= 3072.0f))
		{
			GrappleStop();
		}
	}
}

void APlayerChr::GrappleStop()
{
	bIsGrappling = false;
	GrappleLocation = FVector(0, 0, 0);
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	SetGrappleVisibility(false);
}

void APlayerChr::SetGrappleVisibility(bool Visibility)
{
	UCableComponent* Cable = FindComponentByClass<UCableComponent>();
	if(Cable->IsValidLowLevel())
	{
		Cable->SetVisibility(Visibility);
	} else
	{
		UCSL_Window::PrintToConsole("Character", "Warning", "No cable component found! Please add a cable component to the Player Character blueprint in order to use the Grapple functions");
	}
}

