//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_KismetTypes.h"

#include "Editor/BlueprintGraph/Public/BlueprintNodeSpawner.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Parser Default Reflected Classes:

#include "Runtime/Engine/Classes/GameFramework/Actor.h"										//AActor
#include "Runtime/Engine/Classes/Sound/AmbientSound.h"										//AAmbientSound
#include "Runtime/Engine/Classes/Engine/Brush.h"											//ABrush
#include "Runtime/Engine/Classes/Engine/BrushShape.h"										//ABrushShape
#include "Runtime/Engine/Classes/GameFramework/Volume.h"									//AVolume
#include "Runtime/Engine/Classes/Sound/AudioVolume.h"										//AAudioVolume
#include "Runtime/Engine/Classes/Engine/BlockingVolume.h"									//ABlockingVolume
#include "Runtime/Engine/Classes/GameFramework/CameraBlockingVolume.h"						//ACameraBlockingVolume
#include "Runtime/Engine/Classes/Engine/LevelStreamingVolume.h"								//ALevelStreamingVolume
#include "Runtime/Engine/Classes/Lightmass/LightmassCharacterIndirectDetailVolume.h"		//ALightmassCharacterIndirectDetailVolume
#include "Runtime/Engine/Classes/Lightmass/LightmassImportanceVolume.h"						//ALightmassImportanceVolume
#include "Runtime/Engine/Classes/Engine/MeshMergeCullingVolume.h"							//AMeshMergeCullingVolume
#include "Runtime/Engine/Classes/GameFramework/PhysicsVolume.h"								//APhysicsVolume
#include "Runtime/Engine/Classes/GameFramework/DefaultPhysicsVolume.h"						//ADefaultPhysicsVolume
#include "Runtime/Engine/Classes/GameFramework/KillZVolume.h"								//AKillZVolume
#include "Runtime/Engine/Classes/GameFramework/PainCausingVolume.h"							//APainCausingVolume
#include "Runtime/Engine/Classes/Engine/PostProcessVolume.h"								//APostProcessVolume
#include "Runtime/Engine/Classes/Lightmass/PrecomputedVisibilityOverrideVolume.h"			//APrecomputedVisibilityOverrideVolume
#include "Runtime/Engine/Classes/Lightmass/PrecomputedVisibilityVolume.h"					//APrecomputedVisibilityVolume
#include "Runtime/Engine/Classes/Engine/TriggerVolume.h"									//ATriggerVolume
#include "Runtime/Engine/Classes/Lightmass/VolumetricLightmapDensityVolume.h"				//AVolumetricLightmapDensityVolume
#include "Runtime/Engine/Classes/Camera/CameraActor.h"										//ACameraActor
#include "Runtime/Engine/Classes/GameFramework/Controller.h"								//AController
#include "Runtime/Engine/Classes/Engine/DebugCameraController.h"							//ADebugCameraController
#include "Runtime/Engine/Classes/Engine/DecalActor.h"										//ADecalActor
#include "Runtime/Engine/Classes/Engine/DocumentationActor.h"								//ADocumentationActor
#include "Runtime/Engine/Classes/Particles/Emitter.h"										//AEmitter
#include "Runtime/Engine/Classes/GameFramework/HUD.h"										//AHUD
#include "Runtime/Engine/Classes/Engine/DebugCameraHUD.h"									//ADebugCameraHUD
#include "Runtime/Engine/Classes/GameFramework/Info.h"										//AInfo
#include "Runtime/Engine/Classes/Engine/ExponentialHeightFog.h"								//AExponentialHeightFog
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"								//AGameModeBase
#include "Runtime/Engine/Classes/Particles/EmitterCameraLensEffectBase.h"					//AEmitterCameraLensEffectBase
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"									//AGameMode
#include "Runtime/Engine/Classes/GameFramework/GameNetworkManager.h"						//AGameNetworkManager
#include "Runtime/Engine/Classes/GameFramework/GameSession.h"								//AGameSession
#include "Runtime/Engine/Classes/GameFramework/GameStateBase.h"								//AGameStateBase
#include "Runtime/Engine/Classes/GameFramework/GameState.h"									//AGameState
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"								//APlayerState
#include "Runtime/Engine/Classes/Engine/ServerStatReplicator.h"								//AServerStatReplicator
#include "Runtime/Engine/Classes/Engine/SkyLight.h"											//ASkyLight
#include "Runtime/Engine/Classes/Engine/WindDirectionalSource.h"							//AWindDirectionalSource
#include "Runtime/Engine/Classes/GameFramework/WorldSettings.h"								//AWorldSettings
#include "Runtime/Engine/Classes/Engine/LevelBounds.h"										//ALevelBounds
#include "Runtime/Engine/Classes/Engine/LevelScriptActor.h"									//ALevelScriptActor
#include "Runtime/Engine/Classes/Engine/Light.h"											//ALight
#include "Runtime/Engine/Classes/Engine/DirectionalLight.h"									//ADirectionalLight
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"							//APlayerController
#include "Runtime/Engine/Classes/Engine/PointLight.h"										//APointLight
#include "Runtime/Engine/Classes/Engine/RectLight.h"										//ARectLight
#include "Runtime/Engine/Classes/Engine/SpotLight.h"										//ASpotLight
#include "Runtime/Engine/Classes/Engine/GeneratedMeshAreaLight.h"							//AGeneratedMeshAreaLight
#include "Runtime/Engine/Classes/Lightmass/LightmassPortal.h"								//ALightmassPortal
#include "Runtime/Engine/Classes/Engine/LODActor.h"											//ALODActor
#include "Runtime/Engine/Classes/Materials/MaterialInstanceActor.h"							//AMaterialInstanceActor
#include "Runtime/Engine/Classes/Matinee/MatineeActor.h"									//AMatineeActor
#include "Runtime/Engine/Classes/Matinee/MatineeActorCameraAnim.h"							//AMatineeActorCameraAnim
#include "Runtime/Engine/Classes/Engine/NavigationObjectBase.h"								//ANavigationObjectBase
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"								//APlayerStart
#include "Runtime/Engine/Classes/Engine/PlayerStartPIE.h"									//APlayerStartPIE
#include "Runtime/Engine/Classes/Engine/Note.h"												//ANote
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"										//APawn
#include "Runtime/Engine/Classes/GameFramework/Character.h"									//ACharacter
#include "Runtime/Engine/Classes/GameFramework/DefaultPawn.h"								//ADefaultPawn
#include "Runtime/Engine/Classes/GameFramework/SpectatorPawn.h"								//ASpectatorPawn
#include "Runtime/Engine/Classes/Camera/PlayerCameraManager.h"								//APlayerCameraManager
#include "Runtime/Engine/Classes/Engine/ReflectionCapture.h"								//AReflectionCapture
#include "Runtime/Engine/Classes/Engine/BoxReflectionCapture.h"								//ABoxReflectionCapture
#include "Runtime/Engine/Classes/Engine/PlaneReflectionCapture.h"							//APlaneReflectionCapture
#include "Runtime/Engine/Classes/Engine/SphereReflectionCapture.h"							//ASphereReflectionCapture
#include "Runtime/Engine/Classes/PhysicsEngine/RigidBodyBase.h"								//ARigidBodyBase
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintActor.h"					//APhysicsConstraintActor
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceActor.h"							//ARadialForceActor
#include "Runtime/Engine/Classes/Engine/SceneCapture.h"										//ASceneCapture
#include "Runtime/Engine/Classes/Engine/PlanarReflection.h"									//APlanarReflection
#include "Runtime/Engine/Classes/Engine/SceneCapture2D.h"									//ASceneCapture2D
#include "Runtime/Engine/Classes/Engine/SceneCaptureCube.h"									//ASceneCaptureCube
#include "Runtime/Engine/Classes/Animation/SkeletalMeshActor.h"								//ASkeletalMeshActor
#include "Runtime/Engine/Classes/Engine/SplineMeshActor.h"									//ASplineMeshActor
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"									//AStaticMeshActor
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"										//ATargetPoint
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"									//ATextRenderActor
#include "Runtime/Engine/Classes/Engine/TriggerBase.h"										//ATriggerBase
#include "Runtime/Engine/Classes/Engine/TriggerBox.h"										//ATriggerBox
#include "Runtime/Engine/Classes/Engine/TriggerCapsule.h"									//ATriggerCapsule
#include "Runtime/Engine/Classes/Engine/TriggerSphere.h"									//ATriggerSphere
#include "Runtime/Engine/Classes/VectorField/VectorFieldVolume.h"							//AVectorFieldVolume

