// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSL_Window.h"
#include "GameFramework/Character.h"
#include "PlayerChr.generated.h"

UCLASS()
class VELO_API APlayerChr : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChr();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Sensitivity")
		float MouseSensitivity = 0.25f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Sensitivity")
		float SensitivityZ = 25.0f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Sensitivity")
		float SensitivityX = 45.0f;

	UFUNCTION(BlueprintPure)
		float GetMouseSens() const
		{
			UCSL_Window::PrintToConsole("Character", "Warning", FString::Printf(TEXT("Sensitivity: %f"), MouseSensitivity));
			return MouseSensitivity;	
		}
	UFUNCTION(BlueprintPure)
		float GetSensZ() const
		{
			UCSL_Window::PrintToConsole("Character", "Warning", FString::Printf(TEXT("Z Sensitivity: %f"), SensitivityZ));
			return SensitivityZ;
		}
	UFUNCTION(BlueprintPure)
		float GetSensX() const
		{
			UCSL_Window::PrintToConsole("Character", "Warning", FString::Printf(TEXT("X Sensitivity: %f"), SensitivityX));
			return SensitivityX;
		}

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Speed")
		float BaseWalkSpeed = 375.0f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Speed")
		float BaseWalkSpeedMultiplier = 2.2f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Speed")
		float BaseCrouchSpeed = 150.0f;

	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value);
	UFUNCTION(BlueprintCallable)
		void MoveSide(float Value);
	UFUNCTION(BlueprintCallable)
		void VerticalLook(float Axis);
	UFUNCTION(BlueprintCallable)
		void HorizontalLook(float Axis);

	bool bDevWalk = false;
	UFUNCTION(BlueprintCallable, Exec)
		void Noclip(int Mode); // 0 = disable, 1 = enable
};
