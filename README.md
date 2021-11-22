# Unreal - ReBirth
  这是一款独立开发的游戏，目的是为了入门虚化4游戏引擎，更好的兼容BluePrint 与 C++的ue4开发，让我们懂得什么时候使用BluePrint进行开发，什么时候使用C++开发虚幻4。
  这是一件非常有意思的事情。当然你可以:
  `git clone https://github.com/Sugar0612/ReBirth.git` 或者 `git clone git@github.com:Sugar0612/ReBirth.git`来获取源码。
  
  ![image](https://img.shields.io/badge/Base-ue4-blue.svg)   ![image](https://img.shields.io/badge/Language-c++-orange.svg)
  
## 目前的进程

这是我在学习使用C++/BluePrint开发的时候的最近Demo。
![image](https://raw.githubusercontent.com/Sugar0612/ReBirth/main/image/ue4.gif)

## 目前涉及的一些有意思的功能

目前还是以C++开发为主。
- 关于各种组件的使用与声明 (UStaticMeshComponent, UCameraComponent, USphereComponent, UParticleSystemComponent...)，他们都需要
    ```cpp 
    CreateDefaultSubObject<class* T>(TEXT());
    ``` 
    来进行创建声明，也都需要 
    ```cpp
    SetupAttachment(GetComponent());
    ``` 
    来依赖于根组件，值得注意的是碰撞组件一般都会成为根组件即:
    ```cpp
    RootComponent = CollisionComponent;
    ```
- Tick函数 与 BeginPlay函数的使用 与 重写，重写一般的模板 `virtual T function() override;`
- 对于人物控制的一些机制: 继承APawn / ACharacter 来使用函数 
    ```cpp 
    SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
    ```
- 盒子的碰撞机制 
    ```cpp
    void T::BeginPlay() {
    /* trigger Box Bind Function */
      TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &T::BeginOverlap);
      TriggerBox->OnComponentEndOverlap.AddDynamic(this, &T::EndOverlap);
    }
    ```
  当然你也需要重写 BeginOverlap 和 EndOverlap
  ```cpp
    UFUNCTION()
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
  ```
- HUD窗口实现他在我的`MainPlayerController`中添加到游戏窗口的，并且在游戏模式`Player Controller Class`中设置他。
- 动画蓝图 继承与于 `UAnimInstance`, 定义一个函数
    ```cpp
    UFUNCTION(BlueprintCallable, Category = Animations)
    void UpdateAnimation();
    ```
  在里面写一些动作的机制从而让每个动作之间切换有判断的标准，再次之前在蓝图中用 `Event Blueprint Update Animation`来调用他。
- 武器装备 继承于 `AItem` 我让他专门为新的场景物体提供最基本的功能，如：粒子特效，MeshComponent，我们需要在人物骨架中提前添加好 `Socket` 这将成为武器插入的槽点。
  我们将定义对应的骨骼控件，用来载入武器模型
  ```cpp
  /* *武器骨骼控件 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal")
	class USkeletalMeshComponent* SkeletalComponent;
  ```
  创建后我们将武器插入槽中
  ```cpp
  void AWeapon::equipWeapon(AMain* player) {
       /* *忽略某些类型的碰撞 */
       SkeletalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
       SkeletalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
       SkeletalComponent->SetSimulatePhysics(false);

       /* *获得拿武器的槽口 */
       const USkeletalMeshSocket* WeaponSocket = player->GetMesh()->GetSocketByName("WeaponSocket");
       if (WeaponSocket) {

	       /* *如果获取成功那么附加在插槽上 */
       		WeaponSocket->AttachActor(this, player->GetMesh());
       }
   }
  ```
- Montage动画 总结了一下基本步骤：
    ``
    1.创建
    2.拖入动画
    3.分段
    4.创建 slot: 'combet'
    ``
  下来就是对 AnimInstance_BP 的设定，需要将 `State Machine` -> `StateMacheCached`, `Use 'StateMacheCached'` -> `Slot 'combet'` -> `Output Pose`,
  再按不同的标准对状态机进行修改。关于Montage在c++中的定义与使用:
  定义蒙太奇：
  ```cpp
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anims")
    class UAnimMontage* CombatMontage;
  ```
  调用不同片段的动画：
  ```cpp
    /* *获取蒙太奇的实例 */
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(CombatMontage, 1.2f);
	AnimInstance->Montage_JumpToSection(FName("NAME"), CombatMontage);
  ```
- Ai跟随/自动攻击 利用 `黑板 + 行为树 + AIContorller` 来实现AI的随机找点和看见玩家追击动作。在黑板中创建你需要的变量，变量是用来在行为树中进行 `Selector` 判断的，需要将
  黑板的变量附加在属性为 `Both` 的不同的 `Sequence` 上面来实现不同的 `Task` (你需要在行为树中自己去 `New Task`)。在AIController中首先需要设置Ai感知器官 `Ai Perception`
  为 `AI Sight Config`，接着在Blueprint中：
  ```cpp
  Event BeginPlay -> Run Behavior Tree
  Event On Target Perception Updated -> Cast To Main_BP -> Set View as Bool 
  (Set View as Bool.Target = Get Blackboard, Set View as Bool.Keyname = Make Litera Name(is_viewPlayer)) 
  (Event On Target Perception Updated.Stimulus = Break AIStimulus.AIStimulus)
  (Break AIStimulus.Successfully_Sensed = Set View as Bool.Bool Value)
  ```
  最后只需要在Monster_BP中设置 `AI Controller Class => AIController`，就可以实现你想要的动作了。 
  对于攻击动作的触发，我是选择在C++中设置盒子碰撞，并且创建了敌人的emun状态：
  ```cpp
    UENUM(BlueprintType)
    enum class EMonsterState : uint8 {
         EMS_Ldle UMETA(DeplayName = "Ldle"),
         EMS_MoveToTarget UMETA(DeplayName = "MoveToPlayer"),
         EMS_Attacking UMETA(DeplayName = "Attack"),
         EMS_Default UMETA(DeplayName = "Default")
    };
  ```
  碰撞盒子改变状态：
  ```cpp
    if (OtherActor) {
    	AMain* Player = Cast<AMain>(OtherActor);
        	if (Player) {
			if (MonsterController) {
				bisOverlap = true;
				SetMonsterState(EMonsterState::EMS_Attacking);
			}
    	}
    }
  ```
- 粒子系统, 粒子系统 `ParticleSyetem` 不是 粒子控件 `ParticleSystemComponent`，他们的区别在于粒子控件会一直附加存在，而粒子系统是在某种特定的机制下触发如：碰撞...。用代码播放粒子控件系统各有不同：  
  如果是武器模型，他们大多没有 `StaticMeshComponent` 而只有 `SkeletalMeshComponent`：  
  ```cpp
  const USkeletalMeshSocket* WeaponSocket = SkeletalComponent->GetSocketByName("WeaponSocket");
  if (WeaponSocket) {
      FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalComponent);
      UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), monster->BleedParticles, SocketLocation, FRotator(0.f), false);
  }
  ```
  如果是 Monster模型，我在他的手上创建了一个 Socket让他成为触发粒子系统的地方，因为他自带 `StaticMeshComponent`:
  ```cpp
  const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName("BloodSocket");
	//UE_LOG(LogTemp, Warning, TEXT(" Blood Socket Begin Blood! "));
	if (HandSocket) {
		FVector SocketLocation = HandSocket->GetSocketLocation(GetMesh());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), player->BloodParticles, SocketLocation, FRotator(0.f), false);
    }
  }
  ```
  前者我还需要在构造去声明以及定义：  
  ```cpp
  SkeletalComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
  SkeletalComponent->SetupAttachment(GetRootComponent());
  ```
- 攻击伤害的传递。有很多方法传递来自其他角色造成的伤害，但是 ue4 给了我们一套自己的伤害传递方式：  
  ```cpp
  /* *伤害机制 */
  virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
  ```
  你需要重写 `TackDamage` 函数：  
  ```cpp
  float AMonster::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
  {
	 ReduceHp(DamageTaken);

	 return DamageTaken;
  }
  ```
  当其他角色有对其他角色造成伤害的功能时：  
  需要先创建一个 `TSubclassOf<class T>` 这个是ue4固有的模板：
  ```cpp
  TSubclassOf<UDamageType> DamageTypeClass;
  ```
  然后在对角色造成伤害时调用他：  
  ```cpp
  if (DamageTypeClass) {
	 UGameplayStatics::ApplyDamage(monster, Attack_Power, nullptr, this, DamageTypeClass);
  }
  ```
  这样就会调用收到伤害角色的 `TackDamage`。
- 显示敌人的血条。这个比较简单首先还是先创建 HUD 然后在 `PlayerController` 中添加并显示在窗口中，问题是如何显示在敌人的正上方，并且有着合理的大小： 
  我们可以通过获得敌人的坐标然后 在类`PlayerController` 重写的 `Tick` 中不断获取新的坐标，再将其转换为屏幕的2d坐标显示就可以了：  
  ```cpp
  if (MonsterHpBar) {
	 FVector2D ScreenPosition;

	 /* *将3d转换为屏幕2d坐标 */
	 ProjectWorldLocationToScreen(showLocation, ScreenPosition);

	 FVector2D ProgressBarSize(200.f, 25.f);

	 MonsterHpBar->SetPositionInViewport(ScreenPosition);
	 MonsterHpBar->SetDesiredSizeInViewport(ProgressBarSize);
  }
  ```
- 保存和载入游戏。需要去继承一个新的c++类 `USaveGame`，你可以在里面建立一个结构体，让他来保存当前人物的信息。  
  比如像这样： 
  ```cpp
  USTRUCT(BlueprintType)
  struct FCharacterState {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurHp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurEp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxEp;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 CoinCnt;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;
  };
  ```
  那么现在我需要从 `MainPlayer` 中调用他并实现保存和载入信息。  
  你需要单独为保存和载入创建新的函数体：  
  ```cpp
  /* *保存游戏函数 */
  void AMain::SaveGame() {
	USaveMyGame* SaveGame =  Cast<USaveMyGame>(UGameplayStatics::CreateSaveGameObject(USaveMyGame::StaticClass()));
	SaveGame->CharacterState.CurHp = CurrentHp;
	SaveGame->CharacterState.MaxHp = MaxHp;
	SaveGame->CharacterState.CurEp = CurrentEp;
	SaveGame->CharacterState.MaxEp = MaxEp;
	SaveGame->CharacterState.CoinCnt = cntCoins;
	SaveGame->CharacterState.Location = GetActorLocation();
	SaveGame->CharacterState.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveGame->GameName, SaveGame->PlayerIndex);
  }

  /* *载入游戏函数 */
  void AMain::LoadGame(bool bLoad) {
	USaveMyGame* LoadGame = Cast<USaveMyGame>(UGameplayStatics::CreateSaveGameObject(USaveMyGame::StaticClass()));
	USaveMyGame* LoadGameInstance = Cast<USaveMyGame>(UGameplayStatics::LoadGameFromSlot(LoadGame->GameName, LoadGame->PlayerIndex)); //载入实例
	if (LoadGameInstance == nullptr) return;

	CurrentHp = LoadGameInstance->CharacterState.CurHp;
	MaxHp = LoadGameInstance->CharacterState.MaxHp;
	CurrentEp = LoadGameInstance->CharacterState.CurEp;
	MaxEp = LoadGameInstance->CharacterState.MaxEp;
	cntCoins = LoadGameInstance->CharacterState.CoinCnt;

    //进入新的关卡，并不需要载入在上个关卡的位置和旋转角度！
	if (bLoad) {
		SetActorLocation(LoadGameInstance->CharacterState.Location);
		SetActorRotation(LoadGameInstance->CharacterState.Rotation);
	}
  }
  ```

## 学习与交流
<img src = "https://raw.githubusercontent.com/Sugar0612/ReBirth/main/image/Wechat.png" width="500" alt="wechat">