#include "Runtime/Engine/Classes/Components/ApplicationLifecycleComponent.h"				//UApplicationLifecycleComponent
#include "Runtime/Engine/Classes/Components/InputComponent.h"								//UInputComponent
#include "Runtime/Engine/Classes/GameFramework/MovementComponent.h"							//UMovementComponent
#include "Runtime/Engine/Classes/Components/InterpToMovementComponent.h"					//UInterpToMovementComponent
#include "Runtime/Engine/Classes/GameFramework/NavMovementComponent.h"						//UNavMovementComponent
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"						//UPawnMovementComponent
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"				//UCharacterMovementComponent
#include "Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h"						//UFloatingPawnMovement
#include "Runtime/Engine/Classes/GameFramework/SpectatorPawnMovement.h"						//USpectatorPawnMovement
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"				//UProjectileMovementComponent
#include "Runtime/Engine/Classes/GameFramework/RotatingMovementComponent.h"					//URotatingMovementComponent
#include "Runtime/Engine/Classes/Components/PawnNoiseEmitterComponent.h"					//UPawnNoiseEmitterComponent
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicalAnimationComponent.h"				//UPhysicalAnimationComponent
#include "Runtime/Engine/Classes/Components/PlatformEventsComponent.h"						//UPlatformEventsComponent
#include "Runtime/Engine/Classes/Components/SceneComponent.h"								//USceneComponent
#include "Runtime/Engine/Classes/Atmosphere/AtmosphericFogComponent.h"						//UAtmosphericFogComponent
#include "Runtime/Engine/Classes/Components/AudioComponent.h"								//UAudioComponent
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"									//UCameraComponent
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"							//UChildActorComponent
#include "Runtime/Engine/Classes/Components/DecalComponent.h"								//UDecalComponent
#include "Runtime/Engine/Classes/Components/ExponentialHeightFogComponent.h"				//UExponentialHeightFogComponent
#include "Runtime/Engine/Classes/Components/ForceFeedbackComponent.h"						//UForceFeedbackComponent
#include "Runtime/Engine/Classes/Components/LightComponentBase.h"							//ULightComponentBase
#include "Runtime/Engine/Classes/Components/LightComponent.h"								//ULightComponent
#include "Runtime/Engine/Classes/Components/DirectionalLightComponent.h"					//UDirectionalLightComponent
#include "Runtime/Engine/Classes/Components/LocalLightComponent.h"							//ULocalLightComponent
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"							//UPointLightComponent
#include "Runtime/Engine/Classes/Components/SpotLightComponent.h"							//USpotLightComponent
#include "Runtime/Engine/Classes/Components/RectLightComponent.h"							//URectLightComponent
#include "Runtime/Engine/Classes/Components/SkyLightComponent.h"							//USkyLightComponent
#include "Runtime/Engine/Classes/Components/LightmassPortalComponent.h"						//ULightmassPortalComponent
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"				//UPhysicsConstraintComponent
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsSpringComponent.h"					//UPhysicsSpringComponent
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsThrusterComponent.h"					//UPhysicsThrusterComponent
#include "Runtime/Engine/Classes/Components/PostProcessComponent.h"							//UPostProcessComponent
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"							//UPrimitiveComponent
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"								//UArrowComponent
#include "Runtime/Engine/Classes/Components/BillboardComponent.h"							//UBillboardComponent
#include "Runtime/Engine/Classes/Components/BrushComponent.h"								//UBrushComponent
#include "Runtime/Engine/Classes/Components/DrawFrustumComponent.h"							//UDrawFrustumComponent
#include "Runtime/Engine/Classes/Components/LineBatchComponent.h"							//ULineBatchComponent
#include "Runtime/Engine/Classes/Components/MaterialBillboardComponent.h"					//UMaterialBillboardComponent
#include "Runtime/Engine/Classes/Components/MeshComponent.h"								//UMeshComponent
#include "Runtime/Engine/Classes/Components/SkinnedMeshComponent.h"							//USkinnedMeshComponent
#include "Runtime/Engine/Classes/Components/PoseableMeshComponent.h"						//UPoseableMeshComponent
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"						//USkeletalMeshComponent
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"							//UStaticMeshComponent
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"					//UInstancedStaticMeshComponent
#include "Runtime/Engine/Classes/Components/HierarchicalInstancedStaticMeshComponent.h"		//UHierarchicalInstancedStaticMeshComponent
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"							//USplineMeshComponent
#include "Runtime/Engine/Classes/Components/ModelComponent.h"								//UModelComponent
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"						//UParticleSystemComponent
#include "Runtime/Engine/Classes/Components/ShapeComponent.h"								//UShapeComponent
#include "Runtime/Engine/Classes/Components/BoxComponent.h"									//UBoxComponent
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"								//UCapsuleComponent
#include "Runtime/Engine/Classes/Components/SphereComponent.h"								//USphereComponent
#include "Runtime/Engine/Classes/Components/DrawSphereComponent.h"							//UDrawSphereComponent
#include "Runtime/Engine/Classes/Components/SplineComponent.h"								//USplineComponent
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"							//UTextRenderComponent
#include "Runtime/Engine/Classes/Components/VectorFieldComponent.h"							//UVectorFieldComponent
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"						//URadialForceComponent
#include "Runtime/Engine/Classes/Components/ReflectionCaptureComponent.h"					//UReflectionCaptureComponent
#include "Runtime/Engine/Classes/Components/BoxReflectionCaptureComponent.h"				//UBoxReflectionCaptureComponent
#include "Runtime/Engine/Classes/Components/PlaneReflectionCaptureComponent.h"				//UPlaneReflectionCaptureComponent
#include "Runtime/Engine/Classes/Components/SphereReflectionCaptureComponent.h"				//USphereReflectionCaptureComponent
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent.h"						//USceneCaptureComponent
#include "Runtime/Engine/Classes/Components/PlanarReflectionComponent.h"					//UPlanarReflectionComponent
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"						//USceneCaptureComponent2D
#include "Runtime/Engine/Classes/Components/SceneCaptureComponentCube.h"					//USceneCaptureComponentCube
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"						//USpringArmComponent
#include "Runtime/Engine/Classes/Components/StereoLayerComponent.h"							//UStereoLayerComponent
#include "Runtime/Engine/Classes/Components/WindDirectionalSourceComponent.h"				//UWindDirectionalSourceComponent
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"							//UTimelineComponent

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UMGC_KeywordDB::UMGC_KeywordDB() {
	ScriptCore.Add(TEXT("void"));
	ScriptCore.Add(TEXT("alignas"));
	ScriptCore.Add(TEXT("alignof"));
	ScriptCore.Add(TEXT("and"));
	ScriptCore.Add(TEXT("and_eq"));
	ScriptCore.Add(TEXT("asm"));
	ScriptCore.Add(TEXT("auto"));
	ScriptCore.Add(TEXT("bitand"));
	ScriptCore.Add(TEXT("bitor"));
	ScriptCore.Add(TEXT("break"));
	ScriptCore.Add(TEXT("case"));
	ScriptCore.Add(TEXT("catch"));
	ScriptCore.Add(TEXT("char"));
	ScriptCore.Add(TEXT("char16_t"));
	ScriptCore.Add(TEXT("char32_t"));
	ScriptCore.Add(TEXT("class"));
	ScriptCore.Add(TEXT("compl"));
	ScriptCore.Add(TEXT("concept"));
	ScriptCore.Add(TEXT("const"));
	ScriptCore.Add(TEXT("consteval"));
	ScriptCore.Add(TEXT("constexpr"));
	ScriptCore.Add(TEXT("const_cast"));
	ScriptCore.Add(TEXT("continue"));
	ScriptCore.Add(TEXT("decltype"));
	ScriptCore.Add(TEXT("default"));
	ScriptCore.Add(TEXT("delete"));
	ScriptCore.Add(TEXT("do"));
	ScriptCore.Add(TEXT("dynamic_cast"));
	ScriptCore.Add(TEXT("else"));
	ScriptCore.Add(TEXT("enum"));
	ScriptCore.Add(TEXT("explicit"));
	ScriptCore.Add(TEXT("export"));
	ScriptCore.Add(TEXT("extern"));
	ScriptCore.Add(TEXT("false"));
	ScriptCore.Add(TEXT("for"));
	ScriptCore.Add(TEXT("friend"));
	ScriptCore.Add(TEXT("goto"));
	ScriptCore.Add(TEXT("if"));
	ScriptCore.Add(TEXT("inline"));
	ScriptCore.Add(TEXT("long"));
	ScriptCore.Add(TEXT("mutable"));
	ScriptCore.Add(TEXT("namespace"));
	ScriptCore.Add(TEXT("new"));
	ScriptCore.Add(TEXT("noexcept"));
	ScriptCore.Add(TEXT("not"));
	ScriptCore.Add(TEXT("not_eq"));
	ScriptCore.Add(TEXT("nullptr"));
	ScriptCore.Add(TEXT("operator"));
	ScriptCore.Add(TEXT("override"));
	ScriptCore.Add(TEXT("or"));
	ScriptCore.Add(TEXT("or_eq"));
	ScriptCore.Add(TEXT("private"));
	ScriptCore.Add(TEXT("protected"));
	ScriptCore.Add(TEXT("public"));
	ScriptCore.Add(TEXT("register"));
	ScriptCore.Add(TEXT("reinterpret_cast"));
	ScriptCore.Add(TEXT("requires"));
	ScriptCore.Add(TEXT("return"));
	ScriptCore.Add(TEXT("short"));
	ScriptCore.Add(TEXT("signed"));
	ScriptCore.Add(TEXT("sizeof"));
	ScriptCore.Add(TEXT("static"));
	ScriptCore.Add(TEXT("static_assert"));
	ScriptCore.Add(TEXT("static_cast"));
	ScriptCore.Add(TEXT("struct"));
	ScriptCore.Add(TEXT("switch"));
	ScriptCore.Add(TEXT("template"));
	ScriptCore.Add(TEXT("this"));
	ScriptCore.Add(TEXT("throw"));
	ScriptCore.Add(TEXT("true"));
	ScriptCore.Add(TEXT("try"));
	ScriptCore.Add(TEXT("typedef"));
	ScriptCore.Add(TEXT("typeid"));
	ScriptCore.Add(TEXT("typename"));
	ScriptCore.Add(TEXT("union"));
	ScriptCore.Add(TEXT("unsigned"));
	ScriptCore.Add(TEXT("using"));
	ScriptCore.Add(TEXT("virtual"));
	ScriptCore.Add(TEXT("void"));
	ScriptCore.Add(TEXT("volatile"));
	ScriptCore.Add(TEXT("wchar_t"));
	ScriptCore.Add(TEXT("while"));
	ScriptCore.Add(TEXT("xor"));
	ScriptCore.Add(TEXT("xor_eq"));
	//
	Operators.Add(TEXT(">>="));
	Operators.Add(TEXT("<<="));
	Operators.Add(TEXT("/*"));
	Operators.Add(TEXT("*/"));
	Operators.Add(TEXT("//"));
	Operators.Add(TEXT("\""));
	Operators.Add(TEXT("\'"));
	Operators.Add(TEXT("::"));
	Operators.Add(TEXT("+="));
	Operators.Add(TEXT("++"));
	Operators.Add(TEXT("--"));
	Operators.Add(TEXT("-="));
	Operators.Add(TEXT("->"));
	Operators.Add(TEXT("!="));
	Operators.Add(TEXT("&="));
	Operators.Add(TEXT("*="));
	Operators.Add(TEXT("->"));
	Operators.Add(TEXT("/="));
	Operators.Add(TEXT("%="));
	Operators.Add(TEXT("<<"));
	Operators.Add(TEXT("<="));
	Operators.Add(TEXT(">>"));
	Operators.Add(TEXT(">="));
	Operators.Add(TEXT("=="));
	Operators.Add(TEXT("&&"));
	Operators.Add(TEXT("^="));
	Operators.Add(TEXT("|="));
	Operators.Add(TEXT("||"));
	Operators.Add(TEXT("-"));
	Operators.Add(TEXT("("));
	Operators.Add(TEXT(")"));
	Operators.Add(TEXT("["));
	Operators.Add(TEXT("]"));
	Operators.Add(TEXT("."));
	Operators.Add(TEXT("+"));
	Operators.Add(TEXT(":"));
	Operators.Add(TEXT("!"));
	Operators.Add(TEXT("~"));
	Operators.Add(TEXT("&"));
	Operators.Add(TEXT("*"));
	Operators.Add(TEXT("/"));
	Operators.Add(TEXT("%"));
	Operators.Add(TEXT("<"));
	Operators.Add(TEXT(">"));
	Operators.Add(TEXT("&"));
	Operators.Add(TEXT("^"));
	Operators.Add(TEXT("|"));
	Operators.Add(TEXT("?"));
	Operators.Add(TEXT("="));
	Operators.Add(TEXT(","));
	Operators.Add(TEXT("{"));
	Operators.Add(TEXT("}"));
	Operators.Add(TEXT(";"));
	//
	Processors.Add(TEXT("#pragma once"));
	Processors.Add(TEXT("#include"));
	Processors.Add(TEXT("#pragma"));
	Processors.Add(TEXT("#ifndef"));
	Processors.Add(TEXT("#define"));
	Processors.Add(TEXT("#error"));
	Processors.Add(TEXT("#undef"));
	Processors.Add(TEXT("#ifdef"));
	Processors.Add(TEXT("#endif"));
	Processors.Add(TEXT("#else"));
	Processors.Add(TEXT("#elif"));
	Processors.Add(TEXT("#if"));
	//
	Macros.Add(TEXT("NULL"));
	Macros.Add(TEXT("TEXT"));
	Macros.Add(TEXT("UCLASS"));
	Macros.Add(TEXT("UPROPERTY"));
	Macros.Add(TEXT("UFUNCTION"));
	Macros.Add(TEXT("USTRUCT"));
	Macros.Add(TEXT("UMETA"));
	Macros.Add(TEXT("UPARAM"));
	Macros.Add(TEXT("UENUM"));
	Macros.Add(TEXT("UDELEGATE"));
	Macros.Add(TEXT("GENERATED_BODY"));
	Macros.Add(TEXT("GENERATED_USTRUCT_BODY"));
	Macros.Add(TEXT("GENERATED_UCLASS_BODY"));
	Macros.Add(TEXT("GENERATED_UINTERFACE_BODY"));
	Macros.Add(TEXT("GENERATED_IINTERFACE_BODY"));
	Macros.Add(TEXT("WITH_EDITORONLY_DATA"));
	Macros.Add(TEXT("WITH_HOT_RELOAD"));
	Macros.Add(TEXT("UINTERFACE"));
	Macros.Add(TEXT("WITH_EDITOR"));
	Macros.Add(TEXT("classGroup"));
	Macros.Add(TEXT("Within"));
	Macros.Add(TEXT("BlueprintType"));
	Macros.Add(TEXT("NotBlueprintType"));
	Macros.Add(TEXT("Blueprintable"));
	Macros.Add(TEXT("NotBlueprintable"));
	Macros.Add(TEXT("MinimalAPI"));
	Macros.Add(TEXT("customConstructor"));
	Macros.Add(TEXT("Intrinsic"));
	Macros.Add(TEXT("noexport"));
	Macros.Add(TEXT("placeable"));
	Macros.Add(TEXT("notplaceable"));
	Macros.Add(TEXT("DefaultToInstanced"));
	Macros.Add(TEXT("Abstract"));
	Macros.Add(TEXT("deprecated"));
	Macros.Add(TEXT("Transient"));
	Macros.Add(TEXT("nonTransient"));
	Macros.Add(TEXT("config"));
	Macros.Add(TEXT("perObjectConfig"));
	Macros.Add(TEXT("configdonotcheckdefaults"));
	Macros.Add(TEXT("defaultconfig"));
	Macros.Add(TEXT("editinlinenew"));
	Macros.Add(TEXT("noteditinlinenew"));
	Macros.Add(TEXT("hidedropdown"));
	Macros.Add(TEXT("showCategories"));
	Macros.Add(TEXT("hideCategories"));
	Macros.Add(TEXT("ComponentWrapperClass"));
	Macros.Add(TEXT("showFunctions"));
	Macros.Add(TEXT("hideFunctions"));
	Macros.Add(TEXT("autoExpandCategories"));
	Macros.Add(TEXT("autoCollapseCategories"));
	Macros.Add(TEXT("dontAutoCollapseCategories"));
	Macros.Add(TEXT("collapseCategories"));
	Macros.Add(TEXT("dontCollapseCategories"));
	Macros.Add(TEXT("AdvancedClassDisplay"));
	Macros.Add(TEXT("ConversionRoot"));
	Macros.Add(TEXT("Experimental"));
	Macros.Add(TEXT("EarlyAccessPreview"));
	Macros.Add(TEXT("BlueprintImplementableEvent"));
	Macros.Add(TEXT("BlueprintNativeEvent"));
	Macros.Add(TEXT("SealedEvent"));
	Macros.Add(TEXT("Exec"));
	Macros.Add(TEXT("Server"));
	Macros.Add(TEXT("Client"));
	Macros.Add(TEXT("NetMulticast"));
	Macros.Add(TEXT("Reliable"));
	Macros.Add(TEXT("Unreliable"));
	Macros.Add(TEXT("BlueprintPure"));
	Macros.Add(TEXT("BlueprintCallable"));
	Macros.Add(TEXT("BlueprintGetter"));
	Macros.Add(TEXT("BlueprintSetter"));
	Macros.Add(TEXT("BlueprintAuthorityOnly"));
	Macros.Add(TEXT("BlueprintCosmetic"));
	Macros.Add(TEXT("BlueprintInternalUseOnly"));
	Macros.Add(TEXT("CallInEditor"));
	Macros.Add(TEXT("CustomThunk"));
	Macros.Add(TEXT("Category"));
	Macros.Add(TEXT("WithValidation"));
	Macros.Add(TEXT("ServiceRequest"));
	Macros.Add(TEXT("ServiceResponse"));
	Macros.Add(TEXT("Config"));
	Macros.Add(TEXT("GlobalConfig"));
	Macros.Add(TEXT("Localized"));
	Macros.Add(TEXT("Transient"));
	Macros.Add(TEXT("DuplicateTransient"));
	Macros.Add(TEXT("NonPIETransient"));
	Macros.Add(TEXT("NonPIEDuplicateTransient"));
	Macros.Add(TEXT("Ref"));
	Macros.Add(TEXT("meta"));
	Macros.Add(TEXT("Export"));
	Macros.Add(TEXT("NoClear"));
	Macros.Add(TEXT("EditFixedSize"));
	Macros.Add(TEXT("Replicated"));
	Macros.Add(TEXT("ReplicatedUsing"));
	Macros.Add(TEXT("NotReplicated"));
	Macros.Add(TEXT("Interp"));
	Macros.Add(TEXT("NonTransactional"));
	Macros.Add(TEXT("Instanced"));
	Macros.Add(TEXT("BlueprintAssignable"));
	Macros.Add(TEXT("SimpleDisplay"));
	Macros.Add(TEXT("AdvancedDisplay"));
	Macros.Add(TEXT("EditAnywhere"));
	Macros.Add(TEXT("EditInstanceOnly"));
	Macros.Add(TEXT("EditDefaultsOnly"));
	Macros.Add(TEXT("VisibleAnywhere"));
	Macros.Add(TEXT("VisibleInstanceOnly"));
	Macros.Add(TEXT("VisibleDefaultsOnly"));
	Macros.Add(TEXT("BlueprintReadOnly"));
	Macros.Add(TEXT("BlueprintGetter"));
	Macros.Add(TEXT("BlueprintReadWrite"));
	Macros.Add(TEXT("BlueprintSetter"));
	Macros.Add(TEXT("AssetRegistrySearchable"));
	Macros.Add(TEXT("SaveGame"));
	Macros.Add(TEXT("BlueprintCallable"));
	Macros.Add(TEXT("BlueprintAuthorityOnly"));
	Macros.Add(TEXT("TextExportTransient"));
	Macros.Add(TEXT("SkipSerialization"));
	Macros.Add(TEXT("NoExport"));
	Macros.Add(TEXT("Atomic"));
	Macros.Add(TEXT("Immutable"));
	Macros.Add(TEXT("ToolTip"));
	Macros.Add(TEXT("ShortTooltip"));
	Macros.Add(TEXT("BlueprintSpawnableComponent"));
	Macros.Add(TEXT("ChildCanTick"));
	Macros.Add(TEXT("ChildCannotTick"));
	Macros.Add(TEXT("IgnoreCategoryKeywordsInSubclasses"));
	Macros.Add(TEXT("DeprecatedNode"));
	Macros.Add(TEXT("DeprecationMessage"));
	Macros.Add(TEXT("DisplayName"));
	Macros.Add(TEXT("ScriptName"));
	Macros.Add(TEXT("IsBlueprintBase"));
	Macros.Add(TEXT("KismetHideOverrides"));
	Macros.Add(TEXT("ProhibitedInterfaces"));
	Macros.Add(TEXT("RestrictedToClasses"));
	Macros.Add(TEXT("ShowWorldContextPin"));
	Macros.Add(TEXT("DontUseGenericSpawnObject"));
	Macros.Add(TEXT("ExposedAsyncProxy"));
	Macros.Add(TEXT("BlueprintThreadSafe"));
	Macros.Add(TEXT("UsesHierarchy"));
	Macros.Add(TEXT("HasNativeBreak"));
	Macros.Add(TEXT("HasNativeMake"));
	Macros.Add(TEXT("HiddenByDefault"));
	Macros.Add(TEXT("AllowAbstract"));
	Macros.Add(TEXT("AllowedClasses"));
	Macros.Add(TEXT("AllowPreserveRatio"));
	Macros.Add(TEXT("ArrayClamp"));
	Macros.Add(TEXT("AssetBundles"));
	Macros.Add(TEXT("BlueprintBaseOnly"));
	Macros.Add(TEXT("BlueprintCompilerGeneratedDefaults"));
	Macros.Add(TEXT("ClampMin"));
	Macros.Add(TEXT("ClampMax"));
	Macros.Add(TEXT("ConfigHierarchyEditable"));
	Macros.Add(TEXT("ContentDir"));
	Macros.Add(TEXT("DeprecatedProperty"));
	Macros.Add(TEXT("ScriptNoExport"));
	Macros.Add(TEXT("DisplayThumbnail"));
	Macros.Add(TEXT("EditCondition"));
	Macros.Add(TEXT("EditFixedOrder"));
	Macros.Add(TEXT("ExactClass"));
	Macros.Add(TEXT("ExposeFunctionCategories"));
	Macros.Add(TEXT("ExposeOnSpawn"));
	Macros.Add(TEXT("FilePathFilter"));
	Macros.Add(TEXT("FixedIncrement"));
	Macros.Add(TEXT("HideAlphaChannel"));
	Macros.Add(TEXT("HideViewOptions"));
	Macros.Add(TEXT("InlineEditConditionToggle"));
	Macros.Add(TEXT("LongPackageName"));
	Macros.Add(TEXT("MakeEditWidget"));
	Macros.Add(TEXT("MakeStructureDefaultValue"));
	Macros.Add(TEXT("MetaClass"));
	Macros.Add(TEXT("MustImplement"));
	Macros.Add(TEXT("Multiple"));
	Macros.Add(TEXT("MultiLine"));
	Macros.Add(TEXT("PasswordField"));
	Macros.Add(TEXT("NoElementDuplicate"));
	Macros.Add(TEXT("NoResetToDefault"));
	Macros.Add(TEXT("NoSpinbox"));
	Macros.Add(TEXT("OnlyPlaceable"));
	Macros.Add(TEXT("RelativePath"));
	Macros.Add(TEXT("RelativeToGameContentDir"));
	Macros.Add(TEXT("ShowOnlyInnerProperties"));
	Macros.Add(TEXT("ShowTreeView"));
	Macros.Add(TEXT("SliderExponent"));
	Macros.Add(TEXT("UIMin"));
	Macros.Add(TEXT("UIMax"));
	Macros.Add(TEXT("NeverAsPin"));
	Macros.Add(TEXT("PinHiddenByDefault"));
	Macros.Add(TEXT("PinShownByDefault"));
	Macros.Add(TEXT("AlwaysAsPin"));
	Macros.Add(TEXT("CustomizeProperty"));
	Macros.Add(TEXT("OverridingInputProperty"));
	Macros.Add(TEXT("RequiredInput"));
	Macros.Add(TEXT("ArrayParm"));
	Macros.Add(TEXT("ArrayTypeDependentParams"));
	Macros.Add(TEXT("AutoCreateRefTerm"));
	Macros.Add(TEXT("BlueprintInternalUseOnly"));
	Macros.Add(TEXT("BlueprintProtected"));
	Macros.Add(TEXT("CallableWithoutWorldContext"));
	Macros.Add(TEXT("CommutativeAssociativeBinaryOperator"));
	Macros.Add(TEXT("CompactNodeTitle"));
	Macros.Add(TEXT("CustomStructureParam"));
	Macros.Add(TEXT("DefaultToSelf"));
	Macros.Add(TEXT("DeprecatedFunction"));
	Macros.Add(TEXT("ExpandEnumAsExecs"));
	Macros.Add(TEXT("ScriptMethod"));
	Macros.Add(TEXT("ScriptMethodSelfReturn"));
	Macros.Add(TEXT("ScriptOperator"));
	Macros.Add(TEXT("ScriptConstant"));
	Macros.Add(TEXT("ScriptConstantHost"));
	Macros.Add(TEXT("HidePin"));
	Macros.Add(TEXT("HideSpawnParms"));
	Macros.Add(TEXT("Keywords"));
	Macros.Add(TEXT("Latent"));
	Macros.Add(TEXT("LatentInfo"));
	Macros.Add(TEXT("MaterialParameterCollectionFunction"));
	Macros.Add(TEXT("NativeBreakFunc"));
	Macros.Add(TEXT("NativeMakeFunc"));
	Macros.Add(TEXT("UnsafeDuringActorConstruction"));
	Macros.Add(TEXT("WorldContext"));
	Macros.Add(TEXT("BlueprintAutocast"));
	Macros.Add(TEXT("NotBlueprintThreadSafe"));
	Macros.Add(TEXT("CannotImplementInterfaceInBlueprint"));
	Macros.Add(TEXT("TCHAR_TO_ANSI"));
	Macros.Add(TEXT("TCHAR_TO_OEM"));
	Macros.Add(TEXT("ANSI_TO_TCHAR"));
	Macros.Add(TEXT("TCHAR_TO_UTF8"));
	Macros.Add(TEXT("UTF8_TO_TCHAR"));
	Macros.Add(TEXT("LOG_MGC"));
	Macros.Add(TEXT("UE_LOG"));
	//
	Macros.Add(TEXT("FSelf"));
	Macros.Add(TEXT("Ensure"));
	Macros.Add(TEXT("MgcHint"));
	Macros.Add(TEXT("Enroll"));
	//
	Macros.Add(TEXT("STOP"));
	Macros.Add(TEXT("AWAKE"));
	Macros.Add(TEXT("START"));
	Macros.Add(TEXT("UPDATE"));
	//
	Macros.Add(TEXT("IMGC"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UMGC_ClassDB::UMGC_ClassDB() {
	ScriptCore.Add(TEXT("UClass"));
	ScriptCore.Add(TEXT("UObject"));
	ScriptCore.Add(TEXT("UStruct"));
	ScriptCore.Add(TEXT("UFunction"));
	ScriptCore.Add(TEXT("FProperty"));
	ScriptCore.Add(TEXT("UField"));
	//
	ScriptCore.Add(TEXT("Debug"));
	ScriptTypes.Add(TEXT("Super"));
	ScriptTypes.Add(TEXT("ThisClass"));
	ScriptTypes.Add(TEXT("WithinClass"));
	//
	ScriptTypes.Add(TEXT("bool"));
	ScriptTypes.Add(TEXT("float"));
	ScriptTypes.Add(TEXT("double"));
	ScriptTypes.Add(TEXT("int"));
	ScriptTypes.Add(TEXT("uint"));
	ScriptTypes.Add(TEXT("int8"));
	ScriptTypes.Add(TEXT("int16"));
	ScriptTypes.Add(TEXT("int32"));
	ScriptTypes.Add(TEXT("int64"));
	ScriptTypes.Add(TEXT("uint8"));
	ScriptTypes.Add(TEXT("uint16"));
	ScriptTypes.Add(TEXT("uint32"));
	ScriptTypes.Add(TEXT("uint64"));
	//
	ScriptTypes.Add(TEXT("FBool"));
	ScriptTypes.Add(TEXT("FFloat"));
	ScriptTypes.Add(TEXT("FDouble"));
	ScriptTypes.Add(TEXT("FByte"));
	ScriptTypes.Add(TEXT("FShort"));
	ScriptTypes.Add(TEXT("FInt"));
	ScriptTypes.Add(TEXT("FLong"));
	ScriptTypes.Add(TEXT("FUByte"));
	ScriptTypes.Add(TEXT("FUShort"));
	ScriptTypes.Add(TEXT("FUInt"));
	ScriptTypes.Add(TEXT("FULong"));
	//
	ScriptTypes.Add(TEXT("TCHAR"));
	ScriptTypes.Add(TEXT("FName"));
	ScriptTypes.Add(TEXT("FText"));
	ScriptTypes.Add(TEXT("FColor"));
	ScriptTypes.Add(TEXT("FString"));
	ScriptTypes.Add(TEXT("FVector"));
	ScriptTypes.Add(TEXT("FRotator"));
	ScriptTypes.Add(TEXT("FVector2D"));
	ScriptTypes.Add(TEXT("FTransform"));
	ScriptTypes.Add(TEXT("FLinearColor"));
	//
	ScriptTypes.Add(TEXT("TSet"));
	ScriptTypes.Add(TEXT("TMap"));
	ScriptTypes.Add(TEXT("TPair"));
	ScriptTypes.Add(TEXT("TArray"));
	ScriptTypes.Add(TEXT("TSharedPtr"));
	ScriptTypes.Add(TEXT("TSharedRef"));
	ScriptTypes.Add(TEXT("TSubclassOf"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UMGC_FunctionDB::UMGC_FunctionDB() {
	ScriptCore.Add(TEXT("Run"));
	ScriptCore.Add(TEXT("Stop"));
	ScriptCore.Add(TEXT("Awake"));
	ScriptCore.Add(TEXT("Start"));
	ScriptCore.Add(TEXT("Update"));
	ScriptCore.Add(TEXT("Finish"));
	ScriptCore.Add(TEXT("Execute"));
	//
	ScriptCore.Add(TEXT("Log"));
	ScriptCore.Add(TEXT("StaticClass"));
	ScriptCore.Add(TEXT("StaticClassFlags"));
	//
	ScriptCore.Add(TEXT("Cast"));
	ScriptCore.Add(TEXT("CastChecked"));
	//
	//
	for (TObjectIterator<UClass>IT; IT; ++IT) {
		if (IT->ClassDefaultObject==nullptr) {continue;}
		//
		for (TFieldIterator<UFunction>FIT(*IT,EFieldIteratorFlags::ExcludeSuper); FIT; ++FIT) {
			FString Target = FIT->GetName();
			ScriptCore.Add(Target);
		}///
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EDatabaseState UMGC_SemanticDB::DBState = EDatabaseState::READY;

UMGC_SemanticDB::UMGC_SemanticDB() {
	FKeywordDefinition KDefinition;
	FString Keyword;
	//
	Keyword = TEXT("alignas");
	KDefinition.Info = TEXT("Microsoft Specific.\nUse __declspec(align(#)) to precisely control the alignment of user-defined data (for example, static allocations or automatic data in a function).");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("alignof");
	KDefinition.Info = TEXT("C++11 introduces the alignof operator that returns the alignment, in bytes, of the specified type.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("and");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	&&");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("and_eq");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	&=");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("asm");
	KDefinition.Info = TEXT("Declaration of an inline assembly block.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("auto");
	KDefinition.Info = TEXT("For variables, specifies that the type of the variable that is being declared will be automatically deduced from its initializer.\nFor functions, specifies that the return type is a trailing return type or will be deduced from its return statements.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("bitand");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	&");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("bitor");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	|");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("break");
	KDefinition.Info = TEXT("Causes the enclosing for, range-for, while or do-while loop or switch statement to terminate.\nUsed when it is otherwise awkward to terminate the loop using the condition expression and conditional statements.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("case");
	KDefinition.Info = TEXT("The body of a switch statement may have an arbitrary number of case: labels, as long as the values of all expressions are unique:\n\nswitch(1)\n{\n\tcase 1 : Log('1');\n\t\t\tbreak;\n\tcase 2 : Log('2');\n}");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("catch");
	KDefinition.Info = TEXT("Associates one or more exception handlers (catch-clauses) with a compound statement.\n\ntry { /*...*/ } catch (const std::exception &e) { /*...*/ }.\nShouldn't be used in Unreal code as by default Unreal has exceptions disabled.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("char");
	KDefinition.Info = TEXT("Type for character representation which can be most efficiently processed on the target system.\nMultibyte characters strings use this type to represent text units.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("char16_t");
	KDefinition.Info = TEXT("Type for UTF-16 character representation, required to be large enough to represent any UTF-16 code unit (16 bits).");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("char32_t");
	KDefinition.Info = TEXT("Type for UTF-32 character representation, required to be large enough to represent any UTF-32 code unit (32 bits).");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("class");
	KDefinition.Info = TEXT("Declaration of a class.\nClasses and structs are user-defined types, defined by class-specifier, which appears in specifier of the declaration syntax.\nThe class specifier has the following syntax:\n\nclass-keyword attr class-head-name base-clause { member-specification }\n\nclass Vector;\t// forward declaration\nclass Matrix {/*...*/}");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("compl");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	~");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("concept");
	KDefinition.Info = TEXT("A concept is a named set of requirements. The definition of a concept must appear at namespace scope.\nThe definition of a concept has the form:\n\ntemplate < template-parameter-list >\nconcept concept-name = constraint-expression;");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("const");
	KDefinition.Info = TEXT("Appear in any type specifier, including decl-specifier-seq of declaration grammar, to specify constness:\n\nconst object - an object whose type is const-qualified, or a non-mutable subobject of a const object.\nSuch object cannot be modified, attempt to do so directly is a compile-time error, and attempt to do so indirectly results in undefined behavior.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("consteval");
	KDefinition.Info = TEXT("Specifies that a function is an immediate function, that is, every call to the function must produce a compile-time constant expression.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("constexpr");
	KDefinition.Info = TEXT("The constexpr specifier declares that it is possible to evaluate the value of the function or variable at compile time.\nIt may not be applied to destructors, allocation functions, or deallocation functions.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("const_cast");
	KDefinition.Info = TEXT("const_cast makes it possible to form a reference or pointer to non-const type that is actually referring to a const object\nor a reference or pointer to non-volatile type that is actually referring to a volatile object.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("continue");
	KDefinition.Info = TEXT("Causes the remaining portion of the enclosing for, range-for, while or do-while loop body to be skipped.\nUsed when it is otherwise awkward to ignore the remaining portion of the loop using conditional statements.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("decltype");
	KDefinition.Info = TEXT("Inspects the declared type of an entity or the type and value category of an expression.\n'decltype' is useful when declaring types that are difficult or impossible to declare using standard notation,\nlike lambda-related types or types that depend on template parameters.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("default");
	KDefinition.Info = TEXT("The body of a switch statement may have an arbitrary number of case: labels.\nAt most one default: label may be present. If condition evaluates to the value that doesn't match any of the case: labels, and the default: label is present,\ncontrol is transferred to the statement labeled with the default: label.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("delete");
	KDefinition.Info = TEXT("Destroys object(s) previously allocated by the new expression and releases obtained memory area.\nAvoid using this in Unreal UObjects since 'Garbage Collection' should take care of those types.\nUObjects have 'Destroy()' methods that should be used instead.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("do");
	KDefinition.Info = TEXT("do-while loop:\nExecutes a statement repeatedly, until the value of expression becomes false. The test takes place after each iteration:\n\ndo {\n\t\tj += 2;\n} while (j < 10);");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("dynamic_cast");
	KDefinition.Info = TEXT("Safely converts pointers and references to classes up, down, and sideways along the inheritance hierarchy tree.\nIn Unreal Engine consider making use of 'Cast<type>(...)' instead whenever possible.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("else");
	KDefinition.Info = TEXT("if statement - Conditionally executes another statement.\nUsed where code needs to be executed based on a run-time or compile-time condition.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("enum");
	KDefinition.Info = TEXT("Declaration of an enumeration type - An enumeration is a distinct type whose value is restricted to a range of values, which may include several explicitly named constants (enumerators).\nThe values of the constants are values of an integral type known as the underlying type of the enumeration.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("explicit");
	KDefinition.Info = TEXT("The explicit specifier may only appear within the decl-specifier-seq of the declaration of a constructor or conversion function within its class definition.\nSpecifies that a constructor or conversion function is explicit, that is, it cannot be used for implicit conversions and copy-initialization.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("export");
	KDefinition.Info = TEXT("Used to mark a template definition exported, which allows the same template to be declared, but not defined, in other translation units.\nMarks a declaration, a group of declarations, or another module as exported by the current module.\nIn Unreal you usually want to use the 'MODULENAME_API' macro instead.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("extern");
	KDefinition.Info = TEXT("Language linkage - Provides for linkage between modules written in different programming languages.\nSee more info at  https://en.cppreference.com/w/cpp/language/language_linkage");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("false");
	KDefinition.Info = TEXT("The Boolean literals are the keywords true and false. They are 'prvalues' of type bool.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("for");
	KDefinition.Info = TEXT("for loop - Executes a for loop over a range. Used as a more readable equivalent to the traditional for loop operating over a range of values, such as all elements in a container:\n\nfor (int32 I = 0; I < MyArray.Num(); I++) {\n\t\tif (I <= 9) {\n\t\t\tMyArray[I]->CallFunction();\n\t\t}\n}");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("friend");
	KDefinition.Info = TEXT("friend declaration - The friend declaration appears in a class body and grants a function or another unrelated class access\nto private and protected members of the class where the friend declaration appears.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("goto");
	KDefinition.Info = TEXT("Transfers control unconditionally.\nUsed when it is otherwise impossible to transfer control to the desired location using other statements (very rare occasions).");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("if");
	KDefinition.Info = TEXT("if statement - Conditionally executes another statement.\nUsed where code needs to be executed based on a run-time or compile-time condition.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("inline");
	KDefinition.Info = TEXT("The inline specifier, when used in a function's decl-specifier-seq, declares the function to be an inline function.\nA function defined entirely inside a class/struct/union definition, is implicitly an inline function.\nInstead of executing the function call CPU instruction to transfer control to the function body, a copy of the function body is executed without generating the call.\nThis avoids overhead created by the function call, but it may result in a larger executable as the code for the function has to be repeated multiple times.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("int");
	KDefinition.Info = TEXT("Native Type.\n\nBasic integer type. If no length modifiers are present, it's guaranteed to have a width of at least 16 bits.\nHowever, on 64 bit systems it is almost exclusively guaranteed to have width of at least 32 bits.\n\nMax value:		2147483647");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("long");
	KDefinition.Info = TEXT("long modifier - Target type will have width of at least 32 bits.\nlong long - Target type will have width of at least 64 bits.\nFor more info see 'Fundamental Types' at  https://en.cppreference.com/w/cpp/language/types");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("mutable");
	KDefinition.Info = TEXT("Applies to non-static class members of non-reference non-const type and specifies that the member does not affect the externally visible state of the class.\nMutable members of const class instances are modifiable.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("namespace");
	KDefinition.Info = TEXT("Namespaces provide a method for preventing name conflicts in large projects.\nSymbols declared inside a namespace block are placed in a named scope that prevents them from being mistaken\nfor identically-named symbols in other scopes.\nMultiple namespace blocks with the same name are allowed.\nAll declarations within those blocks are declared in the named scope.\nYou cannot create Unreal types inside namespaces, i.e: UCLASS().");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("new");
	KDefinition.Info = TEXT("Creates and initializes objects with dynamic storage duration, that is, objects whose lifetime is not limited by the scope in which they were created.\nDo not use 'new' to create UObject instances, use 'NewObject<type>(...)' functions for that.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("noexcept");
	KDefinition.Info = TEXT("Specifies whether a function could throw exceptions.\nIn general, try/catch exceptions in Unreal are disabled by default (don't use).");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("not");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	!");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("not_eq");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	!=");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("nullptr");
	KDefinition.Info = TEXT("invalid pointer (literal) - The keyword nullptr denotes the pointer literal. It is a prvalue of type std::nullptr_t.\nThere exist implicit conversions from nullptr to null pointer value of any pointer type and any pointer to member type.\nSimilar conversions exist for any null pointer constant, which includes values of type std::nullptr_t as well as the macro NULL.\nThink of it as \"comparing to zero\".");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("operator");
	KDefinition.Info = TEXT("operator overloading - Customizes the C++ operators for operands of user-defined types (advanced C++).\nAny of the following 38 operators can be overloaded:\n\n+ - * / % ^ & | ~ ! = < > += -= *= /= %= ^= &= |= << >> >>= <<= == != <= >= && || ++ -- , ->* -> ( ) [ ]");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("override");
	KDefinition.Info = TEXT("Specifies that a virtual function overrides another virtual function.\nIn a member function declaration or definition, 'override' ensures that the function is virtual and is overriding a virtual function from parent base class.\nThe program is ill-formed (a compile-time error is generated) if this is not true.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("or");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	||");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("or_eq");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	|=");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("private");
	KDefinition.Info = TEXT("In a member-specification of a class/struct or union, define the accessibility of subsequent members.\nIn a base-specifier of a derived class declaration, define the accessibility of inherited members of the subsequent base class.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("protected");
	KDefinition.Info = TEXT("In a member-specification of a class/struct or union, define the accessibility of subsequent members.\nIn a base-specifier of a derived class declaration, define the accessibility of inherited members of the subsequent base class.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("public");
	KDefinition.Info = TEXT("In a member-specification of a class/struct or union, define the accessibility of subsequent members.\nIn a base-specifier of a derived class declaration, define the accessibility of inherited members of the subsequent base class.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("register");
	KDefinition.Info = TEXT("The storage class specifiers are a part of the decl-specifier-seq of a name's declaration syntax. (C++17 deprecated)");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("reinterpret_cast");
	KDefinition.Info = TEXT("Converts between types by reinterpreting the underlying bit pattern.\nIt is purely a compile-time directive which instructs the compiler to treat expression as if it had the type new_type.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("requires");
	KDefinition.Info = TEXT("The keyword 'requires' is used to introduce a requires-clause, which specifies constraints on template arguments or on a function declaration.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("return");
	KDefinition.Info = TEXT("return statement - Terminates the current function and returns the specified value (if any) to its caller.\nAny for loops within current function will be interrupted and function exited.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("signed");
	KDefinition.Info = TEXT("Target type will have signed representation (this is the default if omitted). I.e: can represent negative numbers, below zero.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("short");
	KDefinition.Info = TEXT("Native Type.\n\nTarget type will be optimized for space and will have width of at least 16 bits.\n\nMax value:		32767");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("sizeof");
	KDefinition.Info = TEXT("Queries size of the object or type.\nUsed when actual size of the object must be known for next expression executed.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("static");
	KDefinition.Info = TEXT("Inside a class definition, the keyword static declares members that are not bound to any instances.\nThink of it as immutable 'global' declaration that can be accessed by any foreign class members, (if public)\nwithout pointing to any actual instance of the object being accessed.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("static_assert");
	KDefinition.Info = TEXT("Performs compile-time assertion checking.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("static_cast");
	KDefinition.Info = TEXT("Converts between types using a combination of implicit and user-defined conversions.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("struct");
	KDefinition.Info = TEXT("Classes and structs are user-defined types, defined by class-specifier, which appears in decl-specifier-seq of the declaration syntax.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("switch");
	KDefinition.Info = TEXT("The body of a switch statement may have an arbitrary number of case: labels, as long as the values of all expressions are unique:\n\nswitch(1)\n{\n\tcase 1 : Log('1');\n\t\t\tbreak;\n\tcase 2 : Log('2');\n}");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("template");
	KDefinition.Info = TEXT("A template is a C++ entity that defines one of the following:\n\nA family of classes (class template), which may be nested classes.\nA family of functions (function template), which may be member functions.\nAn alias to a family of types (alias template).\nA family of variables.\nA concept.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("this");
	KDefinition.Info = TEXT("this pointer - The keyword this is a 'prvalue' expression whose value is the memory address of the object itself, on which the member function is being called.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("throw");
	KDefinition.Info = TEXT("Signals an erroneous condition and executes an error handler.\nUnreal has a library to handle exceptions which should be used instead, check:\n\n https://docs.unrealengine.com/en-us/Programming/Assertions");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("true");
	KDefinition.Info = TEXT("The Boolean literals are the keywords true and false. They are 'prvalues' of type bool.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("try");
	KDefinition.Info = TEXT("try/catch expression - Associates one or more exception handlers (catch-clauses) with a compound statement.\n\ntry { /*...*/ } catch (const std::exception &e) { /*...*/ }.\nShouldn't be used in Unreal code as by default Unreal has exceptions disabled.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("typedef");
	KDefinition.Info = TEXT("Creates an alias that can be used anywhere in place of a (possibly complex) existing type name.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("typeid");
	KDefinition.Info = TEXT("Queries information of a type.\nUsed where the dynamic type of a polymorphic object must be known and for static type identification.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("typename");
	KDefinition.Info = TEXT("In a template declaration, typename can be used as an alternative to class to declare type template parameters and template parameters.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("union");
	KDefinition.Info = TEXT("A union is a special class type that can hold only one of its non-static data members at a time.\nThe class specifier for a union declaration is similar to class or struct declaration.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("using");
	KDefinition.Info = TEXT("Introduces a name that is defined elsewhere into the declarative region where this using-declaration appears.\nUsing-declarations can be used to introduce namespace members into other namespaces and block scopes,\nor to introduce base class members into derived class definitions.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("unsigned");
	KDefinition.Info = TEXT("Target type will have unsigned representation. I.e: can NOT represent negative numbers.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("virtual");
	KDefinition.Info = TEXT("The virtual specifier specifies that a non-static member function is virtual and supports dynamic dispatch.\nIt may only appear in the decl-specifier-seq of the initial declaration of a non-static member function.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("void");
	KDefinition.Info = TEXT("When used as a function return type, the void keyword specifies that the function does not return a value.\n When used for a function's parameter list, void specifies that the function takes no parameters.\n When used in the declaration of a pointer, void specifies that the pointer is \"universal\".");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("volatile");
	KDefinition.Info = TEXT("An object whose type is volatile-qualified, or a subobject of a volatile object, or a mutable subobject of a const-volatile object.");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("wchar_t");
	KDefinition.Info = TEXT("(TCHAR) Type for wide character representation.\nRequired to be large enough to represent any supported character code point (32 bits on systems that support Unicode).");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("while");
	KDefinition.Info = TEXT("while loop - Executes a statement repeatedly, until the value of condition becomes false. The test takes place before each iteration:\n\nwhile (--x >= 0)\n\t\ti++");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("xor");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	^");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	Keyword = TEXT("xor_eq");
	KDefinition.Info = TEXT("Alternative operator, an alternative for:	^=");
	KeywordDefinitions.Add(Keyword,KDefinition);
	//
	//
	FKeywordDefinition TDefinition;
	//
	Keyword = TEXT("UObject");
	TDefinition.Info = TEXT("Native Type.\n\nThe base class of all objects.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("bool");
	TDefinition.Info = TEXT("Blueprint Type.\n\nCapable of holding one of the two values: true or false.\nThe value of sizeof(bool) is implementation defined and might differ from 1.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("float");
	TDefinition.Info = TEXT("Blueprint Type.\n\nSingle precision floating point type. Usually IEEE-754 32 bit floating point type.\n\nMax value:		3.40282 E+38");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("double");
	TDefinition.Info = TEXT("Native Type.\n\nDouble precision floating point type. Usually IEEE-754 64 bit floating point type.\n\nMax value:		1.79769 E+308");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("uint8");
	TDefinition.Info = TEXT("Blueprint Type.\n\nTarget type will be optimized for space and will have width of at least 8 bits.\n\nMax value:		0  to  255");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("uint16");
	TDefinition.Info = TEXT("Native Type.\n\nSigned 'short' type.\nTarget type will be optimized for space and will have width of at least 16 bits.\n\nMax value:		0  to  65535");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("uint32");
	TDefinition.Info = TEXT("Native Type.\n\nSigned 'int' type.\n\nMax value:		0  to  4294967295");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("uint64");
	TDefinition.Info = TEXT("Native Type.\n\nSigned 'int' type.\n\nMax value:		0  to  4294967295");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("int8");
	TDefinition.Info = TEXT("Native Type.\n\nTarget type will be optimized for space and will have width of at least 8 bits.\n\nMax value:		-127  to  127");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("int16");
	TDefinition.Info = TEXT("Native Type.\n\nSigned 'short' type.\nTarget type will be optimized for space and will have width of at least 16 bits.\n\nMax value:		-32768  to  32767");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("int32");
	TDefinition.Info = TEXT("Blueprint Type.\n\nSigned 'int' type.\n\nMax value:		-2147483648  to  2147483647");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("int64");
	TDefinition.Info = TEXT("Native Type.\n\nSigned 'int' type.\n\nMax value:		-2147483648  to  2147483647");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	//
	Keyword = TEXT("FBool");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'bool'\n when compiled down to Unreal C++ Classes:\n\nCapable of holding one of the two values: true or false.\nThe value of sizeof(bool) is implementation defined and might differ from 1.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FFloat");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'float'\n when compiled down to Unreal C++ Classes:\n\nSingle precision floating point type. Usually IEEE-754 32 bit floating point type.\n\nMax value:		3.40282 E+38");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FDouble");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'double'\n when compiled down to Unreal C++ Classes:\n\nDouble precision floating point type. Usually IEEE-754 64 bit floating point type.\n\nMax value:		1.79769 E+308");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FUByte");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'uint8'\n when compiled down to Unreal C++ Classes:\n\nTarget type will be optimized for space and will have width of at least 8 bits.\n\nMax value:		0 to 255");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FUShort");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'uint16'\n when compiled down to Unreal C++ Classes:\n\nSigned 'short' type.\nTarget type will be optimized for space and will have width of at least 16 bits.\n\nMax value:		0 to 65535");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FUInt");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'uint32'\n when compiled down to Unreal C++ Classes:\n\nSigned 'int' type.\n\nMax value:		0  to  4294967295");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FULong");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'uint64'\n when compiled down to Unreal C++ Classes:\n\nSigned 'int' type.\n\nMax value:		0  to  4294967295");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FByte");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'int8'\n when compiled down to Unreal C++ Classes:\n\nTarget type will be optimized for space and will have width of at least 8 bits.\n\nMax value:		-127  to  127");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FShort");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'int16'\n when compiled down to Unreal C++ Classes:\n\nSigned 'short' type.\nTarget type will be optimized for space and will have width of at least 16 bits.\n\nMax value:		-32768  to  32767");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FInt");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'int32'\n when compiled down to Unreal C++ Classes:\n\nSigned 'int' type.\n\nMax value:		-2147483648  to  2147483647");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FLong");
	TDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically converts this type to 'int64'\n when compiled down to Unreal C++ Classes:\n\nSigned 'int' type.\n\nMax value:		-2147483648  to  2147483647");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TCHAR");
	TDefinition.Info = TEXT("Native Type.\n\ntypedef  wchar_t  TCHAR");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FName");
	TDefinition.Info = TEXT("Blueprint Type.\n\nNames are case-insensitive, but case-preserving strings.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FText");
	TDefinition.Info = TEXT("Blueprint Type.\n\nMulti-culture string (localized) type.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FString");
	TDefinition.Info = TEXT("Blueprint Type.\n\nUnlocalized array of chars, simple text type.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FColor");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA dynamically sizeable string of 'TCHAR's.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FLinearColor");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA linear, 32-bit/component floating point (float) RGBA color.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FVector");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA vector in 3-D space composed of components (X, Y, Z) with floating point precision.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FVector2D");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA vector in 2-D space composed of components (X, Y, Z) with floating point precision.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FRotator");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA container for rotation vectors.\nAll rotation values are stored in degrees.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("FTransform");
	TDefinition.Info = TEXT("Blueprint Type.\n\nTransform composed of Scale, Rotation (quaternion), and Translation.\nTransforms can be used to convert from one space to another, for example by transforming\npositions and directions from local space to world space.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TSet");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA set with an optional KeyFuncs parameters for customizing how the elements are compared and searched.\nE.g: You can specify a mapping from elements to keys if you want to find elements by specifying a subset of\nthe element type.  It uses a TSparseArray of the elements, and also links the elements into a hash with a\nnumber of buckets proportional to the number of elements.  Addition, removal, and finding are O(1).");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TMap");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA Blueprintable key/value pair - specialized type that only allows a single value associated with each unique key.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TArray");
	TDefinition.Info = TEXT("Blueprint Type.\n\nA dynamically sized array of typed elements. Makes the assumption that your elements are relocate-able;\nI.e: that they can be transparently moved to new memory without a copy constructor.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TSubclassOf");
	TDefinition.Info = TEXT("Blueprint Type.\n\nTemplate to allow UClass's to be passed around with type safety.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TPair");
	TDefinition.Info = TEXT("Native Type.\n\nA generic key/value pair type.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TSharedPtr");
	TDefinition.Info = TEXT("Native Type.\n\nTSharedPtr is a non-intrusive reference-counted authoritative object pointer. This shared pointer\nwill be conditionally thread-safe when the optional Mode template argument is set to ThreadSafe.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	Keyword = TEXT("TSharedRef");
	TDefinition.Info = TEXT("Native Type.\n\nTSharedRef is a non-nullable, non-intrusive reference-counted authoritative object reference.\nThis shared reference will be conditionally thread-safe when the optional Mode template argument is set to ThreadSafe.");
	TypeDefinitions.Add(Keyword,TDefinition);
	//
	//
	FKeywordDefinition MDefinition;
	//
	Keyword = TEXT("IMGC");
	MDefinition.Info = TEXT("MGC Macro.\n\nRegisters the 'Execute' function signature that allows this Node to be compiled and executed at runtime.");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("AWAKE");
	MDefinition.Info = TEXT("MGC Macro.\n\nEarliest call of a 'Latent Runtime Node's routines,\nif this Script has subscribed to be latent.\n(executes only once)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("START");
	MDefinition.Info = TEXT("MGC Macro.\n\nCalled from 'Latent Runtime Node's routines once subscription was complete,\nif this Script has subscribed to be latent.\n(executes only once)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("UPDATE");
	MDefinition.Info = TEXT("MGC Macro.\n\nCalled from 'Latent Runtime Node's routines,\nif this Script has subscribed to be latent.\n(executes every tick)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("STOP");
	MDefinition.Info = TEXT("MGC Macro.\n\nCalled from 'Latent Runtime Node's routines once this Script is set to be destroyed,\nif this Script has subscribed to be latent.\n(executes only once)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("FSelf");
	MDefinition.Info = TEXT("MGC Type.\nMagic Node Scripts automatically convert this type to {Script Class}\n when compiled down to Unreal C++ Classes:\n\nReference to 'this' class (in text form, not a pointer).\nThis type doesn't exist in Unreal C++ space.");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Ensure");
	MDefinition.Info = TEXT("MGC Macro.\n\nChecks if a pointer to an object is valid,\npreventing memory access violations (prevents runtime crashes).");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Enroll");
	MDefinition.Info = TEXT("MGC Macro.\n\nRegisters this Script as a 'Runtime Latent Node'.\nThis is a fundamental step to enable the use Magic Nodes with built-in persistent functions, such as\n\nAwake()\nStart()\nUpdate()\nStop()");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Run");
	MDefinition.Info = TEXT("MGC Function.\n\nStarts the 'Latent Runtime Node's routines if this Script was subscribed correctly.");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Awake");
	MDefinition.Info = TEXT("MGC Function.\n\nEarliest call of a 'Latent Runtime Node's routines,\nif this Script has subscribed to be latent.\n(executes only once)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Start");
	MDefinition.Info = TEXT("MGC Function.\n\nCalled from 'Latent Runtime Node's routines once subscription was complete,\nif this Script has subscribed to be latent.\n(executes only once)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Update");
	MDefinition.Info = TEXT("MGC Function.\n\nCalled from 'Latent Runtime Node's routines,\nif this Script has subscribed to be latent.\n(executes every tick)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Stop");
	MDefinition.Info = TEXT("MGC Function.\n\nCalled from 'Latent Runtime Node's routines once this Script is set to be destroyed,\nif this Script has subscribed to be latent.\n(executes only once)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	Keyword = TEXT("Finish");
	MDefinition.Info = TEXT("MGC Function.\n\nRequests this 'Latent Runtime Node' to be destroyed,\nignored if this Script has never subscribed to be latent.\n(executes only once)");
	MacroDefinitions.Add(Keyword,MDefinition);
	//
	//
	/// :: REGISTER DOCUMENTATION LINKS ::
	///
	FString DocsURL;
	//
	Keyword = TEXT("UObject");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/CoreUObject/UObject/UObject/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	//
	Keyword = TEXT("AActor");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AActor/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ACameraActor");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Camera/ACameraActor/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AController");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AController/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AHUD");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AHUD/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AInfo");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AInfo/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AGameModeBase");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AGameModeBase/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AGameMode");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AGameMode/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AGameNetworkManager");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AGameNetworkManager/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AGameSession");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AGameSession/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AGameStateBase");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AGameStateBase/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AGameState");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AGameState/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("APlayerState");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/APlayerState/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AWorldSettings");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AWorldSettings/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ALevelBounds");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ALevelBounds/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ALevelScriptActor");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ALevelScriptActor/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ALight");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ALight/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ADirectionalLight");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ADirectionalLight/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("APointLight");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/APointLight/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ARectLight");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ARectLight/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ASpotLight");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ASpotLight/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ALODActor");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ALODActor/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("APlayerStart");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/APlayerStart/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ANote");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ANote/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("APawn");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/APawn/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ACharacter");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/ACharacter/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ASpectatorPawn");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/ASpectatorPawn/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("APlayerCameraManager");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Camera/APlayerCameraManager/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ASkeletalMeshActor");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Animation/ASkeletalMeshActor/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ASplineMeshActor");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ASplineMeshActor/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("AStaticMeshActor");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/AStaticMeshActor/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ATriggerBox");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ATriggerBox/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ATriggerCapsule");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ATriggerCapsule/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ATriggerSphere");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Engine/ATriggerSphere/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UActorComponent");
	DocsURL = TEXT("https://api.unrealengine.com/INT/API/Runtime/Engine/Components/UActorComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UInputComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UInputComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UMovementComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/UMovementComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPawnMovementComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/UPawnMovementComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UCharacterMovementComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/UCharacterMovementComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UProjectileMovementComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/UProjectileMovementComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("URotatingMovementComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/URotatingMovementComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USceneComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USceneComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UAudioComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UAudioComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UCameraComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Camera/UCameraComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UChildActorComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UChildActorComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ULightComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/ULightComponentBase/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UDirectionalLightComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UDirectionalLightComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("ULocalLightComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/ULocalLightComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPointLightComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UPointLightComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USpotLightComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USpotLightComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("URectLightComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/URectLightComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USkyLightComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USkyLightComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPhysicsConstraintComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/PhysicsEngine/UPhysicsConstraintComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPhysicsSpringComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/PhysicsEngine/UPhysicsSpringComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPhysicsThrusterComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/PhysicsEngine/UPhysicsThrusterComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPostProcessComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UPostProcessComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPrimitiveComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UPrimitiveComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UArrowComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UArrowComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UBillboardComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UBillboardComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UBrushComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UBrushComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UDrawFrustumComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UDrawFrustumComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UMaterialBillboardComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UMaterialBillboardComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UMeshComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USkinnedMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USkinnedMeshComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPoseableMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UPoseableMeshComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USkeletalMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USkeletalMeshComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UStaticMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UStaticMeshComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UInstancedStaticMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UInstancedStaticMeshComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UHierarchicalInstancedStaticMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UHierarchicalInstancedStaticMesh-/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USplineMeshComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USplineMeshComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UModelComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UModelComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UParticleSystemComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Particles/UParticleSystemComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UShapeComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UShapeComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UBoxComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UBoxComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UCapsuleComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UCapsuleComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USphereComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USphereComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UDrawSphereComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UDrawSphereComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USplineComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USplineComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UTextRenderComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UTextRenderComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UVectorFieldComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UVectorFieldComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("URadialForceComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/PhysicsEngine/URadialForceComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UReflectionCaptureComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UReflectionCaptureComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UBoxReflectionCaptureComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UBoxReflectionCaptureComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPlaneReflectionCaptureComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UPlaneReflectionCaptureComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USphereReflectionCaptureComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USphereReflectionCaptureComponen-/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USceneCaptureComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USceneCaptureComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UPlanarReflectionComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UPlanarReflectionComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USceneCaptureComponent2D");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USceneCaptureComponent2D/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USceneCaptureComponentCube");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/USceneCaptureComponentCube/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("USpringArmComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/USpringArmComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UStereoLayerComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UStereoLayerComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UWindDirectionalSourceComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UWindDirectionalSourceComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	Keyword = TEXT("UTimelineComponent");
	DocsURL = TEXT("http://api.unrealengine.com/INT/API/Runtime/Engine/Components/UTimelineComponent/index.html");
	Documentation.Add(Keyword,DocsURL);
	//
	//
	///// :: REGISTER CORE CLASSES ::
	///
	RegisterClassReflection(UObject::StaticClass(),TEXT("U"));
	//
	RegisterClassReflection(AActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(AAmbientSound::StaticClass(),TEXT("A"));
	RegisterClassReflection(ABrush::StaticClass(),TEXT("A"));
	RegisterClassReflection(ABrushShape::StaticClass(),TEXT("A"));
	RegisterClassReflection(AVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(AAudioVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ABlockingVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ACameraBlockingVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALevelStreamingVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALightmassCharacterIndirectDetailVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALightmassImportanceVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(AMeshMergeCullingVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(APhysicsVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ADefaultPhysicsVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(AKillZVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(APainCausingVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(APostProcessVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(APrecomputedVisibilityOverrideVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(APrecomputedVisibilityVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ATriggerVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(AVolumetricLightmapDensityVolume::StaticClass(),TEXT("A"));
	RegisterClassReflection(ACameraActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(AController::StaticClass(),TEXT("A"));
	RegisterClassReflection(ADebugCameraController::StaticClass(),TEXT("A"));
	RegisterClassReflection(ADecalActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(ADocumentationActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(AEmitter::StaticClass(),TEXT("A"));
	RegisterClassReflection(AHUD::StaticClass(),TEXT("A"));
	RegisterClassReflection(ADebugCameraHUD::StaticClass(),TEXT("A"));
	RegisterClassReflection(AInfo::StaticClass(),TEXT("A"));
	RegisterClassReflection(AExponentialHeightFog::StaticClass(),TEXT("A"));
	RegisterClassReflection(AGameModeBase::StaticClass(),TEXT("A"));
	RegisterClassReflection(AEmitterCameraLensEffectBase::StaticClass(),TEXT("A"));
	RegisterClassReflection(AGameMode::StaticClass(),TEXT("A"));
	RegisterClassReflection(AGameNetworkManager::StaticClass(),TEXT("A"));
	RegisterClassReflection(AGameSession::StaticClass(),TEXT("A"));
	RegisterClassReflection(AGameStateBase::StaticClass(),TEXT("A"));
	RegisterClassReflection(AGameState::StaticClass(),TEXT("A"));
	RegisterClassReflection(APlayerController::StaticClass(),TEXT("A"));
	RegisterClassReflection(APlayerState::StaticClass(),TEXT("A"));
	RegisterClassReflection(AServerStatReplicator::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASkyLight::StaticClass(),TEXT("A"));
	RegisterClassReflection(AWindDirectionalSource::StaticClass(),TEXT("A"));
	RegisterClassReflection(AWorldSettings::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALevelBounds::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALevelScriptActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALight::StaticClass(),TEXT("A"));
	RegisterClassReflection(ADirectionalLight::StaticClass(),TEXT("A"));
	RegisterClassReflection(APointLight::StaticClass(),TEXT("A"));
	RegisterClassReflection(ARectLight::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASpotLight::StaticClass(),TEXT("A"));
	RegisterClassReflection(AGeneratedMeshAreaLight::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALightmassPortal::StaticClass(),TEXT("A"));
	RegisterClassReflection(ALODActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(AMaterialInstanceActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(AMatineeActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(AMatineeActorCameraAnim::StaticClass(),TEXT("A"));
	RegisterClassReflection(ANavigationObjectBase::StaticClass(),TEXT("A"));
	RegisterClassReflection(APlayerStart::StaticClass(),TEXT("A"));
	RegisterClassReflection(APlayerStartPIE::StaticClass(),TEXT("A"));
	RegisterClassReflection(ANote::StaticClass(),TEXT("A"));
	RegisterClassReflection(APawn::StaticClass(),TEXT("A"));
	RegisterClassReflection(ACharacter::StaticClass(),TEXT("A"));
	RegisterClassReflection(ADefaultPawn::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASpectatorPawn::StaticClass(),TEXT("A"));
	RegisterClassReflection(APlayerCameraManager::StaticClass(),TEXT("A"));
	RegisterClassReflection(AReflectionCapture::StaticClass(),TEXT("A"));
	RegisterClassReflection(ABoxReflectionCapture::StaticClass(),TEXT("A"));
	RegisterClassReflection(APlaneReflectionCapture::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASphereReflectionCapture::StaticClass(),TEXT("A"));
	RegisterClassReflection(ARigidBodyBase::StaticClass(),TEXT("A"));
	RegisterClassReflection(APhysicsConstraintActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(ARadialForceActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASceneCapture::StaticClass(),TEXT("A"));
	RegisterClassReflection(APlanarReflection::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASceneCapture2D::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASceneCaptureCube::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASkeletalMeshActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(ASplineMeshActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(AStaticMeshActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(ATargetPoint::StaticClass(),TEXT("A"));
	RegisterClassReflection(ATextRenderActor::StaticClass(),TEXT("A"));
	RegisterClassReflection(ATriggerBase::StaticClass(),TEXT("A"));
	RegisterClassReflection(ATriggerBox::StaticClass(),TEXT("A"));
	RegisterClassReflection(ATriggerCapsule::StaticClass(),TEXT("A"));
	RegisterClassReflection(ATriggerSphere::StaticClass(),TEXT("A"));
	RegisterClassReflection(AVectorFieldVolume::StaticClass(),TEXT("A"));
	//
	RegisterClassReflection(UActorComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UApplicationLifecycleComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UInputComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UMovementComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UInterpToMovementComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UNavMovementComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPawnMovementComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UCharacterMovementComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UFloatingPawnMovement::StaticClass(),TEXT("U"));
	RegisterClassReflection(USpectatorPawnMovement::StaticClass(),TEXT("U"));
	RegisterClassReflection(UProjectileMovementComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(URotatingMovementComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPawnNoiseEmitterComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPhysicalAnimationComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPlatformEventsComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USceneComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UAtmosphericFogComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UAudioComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UCameraComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UChildActorComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UDecalComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UExponentialHeightFogComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UForceFeedbackComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(ULightComponentBase::StaticClass(),TEXT("U"));
	RegisterClassReflection(ULightComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UDirectionalLightComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(ULocalLightComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPointLightComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USpotLightComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(URectLightComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USkyLightComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(ULightmassPortalComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPhysicsConstraintComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPhysicsSpringComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPhysicsThrusterComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPostProcessComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPrimitiveComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UArrowComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UBillboardComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UBrushComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UDrawFrustumComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(ULineBatchComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UMaterialBillboardComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USkinnedMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPoseableMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USkeletalMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UStaticMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UInstancedStaticMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UHierarchicalInstancedStaticMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USplineMeshComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UModelComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UParticleSystemComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UShapeComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UBoxComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UCapsuleComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USphereComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UDrawSphereComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USplineComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UTextRenderComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UVectorFieldComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(URadialForceComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UReflectionCaptureComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UBoxReflectionCaptureComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPlaneReflectionCaptureComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USphereReflectionCaptureComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USceneCaptureComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UPlanarReflectionComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(USceneCaptureComponent2D::StaticClass(),TEXT("U"));
	RegisterClassReflection(USceneCaptureComponentCube::StaticClass(),TEXT("U"));
	RegisterClassReflection(USpringArmComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UStereoLayerComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UWindDirectionalSourceComponent::StaticClass(),TEXT("U"));
	RegisterClassReflection(UTimelineComponent::StaticClass(),TEXT("U"));
	//
	//
	const auto &Settings = GetDefault<UKMGC_Settings>();
	if (!Settings->ScanUnrealTypesOnEditorStartup) {return;}
	//
	(new FAutoDeleteAsyncTask<TASK_BuildAutoCompleteData>(this))->StartSynchronousTask();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UMGC_SemanticDB::UpdateExtensions() {
	if (UMGC_SemanticDB::DBState==EDatabaseState::ASYNCLOADING) {return;}
	UMGC_SemanticDB::DBState = EDatabaseState::ASYNCLOADING;
	//
	for (TObjectIterator<UClass>IT; IT; ++IT) {
		if (!IsValidTarget(*IT)) {continue;}
		//
		RegisterClassReflection((*IT),TEXT("U"));
		//
		FClassRedirector CR;
		CR.ObjectName = IT->GetName();
		CR.ObjectClass = FString(TEXT("U"))+((*IT)->GetName());
		//
		ClassRedirectors.Add((*IT)->GetName(),CR);
	}///
	//
	for (TObjectIterator<UStruct>IT; IT; ++IT) {
		if (IT->IsA(UFunction::StaticClass())) {continue;}
		if (IT->IsA(UClass::StaticClass())) {continue;}
		if (IT->GetName().Len()>56) {continue;}
		//
		RegisterStructReflection((*IT));
		//
		FClassRedirector CR;
		CR.ObjectName = IT->GetName();
		CR.ObjectClass = FString(TEXT("F"))+((*IT)->GetName());
		//
		ClassRedirectors.Add((*IT)->GetName(),CR);
	}///
	//
	for (TObjectIterator<UEnum>IT; IT; ++IT) {
		RegisterEnumReflection((*IT));
		//
		FClassRedirector CR;
		CR.ObjectName = IT->GetName();
		CR.ObjectClass = (*IT)->GetName();
		//
		ClassRedirectors.Add((*IT)->GetName(),CR);
	}///
	//
	for (TObjectIterator<AActor>IT; IT; ++IT) {
		RegisterClassReflection(IT->GetClass(),TEXT("A"));
		//
		FClassRedirector CR;
		CR.ObjectName = IT->GetName();
		CR.ObjectClass = FString(TEXT("A"))+(IT->GetClass()->GetName());
		//
		ClassRedirectors.Add(IT->GetName(),CR);
	}///
	//
	for (TObjectIterator<UActorComponent>IT; IT; ++IT) {
		RegisterClassReflection(IT->GetClass(),TEXT("U"));
		//
		FClassRedirector CR;
		CR.ObjectName = IT->GetName();
		CR.ObjectClass = FString(TEXT("U"))+(IT->GetClass()->GetName());
		//
		ClassRedirectors.Add(IT->GetName(),CR);
	}///
	//
	for (TObjectIterator<UObject>IT; IT; ++IT) {
		if (!IT->HasAllFlags(RF_ClassDefaultObject|RF_ArchetypeObject|RF_DefaultSubObject)) {continue;}
		//
		if (IT->IsA(UEnum::StaticClass())) {continue;}
		if (IT->IsA(UClass::StaticClass())) {continue;}
		if (IT->IsA(AActor::StaticClass())) {continue;}
		if (IT->IsA(UStruct::StaticClass())) {continue;}
		if (IT->IsA(UMagicNode::StaticClass())) {continue;}
		if (IT->IsA(UActorComponent::StaticClass())) {continue;}
		if (IT->IsA(UBlueprintNodeSpawner::StaticClass())) {continue;}
		//
		RegisterClassReflection(IT->GetClass(),TEXT("U"));
		//
		FClassRedirector CR;
		CR.ObjectName = IT->GetName();
		CR.ObjectClass = FString(TEXT("U"))+(IT->GetClass()->GetName());
		//
		ClassRedirectors.Add(IT->GetName(),CR);
	}///
	//
	for (TObjectIterator<UMagicNode>IT; IT; ++IT) {
		RegisterClassReflection(IT->GetClass(),TEXT("U"));
		//
		FClassRedirector CR;
		CR.ObjectName = IT->GetName();
		CR.ObjectClass = FString(TEXT("U"))+(IT->GetClass()->GetName());
		//
		ClassRedirectors.Add(IT->GetName(),CR);
	}///
	//
	//
	{
		FClassRedirector CR_Scale;
		CR_Scale.ObjectName = TEXT("Scale");
		CR_Scale.ObjectClass = TEXT("FVector");
		CR_Scale.OwnerClass = TEXT("FTransform");
		ClassRedirectors.Add(CR_Scale.ObjectName,CR_Scale);
		//
		FClassRedirector CR_Location;
		CR_Location.ObjectName = TEXT("Location");
		CR_Location.ObjectClass = TEXT("FVector");
		CR_Location.OwnerClass = TEXT("FTransform");
		ClassRedirectors.Add(CR_Location.ObjectName,CR_Location);
		//
		FClassRedirector CR_Rotation;
		CR_Rotation.ObjectName = TEXT("Rotation");
		CR_Rotation.ObjectClass = TEXT("FRotator");
		CR_Rotation.OwnerClass = TEXT("FTransform");
		ClassRedirectors.Add(CR_Rotation.ObjectName,CR_Rotation);
	}
	//
	//
	UMGC_SemanticDB::DBState = EDatabaseState::READY;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UMGC_SemanticDB::RegisterClassReflection(UClass* Class, FString Prefix) {
	FString ClassName = Prefix + Class->GetName();
	//
	if (Class->HasAnyClassFlags(CLASS_Hidden)) {return;}
	if (Class->HasAnyClassFlags(CLASS_Deprecated)) {return;}
	if (Class->HasAnyClassFlags(CLASS_NewerVersionExists)) {return;}
	//
	const FString CHint = Class->GetMetaData(TEXT("MgcHint"));
	const FString CMeta = Class->GetMetaData(TEXT("ToolTip"));
	const FString CParent = (Class->GetSuperStruct()) ? Class->GetSuperStruct()->GetName() : TEXT("");
	//
	FClassDefinition ClassDefinition;
	ClassDefinition.ParentClass = CParent;
	ClassDefinition.Tooltip = CMeta;
	ClassDefinition.Hint = CHint;
	//
	for (TFieldIterator<FProperty>PIT(Class,EFieldIteratorFlags::IncludeSuper); PIT; ++PIT) {
		FString PropName = PIT->GetName();
		TFieldPath<FProperty>Property = *PIT;
		//
		const FString PHint = PIT->GetMetaData(TEXT("MgcHint"));
		const FString PMeta = PIT->GetMetaData(TEXT("ToolTip"));
		//
		FPropertyDefinition PropDefinition;
		PropDefinition.ReturnType = PropertyToTypeName(Property)+FString(TEXT(" "))+PropName;
		PropDefinition.Access = EAccessLevel::Public;
		PropDefinition.StackOf = EStack::None;
		PropDefinition.TypeOf = EType::None;
		PropDefinition.Tooltip = PMeta;
		PropDefinition.Hint = PHint;
		//
		if (PIT->HasAnyPropertyFlags(CPF_Protected)) {PropDefinition.Access=EAccessLevel::Protected;}
		//
		if (Property->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
		if (Property->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
		if (Property->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
		if (Property->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
		//
		if (Property->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
		if (Property->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
		if (Property->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
		if (Property->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
		if (Property->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
		if (Property->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
		if (Property->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;} else
		//
		if (Property->IsA(FArrayProperty::StaticClass())) {
			TFieldPath<FArrayProperty>PArray = CastFieldChecked<FArrayProperty>(Property.Get());
			PropDefinition.StackOf = EStack::Array;
			//
			if (PArray->Inner && PArray->Inner->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
			if (PArray->Inner && PArray->Inner->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
			if (PArray->Inner && PArray->Inner->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
			if (PArray->Inner && PArray->Inner->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
			//
			if (PArray->Inner && PArray->Inner->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
			if (PArray->Inner && PArray->Inner->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
			if (PArray->Inner && PArray->Inner->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
			if (PArray->Inner && PArray->Inner->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
			if (PArray->Inner && PArray->Inner->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
			if (PArray->Inner && PArray->Inner->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
			if (PArray->Inner && PArray->Inner->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;}
		} else if (Property->IsA(FSetProperty::StaticClass())) {
			TFieldPath<FSetProperty>PSet = CastFieldChecked<FSetProperty>(Property.Get());
			PropDefinition.StackOf = EStack::Set;
			//
			if (PSet->ElementProp && PSet->ElementProp->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
			//
			if (PSet->ElementProp && PSet->ElementProp->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;}
		} else if (Property->IsA(FMapProperty::StaticClass())) {
			TFieldPath<FMapProperty>PMap = CastFieldChecked<FMapProperty>(Property.Get());
			PropDefinition.StackOf = EStack::Map;
			//
			if (PMap->KeyProp && PMap->KeyProp->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
			//
			if (PMap->KeyProp && PMap->KeyProp->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;}
		}///
		//
		ClassDefinition.Variables.Add(PropName,PropDefinition);
	}///
	//
	for (TFieldIterator<UFunction>FIT(Class,EFieldIteratorFlags::IncludeSuper); FIT; ++FIT) {
		UFunction* Function = *FIT;
		//
		FString FunName = FIT->GetName();
		const FString FHint = FIT->GetMetaData(TEXT("MgcHint"));
		const FString FMeta = FIT->GetMetaData(TEXT("ToolTip"));
		//
		FFunctionDefinition FunDefinition;
		FunDefinition.Tooltip = FMeta;
		FunDefinition.Hint = FHint;
		//
		if (FIT->HasAnyFunctionFlags(FUNC_BlueprintCallable)) {FunDefinition.Flag=EFunctionFlag::Callable;}
		if (FIT->HasAnyFunctionFlags(FUNC_BlueprintEvent)) {FunDefinition.Flag=EFunctionFlag::Event;}
		if (FIT->HasAnyFunctionFlags(FUNC_BlueprintPure)) {FunDefinition.Flag=EFunctionFlag::Pure;}
		if (FIT->HasAnyFunctionFlags(FUNC_Delegate)) {FunDefinition.Flag=EFunctionFlag::Delegate;}
		if (FIT->HasAnyFunctionFlags(FUNC_Const)) {FunDefinition.Flag=EFunctionFlag::Const;}
		if (FIT->HasAnyFunctionFlags(FUNC_Event)) {FunDefinition.Flag=EFunctionFlag::Event;}
		//
		FunDefinition.Access = EAccessLevel::Static;
		if (FIT->HasAnyFunctionFlags(FUNC_Public)) {FunDefinition.Access=EAccessLevel::Public;}
		if (FIT->HasAnyFunctionFlags(FUNC_Private)) {FunDefinition.Access=EAccessLevel::Private;}
		if (FIT->HasAnyFunctionFlags(FUNC_Protected)) {FunDefinition.Access=EAccessLevel::Protected;}
		//
		FunDefinition.ReturnType = TEXT("void");
		for (TFieldIterator<FProperty>PIT(Function); PIT&&(PIT->PropertyFlags&CPF_Parm); ++PIT) {
			TFieldPath<FProperty>Property = *PIT;
			//
			FString Display = (PropertyToTypeName(Property)+FString(TEXT(" "))+Property->GetName());
			if (Property->HasAnyPropertyFlags(CPF_ReturnParm)) {
				FunDefinition.ReturnType = Display;
			} else if (Property->HasAnyPropertyFlags(CPF_OutParm)) {
				FunDefinition.Outputs.Add(Display);
			} else {FunDefinition.Inputs.Add(Display);}
		}///
		//
		ClassDefinition.Functions.Add(FunName,FunDefinition);
	}///
	//
	ClassDefinitions.Add(ClassName,ClassDefinition);
	//
	//
	const auto &Settings = GetDefault<UKMGC_Settings>();
	if (Settings->ClassDB.Num()>=1) {
		UMGC_ClassDB* ClassDB = Settings->ClassDB.Array()[0].Get();
		if (ClassDB) {ClassDB->Extensions.Add(ClassName);}
	}///
}

void UMGC_SemanticDB::RegisterEnumReflection(UEnum* Enum) {
	const FString EnumName = Enum->GetName();
	const FString CHint = Enum->GetMetaData(TEXT("MgcHint"));
	const FString CMeta = Enum->GetMetaData(TEXT("ToolTip"));
	const FString CParent = TEXT("UEnum");
	//
	FClassDefinition EnumDefinition;
	EnumDefinition.ParentClass = CParent;
	EnumDefinition.Tooltip = CMeta;
	EnumDefinition.Hint = CHint;
	//
	for (int64 I=0; I<(Enum->GetMaxEnumValue()); I++) {
		FPropertyDefinition PropDefinition;
		//
		const FString &Item = Enum->GetNameStringByIndex(I);
		PropDefinition.TypeOf = EType::Enum;
		PropDefinition.ReturnType = Item;
		//
		EnumDefinition.Variables.Add(Item,PropDefinition);
	}///
	//
	ClassDefinitions.Add(EnumName,EnumDefinition);
	//
	//
	const auto &Settings = GetDefault<UKMGC_Settings>();
	if (Settings->ClassDB.Num()>=1) {
		UMGC_ClassDB* ClassDB = Settings->ClassDB.Array()[0].Get();
		if (ClassDB) {ClassDB->Extensions.Add(EnumName);}
	}///
}

void UMGC_SemanticDB::RegisterStructReflection(UStruct* Struct) {
	if (Struct->IsA(UFunction::StaticClass())) {return;}
	if (Struct->IsA(UClass::StaticClass())) {return;}
	//
	FString StructName = FString(TEXT("F"))+Struct->GetName();
	//
	const FString CHint = Struct->GetMetaData(TEXT("MgcHint"));
	const FString CMeta = Struct->GetMetaData(TEXT("ToolTip"));
	const FString CParent = (Struct->GetSuperStruct()) ? Struct->GetSuperStruct()->GetName() : TEXT("");
	//
	FClassDefinition StructDefinition;
	StructDefinition.ParentClass = CParent;
	StructDefinition.Tooltip = CMeta;
	StructDefinition.Hint = CHint;
	//
	//
	for (TFieldIterator<FProperty>PIT(Struct,EFieldIteratorFlags::IncludeSuper); PIT; ++PIT) {
		FString PropName = PIT->GetName();
		TFieldPath<FProperty>Property = *PIT;
		//
		const FString PHint = PIT->GetMetaData(TEXT("MgcHint"));
		const FString PMeta = PIT->GetMetaData(TEXT("ToolTip"));
		//
		FPropertyDefinition PropDefinition;
		PropDefinition.ReturnType = PropertyToTypeName(Property)+FString(TEXT(" "))+PropName;
		PropDefinition.Access = EAccessLevel::Public;
		PropDefinition.StackOf = EStack::None;
		PropDefinition.TypeOf = EType::None;
		PropDefinition.Tooltip = PMeta;
		PropDefinition.Hint = PHint;
		//
		if (PIT->HasAnyPropertyFlags(CPF_Protected)) {PropDefinition.Access=EAccessLevel::Protected;}
		//
		if (Property->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
		if (Property->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
		if (Property->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
		if (Property->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
		//
		if (Property->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
		if (Property->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
		if (Property->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
		if (Property->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
		if (Property->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
		if (Property->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
		if (Property->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;} else
		//
		if (Property->IsA(FArrayProperty::StaticClass())) {
			TFieldPath<FArrayProperty>PArray = CastFieldChecked<FArrayProperty>(Property.Get());
			PropDefinition.StackOf = EStack::Array;
			//
			if (PArray->Inner && PArray->Inner->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
			if (PArray->Inner && PArray->Inner->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
			if (PArray->Inner && PArray->Inner->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
			if (PArray->Inner && PArray->Inner->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
			//
			if (PArray->Inner && PArray->Inner->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
			if (PArray->Inner && PArray->Inner->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
			if (PArray->Inner && PArray->Inner->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
			if (PArray->Inner && PArray->Inner->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
			if (PArray->Inner && PArray->Inner->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
			if (PArray->Inner && PArray->Inner->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
			if (PArray->Inner && PArray->Inner->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;}
		} else if (Property->IsA(FSetProperty::StaticClass())) {
			TFieldPath<FSetProperty>PSet = CastFieldChecked<FSetProperty>(Property.Get());
			PropDefinition.StackOf = EStack::Set;
			//
			if (PSet->ElementProp && PSet->ElementProp->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
			//
			if (PSet->ElementProp && PSet->ElementProp->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
			if (PSet->ElementProp && PSet->ElementProp->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;}
		} else if (Property->IsA(FMapProperty::StaticClass())) {
			TFieldPath<FMapProperty>PMap = CastFieldChecked<FMapProperty>(Property.Get());
			PropDefinition.StackOf = EStack::Map;
			//
			if (PMap->KeyProp && PMap->KeyProp->IsA(FEnumProperty::StaticClass())) {PropDefinition.TypeOf=EType::Enum;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FClassProperty::StaticClass())) {PropDefinition.TypeOf=EType::Class;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FStructProperty::StaticClass())) {PropDefinition.TypeOf=EType::Struct;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FObjectProperty::StaticClass())) {PropDefinition.TypeOf=EType::Object;} else
			//
			if (PMap->KeyProp && PMap->KeyProp->IsA(FIntProperty::StaticClass())) {PropDefinition.TypeOf=EType::Int;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FBoolProperty::StaticClass())) {PropDefinition.TypeOf=EType::Bool;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FByteProperty::StaticClass())) {PropDefinition.TypeOf=EType::Byte;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FNameProperty::StaticClass())) {PropDefinition.TypeOf=EType::Name;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FTextProperty::StaticClass())) {PropDefinition.TypeOf=EType::Text;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FStrProperty::StaticClass())) {PropDefinition.TypeOf=EType::String;} else
			if (PMap->KeyProp && PMap->KeyProp->IsA(FFloatProperty::StaticClass())) {PropDefinition.TypeOf=EType::Float;}
		}///
		//
		StructDefinition.Variables.Add(PropName,PropDefinition);
	}///
	//
	ClassDefinitions.Add(StructName,StructDefinition);
	//
	//
	const auto &Settings = GetDefault<UKMGC_Settings>();
	if (Settings->ClassDB.Num()>=1) {
		UMGC_ClassDB* ClassDB = Settings->ClassDB.Array()[0].Get();
		if (ClassDB) {ClassDB->Extensions.Add(StructName);}
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString UMGC_SemanticDB::PropertyToTypeName(TFieldPath<FProperty>Property) {
	FString PType = TEXT("<?>");
	//
	if (Property==nullptr) {return PType;}
	//
	if (Property->IsA(FBoolProperty::StaticClass())) {PType=TEXT("bool"); return PType;}
	if (Property->IsA(FIntProperty::StaticClass())) {PType=TEXT("int32"); return PType;}
	if (Property->IsA(FByteProperty::StaticClass())) {PType=TEXT("uint8"); return PType;}
	if (Property->IsA(FFloatProperty::StaticClass())) {PType=TEXT("float"); return PType;}
	//
	if (Property->IsA(FNameProperty::StaticClass())) {PType=TEXT("FName"); return PType;}
	if (Property->IsA(FTextProperty::StaticClass())) {PType=TEXT("FText"); return PType;}
	if (Property->IsA(FStrProperty::StaticClass())) {PType=TEXT("FString"); return PType;}
	//
	if (Property->IsA(FEnumProperty::StaticClass())) {
		PType = TEXT("enum");
		//
		UEnum* Enum = CastFieldChecked<FEnumProperty>(Property.Get())->GetEnum();
		PType = FString(TEXT("E"))+Enum->GetName();
	return PType;}
	//
	if (Property->IsA(FClassProperty::StaticClass())) {
		PType = TEXT("class");
		//
		TFieldPath<FClassProperty>PCLS = CastFieldChecked<FClassProperty>(Property.Get());
		auto CLS = PCLS->GetDefaultPropertyValue();
		//
		if (CLS!=nullptr) {PType=FString(TEXT("U"))+CLS->GetName();}
	return PType;}
	//
	if (Property->IsA(FStructProperty::StaticClass())) {
		TFieldPath<FStructProperty>PStruct = CastFieldChecked<FStructProperty>(Property.Get());
		PType = TEXT("struct");
		//
		if (PStruct->Struct==TBaseStructure<FColor>::Get()) {PType=TEXT("FColor"); return PType;}
		if (PStruct->Struct==TBaseStructure<FVector>::Get()) {PType=TEXT("FVector"); return PType;}
		if (PStruct->Struct==TBaseStructure<FRotator>::Get()) {PType=TEXT("FRotator"); return PType;}
		if (PStruct->Struct==TBaseStructure<FVector2D>::Get()) {PType=TEXT("FVector2D"); return PType;}
		if (PStruct->Struct==TBaseStructure<FTransform>::Get()) {PType=TEXT("FTransform"); return PType;}
		if (PStruct->Struct==TBaseStructure<FLinearColor>::Get()) {PType=TEXT("FLinearColor"); return PType;}
		//
		PType = FString(TEXT("F"))+PStruct->Struct->GetName();
	return PType;}
	//
	if (Property->IsA(FObjectProperty::StaticClass())) {
		PType = TEXT("object");
		//
		TFieldPath<FObjectProperty>POBJ = CastFieldChecked<FObjectProperty>(Property.Get());
		auto OBJ = POBJ->GetDefaultPropertyValue();
		//
		if (OBJ!=nullptr) {PType=FString(TEXT("U"))+OBJ->GetName();}
	return PType;}
	//
	if (Property->IsA(FSetProperty::StaticClass())) {
		TFieldPath<FSetProperty>PSet = CastFieldChecked<FSetProperty>(Property.Get());
		//
		if (PSet->ElementProp && PSet->ElementProp->IsA(FIntProperty::StaticClass())) {PType=TEXT("TSet< int >"); return PType;}
		if (PSet->ElementProp && PSet->ElementProp->IsA(FBoolProperty::StaticClass())) {PType=TEXT("TSet< bool >"); return PType;}
		if (PSet->ElementProp && PSet->ElementProp->IsA(FByteProperty::StaticClass())) {PType=TEXT("TSet< uint8 >"); return PType;}
		if (PSet->ElementProp && PSet->ElementProp->IsA(FFloatProperty::StaticClass())) {PType=TEXT("TSet< float >"); return PType;}
		//
		if (PSet->ElementProp && PSet->ElementProp->IsA(FNameProperty::StaticClass())) {PType=TEXT("TSet< FName >"); return PType;}
		if (PSet->ElementProp && PSet->ElementProp->IsA(FTextProperty::StaticClass())) {PType=TEXT("TSet< FText >"); return PType;}
		if (PSet->ElementProp && PSet->ElementProp->IsA(FStrProperty::StaticClass())) {PType=TEXT("TSet< FString >"); return PType;}
		//
		if (PSet->ElementProp && PSet->ElementProp->IsA(FEnumProperty::StaticClass())) {
			PType = TEXT("TSet< enum >");
			//
			UEnum* Enum = CastFieldChecked<FEnumProperty>(PSet->ElementProp)->GetEnum();
			PType = FString(TEXT("E"))+Enum->GetName();
		return PType;}
		//
		if (PSet->ElementProp && PSet->ElementProp->IsA(FClassProperty::StaticClass())) {
			PType = TEXT("TSet< class >");
			//
			TFieldPath<FClassProperty>PCLS = CastFieldChecked<FClassProperty>(PSet->ElementProp);
			auto CLS = PCLS->GetDefaultPropertyValue();
			//
			if (CLS!=nullptr) {PType=FString(TEXT("U"))+CLS->GetName();}
		return PType;}
		//
		if (PSet->ElementProp && PSet->ElementProp->IsA(FStructProperty::StaticClass())) {
			TFieldPath<FStructProperty>PStruct = CastFieldChecked<FStructProperty>(PSet->ElementProp);
			PType = TEXT("TSet< struct >");
			//
			if (PStruct->Struct==TBaseStructure<FColor>::Get()) {PType=TEXT("TSet< FColor >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FVector>::Get()) {PType=TEXT("TSet< FVector >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FRotator>::Get()) {PType=TEXT("TSet< FRotator >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FVector2D>::Get()) {PType=TEXT("TSet< FVector2D >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FTransform>::Get()) {PType=TEXT("TSet < FTransform >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FLinearColor>::Get()) {PType=TEXT("TSet< FLinearColor >"); return PType;}
			//
			PType = FString::Printf(TEXT("TSet< F%s >"),*PStruct->Struct->GetName());
		return PType;}
		//
		if (PSet->ElementProp && PSet->ElementProp->IsA(FObjectProperty::StaticClass())) {
			TFieldPath<FObjectProperty>POBJ = CastFieldChecked<FObjectProperty>(PSet->ElementProp);
			PType = TEXT("TSet< object >");
			//
			auto OBJ = POBJ->GetDefaultPropertyValue();
			if (OBJ!=nullptr) {PType=FString::Printf(TEXT("TSet< U%s >"),*OBJ->GetName());}
		return PType;}
	}///
	//
	if (Property->IsA(FMapProperty::StaticClass())) {
		TFieldPath<FMapProperty>PMap = CastFieldChecked<FMapProperty>(Property.Get());
		//
		if (PMap->KeyProp && PMap->KeyProp->IsA(FBoolProperty::StaticClass())) {PType=TEXT("TMap< bool, ... >"); return PType;}
		if (PMap->KeyProp && PMap->KeyProp->IsA(FIntProperty::StaticClass())) {PType=TEXT("TMap< int32, ... >"); return PType;}
		if (PMap->KeyProp && PMap->KeyProp->IsA(FByteProperty::StaticClass())) {PType=TEXT("TMap< uint8, ... >"); return PType;}
		if (PMap->KeyProp && PMap->KeyProp->IsA(FFloatProperty::StaticClass())) {PType=TEXT("TMap< float, ... >"); return PType;}
		//
		if (PMap->KeyProp && PMap->KeyProp->IsA(FNameProperty::StaticClass())) {PType=TEXT("TMap< FName, ... >"); return PType;}
		if (PMap->KeyProp && PMap->KeyProp->IsA(FTextProperty::StaticClass())) {PType=TEXT("TMap< FText, ... >"); return PType;}
		if (PMap->KeyProp && PMap->KeyProp->IsA(FStrProperty::StaticClass())) {PType=TEXT("TMap< FString, ... >"); return PType;}
		//
		if (PMap->KeyProp && PMap->KeyProp->IsA(FEnumProperty::StaticClass())) {
			PType = TEXT("TMap< enum, ... >");
			//
			UEnum* Enum = CastFieldChecked<FEnumProperty>(PMap->KeyProp)->GetEnum();
			PType = FString(TEXT("E"))+Enum->GetName();
		return PType;}
		//
		if (PMap->KeyProp && PMap->KeyProp->IsA(FClassProperty::StaticClass())) {
			TFieldPath<FClassProperty>PCLS = CastFieldChecked<FClassProperty>(PMap->KeyProp);
			PType = TEXT("TMap< class, ... >");
			//
			auto CLS = PCLS->GetDefaultPropertyValue();
			if (CLS!=nullptr) {PType=FString::Printf(TEXT("TMap< U%s, ... >"),*CLS->GetName());}
		return PType;}
		//
		if (PMap->KeyProp && PMap->KeyProp->IsA(FStructProperty::StaticClass())) {
			TFieldPath<FStructProperty>PStruct = CastFieldChecked<FStructProperty>(PMap->KeyProp);
			PType = TEXT("TMap< struct, ... >");
			//
			if (PStruct->Struct==TBaseStructure<FColor>::Get()) {PType=TEXT("TMap< FColor, ... >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FVector>::Get()) {PType=TEXT("TMap< FVector, ... >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FRotator>::Get()) {PType=TEXT("TMap< FRotator, ... >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FVector2D>::Get()) {PType=TEXT("TMap< FVector2D, ... >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FTransform>::Get()) {PType=TEXT("TMap< FTransform, ... >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FLinearColor>::Get()) {PType=TEXT("TMap< FLinearColor, ... >"); return PType;}
			//
			PType = FString::Printf(TEXT("TMap< F%s >"),*PStruct->Struct->GetName());
		return PType;}
		//
		if (PMap->KeyProp && PMap->KeyProp->IsA(FObjectProperty::StaticClass())) {
			TFieldPath<FObjectProperty>POBJ = CastFieldChecked<FObjectProperty>(PMap->KeyProp);
			PType = TEXT("TMap< object, ... >");
			//
			auto OBJ = POBJ->GetDefaultPropertyValue();
			if (OBJ!=nullptr) {PType=FString::Printf(TEXT("TMap< U%s, ... >"),*OBJ->GetName());}
		return PType;}
	}///
	//
	if (Property->IsA(FArrayProperty::StaticClass())) {
		TFieldPath<FArrayProperty>PArray = CastFieldChecked<FArrayProperty>(Property.Get());
		//
		if (PArray->Inner && PArray->Inner->IsA(FBoolProperty::StaticClass())) {PType=TEXT("TArray< bool >"); return PType;}
		if (PArray->Inner && PArray->Inner->IsA(FIntProperty::StaticClass())) {PType=TEXT("TArray< int32 >"); return PType;}
		if (PArray->Inner && PArray->Inner->IsA(FByteProperty::StaticClass())) {PType=TEXT("TArray< uint8 >"); return PType;}
		if (PArray->Inner && PArray->Inner->IsA(FFloatProperty::StaticClass())) {PType=TEXT("TArray< float >"); return PType;}
		//
		if (PArray->Inner && PArray->Inner->IsA(FNameProperty::StaticClass())) {PType=TEXT("TArray< FName >"); return PType;}
		if (PArray->Inner && PArray->Inner->IsA(FTextProperty::StaticClass())) {PType=TEXT("TArray< FText >"); return PType;}
		if (PArray->Inner && PArray->Inner->IsA(FStrProperty::StaticClass())) {PType=TEXT("TArray< FString >"); return PType;}
		//
		if (PArray->Inner && PArray->Inner->IsA(FEnumProperty::StaticClass())) {
			PType = TEXT("TArray< enum >");
			//
			UEnum* Enum = CastFieldChecked<FEnumProperty>(PArray->Inner)->GetEnum();
			PType = FString(TEXT("E"))+Enum->GetName();
		return PType;}
		//
		if (PArray->Inner && PArray->Inner->IsA(FClassProperty::StaticClass())) {
			TFieldPath<FClassProperty>PCLS = CastFieldChecked<FClassProperty>(PArray->Inner);
			PType = TEXT("TArray< class >");
			//
			auto CLS = PCLS->GetDefaultPropertyValue();
			if (CLS!=nullptr) {PType=FString::Printf(TEXT("TArray< U%s, ... >"),*CLS->GetName());}
		return PType;}
		//
		if (PArray->Inner && PArray->Inner->IsA(FStructProperty::StaticClass())) {
			TFieldPath<FStructProperty>PStruct = CastFieldChecked<FStructProperty>(PArray->Inner);
			PType = TEXT("TArray< struct >");
			//
			if (PStruct->Struct==TBaseStructure<FColor>::Get()) {PType=TEXT("TArray< FColor >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FVector>::Get()) {PType=TEXT("TArray< FVector >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FRotator>::Get()) {PType=TEXT("TArray< FRotator >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FVector2D>::Get()) {PType=TEXT("TArray< FVector2D >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FTransform>::Get()) {PType=TEXT("TArray< FTransform >"); return PType;}
			if (PStruct->Struct==TBaseStructure<FLinearColor>::Get()) {PType=TEXT("TArray< FLinearColor >"); return PType;}
			//
			PType = FString::Printf(TEXT("TArray< F%s >"),*PStruct->Struct->GetName());
		return PType;}
		//
		if (PArray->Inner && PArray->Inner->IsA(FObjectProperty::StaticClass())) {
			TFieldPath<FObjectProperty>POBJ = CastFieldChecked<FObjectProperty>(PArray->Inner);
			PType=TEXT("TArray< object >");
			//
			auto OBJ = POBJ->GetDefaultPropertyValue();
			if (OBJ!=nullptr) {PType=FString::Printf(TEXT("TArray< U%s, ... >"),*OBJ->GetName());}
		return PType;}
	}///
	//
	return PType;
}

bool UMGC_SemanticDB::IsValidTarget(UClass* Class) const {
	bool IsValid = true;
	//
	if (Class->HasAnyClassFlags(CLASS_Deprecated|CLASS_NewerVersionExists)) {IsValid=false;}
	//
	return IsValid;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////