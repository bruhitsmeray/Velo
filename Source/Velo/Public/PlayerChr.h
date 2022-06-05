// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSL_Window.h"
#include "CableComponent.h"
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
	virtual bool CanJumpInternal_Implementation() const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName MoveForwardBindName = "move_forward";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName MoveSideBindName = "move_side";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName VerticalLookBindName = "look_vertical";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName VerticalLookOnControllerBindName = "LookRate";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName HorizontalLookBindName = "look_horizontal";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName HorizontalLookOnControllerBindName = "TurnRate";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName JumpBindName = "Jump";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName CrouchBindName = "Crouch";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bindings")
		FName GrappleBindName = "Grapple";
	

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
		float SensitivityY = 45.0f;

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
		float GetSensY() const
		{
			UCSL_Window::PrintToConsole("Character", "Warning", FString::Printf(TEXT("X Sensitivity: %f"), SensitivityY));
			return SensitivityY;
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
		void VerticalLookOnController(float Axis);
	UFUNCTION(BlueprintCallable)
		void HorizontalLook(float Axis);
	UFUNCTION(BlueprintCallable)
		void HorizontalLookOnController(float Axis);

	UFUNCTION(BlueprintCallable)
		void BeginJump();
	UFUNCTION(BlueprintCallable)
		void EndJump();
	UFUNCTION(BlueprintCallable)
		void BeginCrouch();
	UFUNCTION(BlueprintCallable)
		void EndCrouch();
	
	bool bDevWalk = false;
	UFUNCTION(BlueprintCallable, Exec, Category = "Cheats")
		void noclip(int Mode); // 0 = disable, 1 = enable
	UFUNCTION(BlueprintCallable, Exec, Category = "Cheats")
		void AllowCheats(int Mode); // 0 = disable, 1 = enable

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences")
		bool bCanUseCheats = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsGrabbing = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences")
		bool bCanGrab = true;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsCrouching = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsGrappling = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences")
		bool bCanUseHook = true;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsPulling = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsOnWall = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences")
		bool bCanUseWallrun = true;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsFlashOn = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences")
		bool bCanUseFlash = true;
	
protected:
//	UCableComponent* Cable;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character: Movement - Grapple")
		FVector GrappleLocation;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character: Movement - Grapple")
		float GrappleDistance = 3072.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Trace")
		bool Trace(float Distance);
	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
		void Grapple();
	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
		void GrappleTick();
	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
		void GrappleStop();
	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
	    void SetGrappleVisibility(bool Visibility);
};