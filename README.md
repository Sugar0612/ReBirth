# Unreal - ReBirth
  这是一款独立开发的游戏，目的是为了入门虚化4游戏引擎，更好的兼容BluePrint 与 C++开发，让我们懂得什么时候使用BluePrint进行开发，什么时候使用C++开发。
  这是一件非常有意思的事情。
  当然你可以 `git clone https://github.com/Sugar0612/ReBirth.git` 或者 `git clone git@github.com:Sugar0612/ReBirth.git`来获取源码。
  
  ![image](https://img.shields.io/badge/Base-ue4-blue.svg)   ![image](https://img.shields.io/badge/Language-c++-black.svg)
  
## 目前的进程

这是我在学习使用C++/BluePrint开发的时候的最近Demo。
![image](./image/ue4.gif)

## 目前涉及的一些有意思的功能

目前还是以C++开发为主。
- 关于各种组件的使用与声明 (UStaticMeshComponent, UCameraComponent, USphereComponent, UParticleSystemComponent...)，他们都需要使用 ```cpp CreateDefaultSubObject<class* T>(TEXT())``` 来进行创建声明，也都需要 ```cpp SetupAttachment(GetComponent())``` 来依赖于根组件，值得注意的是碰撞组件一般都会成为根组件即: ```cpp RootComponent = CollisionComponent;```
- Tick函数 与 BeginPlay函数的使用 与 重写，重写一般的模板 ```cpp virtual T function() override;```
- 对于人物控制的一些机制: 继承APawn / ACharacter 来使用函数 ```cpp SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) ```
- 盒子的碰撞机制 
  ```cpp
  void T::Begin() {
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
- 动画蓝图 继承与于 `UAnimInstance`, 写一个 `UpdateAnimation()`函数在里面写一些动作的机制从而让每个动作之间切换有判断的标准，再次之前在蓝图中用 `Event Blueprint Update Animation`来调用他，注意：要加入宏定义 `UFUNCTION(BlueprintCallable, Category = Animations)`。
- ...

## 交流与联系
你可以通过WeChat与我交流
![wechat](./image/Wechat.png)
