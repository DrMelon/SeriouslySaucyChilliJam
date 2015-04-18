#include <DayPlaying.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>


namespace ChilliJam
{
	void DayPlayingState::CreateSystems()
	{

	}

	void DayPlayingState::OnInit()
	{
		// Create the camera component
		CSRendering::RenderComponentFactory* renderComponentFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
		CSRendering::CameraComponentSPtr cameraComponent = renderComponentFactory->CreatePerspectiveCameraComponent(CSCore::MathUtils::k_pi / 2.0f, 1.0f, 100.0f);

		// Create the camera entity and add the camera component
		CSCore::EntitySPtr cameraEntity = CSCore::Entity::Create();
		cameraEntity->AddComponent(cameraComponent);
		cameraEntity->GetTransform().SetLookAt(CSCore::Vector3(0.0f, 5.0f, -55.0f), CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY);

		// Add the camera to the scene
		GetScene()->Add(cameraEntity);

		// Load resource pool
		auto theResourcePool = CSCore::Application::Get()->GetResourcePool();

		// Load 2D resources (sprites and textures)
		auto tiledFloorMaterial = theResourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/tiledfloor.csmaterial");
		auto wallsMaterial = theResourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/walls.csmaterial");

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
		


		// Scene construction
		GetScene()->SetClearColour(CSCore::Colour::k_black);
		
		GetScene()->Add(floorPlaneEntity);
		GetScene()->Add(leftWallPlaneEntity);
		GetScene()->Add(rightWallPlaneEntity);
		GetScene()->Add(ceilingPlaneEntity);
	}

	void DayPlayingState::OnUpdate(f32 in_deltaTime)
	{
		// Update stuff here.
	}

	void DayPlayingState::OnDestroy()
	{
		// Destruction stuff here.
	}
}
