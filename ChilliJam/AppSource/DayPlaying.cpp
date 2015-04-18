#include <DayPlaying.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Texture.h>

namespace ChilliJam
{
	void DayPlayingState::CreateSystems()
	{

	}

	void DayPlayingState::OnInit()
	{
		// Create the camera component
		CSRendering::RenderComponentFactory* renderComponentFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
		CSRendering::CameraComponentSPtr cameraComponent = renderComponentFactory->CreatePerspectiveCameraComponent(CSCore::MathUtils::k_pi / 2.0f, 1.0f, 1000.0f);

		// Create the camera entity and add the camera component
		cameraEntity = CSCore::Entity::Create();
		cameraEntity->AddComponent(cameraComponent);
		

		// Add the camera to the scene
		GetScene()->Add(cameraEntity);

		// Load resource pool
		auto theResourcePool = CSCore::Application::Get()->GetResourcePool();
		// Load material factory
		auto theMaterialFactory = CSCore::Application::Get()->GetSystem<CSRendering::MaterialFactory>();

		// Load 2D resources (sprites and textures)
		auto tiledFloorMaterial = theResourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/tiledfloor.csmaterial");
		auto wallsMaterial = theResourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/walls.csmaterial");
		auto backwallsMaterial = theResourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/backwall.csmaterial");

		// Load atlases
		auto littleAliensTexture = theResourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/LittleAliens/LittleAliens.csimage");
		auto littleAliensAtlas = theResourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/LittleAliens/LittleAliens.csatlas");
		auto cartTexture = theResourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/cart/cart.csimage");
		auto cartAtlas = theResourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/cart/cart.csatlas");

		// Create materials
		auto littleAliensMaterial = theMaterialFactory->CreateSprite("littleAliensMat", littleAliensTexture);
		littleAliensMaterial->SetTransparencyEnabled(true);
		auto cartMaterial = theMaterialFactory->CreateSprite("cartMat", cartTexture);
		cartMaterial->SetTransparencyEnabled(true);

		// Load 3D model resources (planes)
		auto simplePlaneModel = theResourcePool->LoadResource<CSRendering::Mesh>(CSCore::StorageLocation::k_package, "Models/plane.csmodel");

		// Add plane model for floor
		CSRendering::StaticMeshComponentSPtr floorPlane = renderComponentFactory->CreateStaticMeshComponent(simplePlaneModel, tiledFloorMaterial);
		CSCore::EntitySPtr floorPlaneEntity = CSCore::Entity::Create();
		floorPlaneEntity->AddComponent(floorPlane);

		// Position floor plane
		floorPlaneEntity->GetTransform().SetPosition(0, -5, -25);
		floorPlaneEntity->GetTransform().ScaleBy(75, 1, 50);

		// Add plane model for left wall
		CSRendering::StaticMeshComponentSPtr leftWallPlane = renderComponentFactory->CreateStaticMeshComponent(simplePlaneModel, wallsMaterial);
		CSCore::EntitySPtr leftWallPlaneEntity = CSCore::Entity::Create();
		leftWallPlaneEntity->AddComponent(leftWallPlane);

		// Position floor plane (WARNING: MAGIC NUMBERS OUT OF THE WAZOO)
		leftWallPlaneEntity->GetTransform().SetPosition(-75 / 2.0f, 5, -25);
		leftWallPlaneEntity->GetTransform().ScaleBy(50, 1, 20);                         
		leftWallPlaneEntity->GetTransform().SetOrientation(CSCore::Quaternion(CSCore::Vector3(1, 0, 0), -90 * 3.14 / 180) * CSCore::Quaternion(CSCore::Vector3(0, 1, 0), -90 * 3.14 / 180));

		// Add plane model for right wall
		CSRendering::StaticMeshComponentSPtr rightWallPlane = renderComponentFactory->CreateStaticMeshComponent(simplePlaneModel, wallsMaterial);
		CSCore::EntitySPtr rightWallPlaneEntity = CSCore::Entity::Create();
		rightWallPlaneEntity->AddComponent(rightWallPlane);

		// Position floor plane (WARNING: MAGIC NUMBERS OUT OF THE WAZOO)
		rightWallPlaneEntity->GetTransform().SetPosition(75 / 2.0f, 5, -25);
		rightWallPlaneEntity->GetTransform().ScaleBy(50, 1, 20);
		rightWallPlaneEntity->GetTransform().SetOrientation(CSCore::Quaternion(CSCore::Vector3(1, 0, 0), -90 * 3.14 / 180) * CSCore::Quaternion(CSCore::Vector3(0, 1, 0), 90 * 3.14 / 180));
		
		// Add plane model for ceiling
		CSRendering::StaticMeshComponentSPtr ceilingPlane = renderComponentFactory->CreateStaticMeshComponent(simplePlaneModel, wallsMaterial);
		CSCore::EntitySPtr ceilingPlaneEntity = CSCore::Entity::Create();
		ceilingPlaneEntity->AddComponent(ceilingPlane);

		// Position floor plane (WARNING: MAGIC NUMBERS OUT OF THE WAZOO)
		ceilingPlaneEntity->GetTransform().SetPosition(0, 15, -25);
		ceilingPlaneEntity->GetTransform().ScaleBy(75, 1, 50);

		// Add plane model for ceiling
		CSRendering::StaticMeshComponentSPtr backWall = renderComponentFactory->CreateStaticMeshComponent(simplePlaneModel, backwallsMaterial);
		
		CSCore::EntitySPtr backWallEntity = CSCore::Entity::Create();
		backWallEntity->AddComponent(backWall);

		// Position floor plane (WARNING: MAGIC NUMBERS OUT OF THE WAZOO)
		backWallEntity->GetTransform().SetPosition(0, 5, 0);
		backWallEntity->GetTransform().ScaleBy(75, 1, 20);
		backWallEntity->GetTransform().SetOrientation(CSCore::Quaternion(CSCore::Vector3(1, 0, 0), -90 * 3.14 / 180));
		
		// Cart Sprite
		CSRendering::SpriteComponentSPtr cartSprite = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2::k_one, cartMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		CSCore::EntitySPtr cartEntity = CSCore::Entity::Create();
		cartEntity->AddComponent(cartSprite);
		cartEntity->GetTransform().SetPosition(-20, 0, -15.5);
		cartEntity->GetTransform().ScaleBy(15);

		// Vendor Sprite

		// Crowd sprites
		// make std vector of these little ones
		CSRendering::SpriteComponentSPtr testCrowdSprite = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2::k_one, littleAliensAtlas, "p1_front", littleAliensMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		CSCore::EntitySPtr testCrowdSpriteEntity = CSCore::Entity::Create();
		testCrowdSpriteEntity->AddComponent(testCrowdSprite);
		testCrowdSpriteEntity->GetTransform().SetPosition(0, 5, -15);
		testCrowdSpriteEntity->GetTransform().ScaleBy(10);
		
		// Create camera tween
		cameraTween = CSCore::MakeEaseOutBackTween<f32>(-4.0, 5.0, 1.8, 1.0, 0.0);
		cameraTween.Play(CSCore::TweenPlayMode::k_once);


		// Scene construction
		GetScene()->SetClearColour(CSCore::Colour::k_black);
		
		GetScene()->Add(floorPlaneEntity);
		GetScene()->Add(leftWallPlaneEntity);
		GetScene()->Add(rightWallPlaneEntity);
		GetScene()->Add(ceilingPlaneEntity);
		GetScene()->Add(backWallEntity);
		GetScene()->Add(testCrowdSpriteEntity);
		GetScene()->Add(cartEntity);
	}

	void DayPlayingState::OnUpdate(f32 in_deltaTime)
	{
		// Update stuff here.
		// Tween the camera Y value
		cameraYValue = cameraTween.Update(in_deltaTime);
		
		
		cameraEntity->GetTransform().SetLookAt(CSCore::Vector3(0.0f, cameraYValue, -50.0f - cameraYValue), CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY);


	}

	void DayPlayingState::OnDestroy()
	{
		// Destruction stuff here.
	}
}
