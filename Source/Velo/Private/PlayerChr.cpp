// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChr.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerChr::APlayerChr()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerChr::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerChr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChr::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("move_forward", this, &APlayerChr::MoveForward);
	PlayerInputComponent->BindAxis("move_side", this, &APlayerChr::MoveSide);
	PlayerInputComponent->BindAxis("look_vertical", this, &APlayerChr::VerticalLook);
	PlayerInputComponent->BindAxis("look_horizontal", this, &APlayerChr::HorizontalLook);
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

void APlayerChr::HorizontalLook(float Axis)
{
	AddControllerYawInput(Axis * MouseSensitivity);
}

void APlayerChr::Noclip(int Mode)
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

