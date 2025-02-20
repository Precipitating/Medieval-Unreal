// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Delegates/DelegateCombinations.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.generated.h"

// Delegate for ints
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntStatUpdated,
											   int32, OldValue,
											   int32, NewValue,
											   int32, MaxValue);

// Delegate for when the player dies
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerIsDead);

// Delegate for when stats based on floats are changed.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFloatStatUpdated,
											   float, OldValue,
											   float, NewValue,
											   float, MaxValue);


UCLASS()
class PLAYGROUND1_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Stamina_Functions

	// Return current stamina.
	UFUNCTION(BlueprintPure, Category = "Player Stamina")
	float GetStamina();
	UFUNCTION(BlueprintPure, Category = "Player Stamina")
	float GetMaxStamina();
	// Set player's stamina.
	UFUNCTION(BlueprintCallable, Category = "Player Stamina")
	void SetStamina(float Stamina);

	// Reduce Stamina
	UFUNCTION(BlueprintCallable, Category = "Player Stamina")
	void ReduceStamina(float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Player Stamina")
	void IncreaseStamina(float Stamina);
	// Set player's stamina recovery amount.
	UFUNCTION(BlueprintCallable, Category = "Player Stamina")
	void SetStaminaRecoveryValue(float Recovery);

	UFUNCTION(BlueprintCallable, Category = "Player Stamina")
	void RegenStamina();

	// Triggered when the player's stamina is updated.
	UPROPERTY(BlueprintAssignable, Category = "Player Stamina")
	FFloatStatUpdated OnStaminaUpdate;


#pragma endregion

#pragma region Action_GettersSetters
	UFUNCTION(BlueprintPure, Category = "Action GettersSetters")
	bool GetJumped() const;
	UFUNCTION(BlueprintPure, Category = "Action GettersSetters")
	bool GetWalking() const;
	UFUNCTION(BlueprintPure, Category = "Action GettersSetters")
	bool GetRunning() const;
	UFUNCTION(BlueprintPure, Category = "Action GettersSetters")
	bool GetCrouching() const;
	UFUNCTION(BlueprintPure, Category = "Action GettersSetters")
	bool GetKicked() const;

	UFUNCTION(BlueprintCallable, Category = "Action GettersSetters")
	void SetKicked(bool Value);

	UFUNCTION(BlueprintCallable, Category = "Action GettersSetters")
	bool IsExecuting();
# pragma endregion

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// On overlap (kick)
	void BeginKickOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

#pragma region Inputs

	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMap;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* CameraMovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* KickAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* AttackAction;

#pragma endregion

	// Player Actions
	void Movement(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump();
	void Crouch();
	void Kick();
	void SetSprint(bool IsSprinting);

	// Kick animation montage reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kick Animation")
	UAnimMontage* KickMontage;

private:
	// Camera
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera = nullptr;
	FVector CameraLocation =		 FVector(0.f, 30.f, 0.f);

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm = nullptr;

	// Speed
	static constexpr float WalkSpeed   = 300.f;
	static constexpr float SprintSpeed = WalkSpeed * 2.f;
	bool IsRunning = false;
	bool HasRan = false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool CanMove = true;

	// Stamina
	float				   CurrentStamina		 = MaxStamina;
	float				   StaminaRecoveryFactor = 3.f;
	static float constexpr MaxStamina			 = 100.f;
	static float constexpr CrouchRecovery		 = 10.f;
	bool				   HasJumped			 = false;
	float                  StaminaRegenDelay	 = 0.5f;
	FTimerHandle		   StaminaTimerHandle;

	// Action stamina cost
	static constexpr float SprintCost	   = 10.f;
	static constexpr float JumpCost		   = 25.f;
	static constexpr float KickCost		   = 30.f;
	bool HasKicked = false;




	// Cached components
	USkeletalMeshComponent* Mesh = nullptr;
	UFunction* KickEvent = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UActorComponent* AttackComp = nullptr;







};
