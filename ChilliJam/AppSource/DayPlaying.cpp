#include <DayPlaying.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>
#include "State_DayBegin.h"

#include <App.h>

using std::string;

namespace ChilliJam
{
	void DayPlayingState::CreateSystems()
	{

	}

	void DayPlayingState::OnInit()
	{

		// Set number of customers etc
		customersToday = 10;


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
		auto cartAtlas = theResourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/cart/cart.csatlas");
		auto vendorTexture = theResourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/vendor/vendor.csimage");
		auto vendorAtlas = theResourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/vendor/vendor.csatlas");
		auto foodTexture = theResourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/food/food.csimage");
		auto foodAtlas = theResourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/food/food.csatlas");
		auto bubbleTexture = theResourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_package, "TextureAtlases/thoughtbubbles/thoughtbubbles.csimage");
		

		// Create materials
		CSRendering::MaterialCSPtr littleAliensMaterial;
		CSRendering::MaterialCSPtr cartMaterial;
		CSRendering::MaterialCSPtr vendorMaterial;
		CSRendering::MaterialCSPtr foodMaterial;
		CSRendering::MaterialCSPtr bubbleMaterial;

		if (theResourcePool->GetResource<CSRendering::Material>("littleAliensMat") == nullptr)
		{
			auto littleAliensMaterial_s = theMaterialFactory->CreateSprite("littleAliensMat", littleAliensTexture);
			littleAliensMaterial_s->SetTransparencyEnabled(true);
			auto cartMaterial_s = theMaterialFactory->CreateSprite("cartMat", cartTexture);
			cartMaterial_s->SetTransparencyEnabled(true);
			auto vendorMaterial_s = theMaterialFactory->CreateSprite("vendorMat", vendorTexture);
			vendorMaterial_s->SetTransparencyEnabled(true);
			auto foodMaterial_s = theMaterialFactory->CreateSprite("foodMat", foodTexture);
			foodMaterial_s->SetTransparencyEnabled(true);
			auto bubbleMaterial_s = theMaterialFactory->CreateSprite("bubbleMat", bubbleTexture);
			bubbleMaterial_s->SetTransparencyEnabled(true);
		}

		littleAliensMaterial = theResourcePool->GetResource<CSRendering::Material>("littleAliensMat");
		cartMaterial = theResourcePool->GetResource<CSRendering::Material>("cartMat");
		vendorMaterial = theResourcePool->GetResource<CSRendering::Material>("vendorMat");
		foodMaterial = theResourcePool->GetResource<CSRendering::Material>("foodMat");
		bubbleMaterial = theResourcePool->GetResource<CSRendering::Material>("bubbleMat");
		
	// Initialize food types
		foodOne.foodAtlas = foodAtlas;
		foodOne.foodMaterial = foodMaterial;
		foodOne.foodType = "0";
		foodTwo = foodOne;


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
		CSRendering::SpriteComponentSPtr vendorSprite = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2::k_one, vendorAtlas, "person", vendorMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		CSCore::EntitySPtr vendorEntity = CSCore::Entity::Create();
		vendorEntity->AddComponent(vendorSprite);
		vendorEntity->GetTransform().SetPosition(-23, 2.5, -14.0);
		vendorEntity->GetTransform().ScaleBy(15);


		
		
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
		GetScene()->Add(cartEntity);
		GetScene()->Add(vendorEntity);

		// Crowd sprites
		for (int i = 0; i < customersToday; i++)
		{
			ShopCustomer* newCustomer = new ShopCustomer(renderComponentFactory, littleAliensAtlas);
			newCustomer->posInQueue = i;

			// Assign food type to buy & wanted food type (if wanted food type is available today, buy that one)
			newCustomer->wantedFoodType = rand() % 5;
			std::string wanttype = std::to_string(newCustomer->wantedFoodType);
			//random choice first
			if (rand() % 2 == 0)
			{
				newCustomer->targetFood = foodOne;
			}
			else
			{
				newCustomer->targetFood = foodTwo;
			}
			newCustomer->gotwantedfood = false;

			// then picked choice second
			if (foodOne.foodType == wanttype)
			{
				newCustomer->targetFood = foodOne;
				newCustomer->gotwantedfood = true;
			}
			if (foodTwo.foodType == wanttype)
			{
				newCustomer->targetFood = foodTwo;
				newCustomer->gotwantedfood = true;
			}
			

			GetScene()->Add(newCustomer->myEntity);
			if (i > 0)
			{
				customersList.at(i - 1)->nextInLine = newCustomer;
			}

			customersList.push_back(newCustomer);
		}

		// Load the HUD ui widget (THIS APPEARS IN EVERY STATE BECAUSE I'M A BAD PERSON -M)
		App* application = (App*) CSCore::Application::Get();

		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();
		auto widgetfactory = CSCore::Application::Get()->GetWidgetFactory();

		auto templatehudwidget = resourcepool->LoadResource<CSUI::WidgetTemplate>( CSCore::StorageLocation::k_package, "UI/HUD.csui" );

		UI_HUD = widgetfactory->Create( templatehudwidget );
		GetUICanvas()->AddWidget( UI_HUD );

		// Convert day number to string and display on HUD
		char buffer[10];
		{
			itoa( application->GetDay(), buffer, 10 );
		}
		string day( buffer );
		UI_HUD->GetWidget( "Day" )->GetComponent<CSUI::TextComponent>()->SetText( day );
	}

	void DayPlayingState::OnUpdate(f32 in_deltaTime)
	{
		// Update stuff here.
		// Tween the camera Y value
		cameraYValue = cameraTween.Update(in_deltaTime);
		
		
		cameraEntity->GetTransform().SetLookAt(CSCore::Vector3(0.0f, cameraYValue, -50.0f - cameraYValue), CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY);

		for (int i = 0; i < customersList.size(); i++)
		{
			customersList.at(i)->Update(in_deltaTime);
		}

	}

	void DayPlayingState::OnDestroy()
	{
		// Destruction stuff here.
		for (int i = 0; i < customersList.size(); i++)
		{
			if (customersList.at(i)->thoughtBubble != nullptr)
			{
				delete customersList.at(i)->thoughtBubble;
			}
			delete (customersList.at(i));
			(customersList.at(i)) = 0;
		}
	}




	// Customer Stuff

	ShopCustomer::ShopCustomer(CSRendering::RenderComponentFactory* n_renderComponentFactory, std::shared_ptr<const CSRendering::TextureAtlas> alienAtlas)
	{
		renderComponentFactory = n_renderComponentFactory;
		thoughtBubble = nullptr;
		// Alien Type Stuff
		alienType = rand()%5;
		queueing = true;
		hasChilli = false;
		shopPosition = CSCore::Vector3(25 + rand()%5, 0.5, -54); //Want them to come in from the right front
		targetPosition = shopPosition;
		posInQueue = 0;
		// ChilliSource Stuff (Note: for different aliens change p1_**** to be p2_ etc)
		// Construct px_front and px_back string
		std::string firstbit = "p";
		std::string backlastbit = "_back";
		std::string frontlastbit = "_front";
		std::string numberstring = std::to_string(alienType);
		std::string frontstring = firstbit + numberstring + frontlastbit;
		std::string backstring = firstbit + numberstring + backlastbit;
		auto theResourcePool = CSCore::Application::Get()->GetResourcePool();
		CSRendering::MaterialCSPtr alienMaterial = theResourcePool->GetResource<CSRendering::Material>("littleAliensMat");

		frontSprite = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2::k_one, alienAtlas, frontstring, alienMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		backSprite = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2::k_one, alienAtlas, backstring, alienMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		frontSprite->SetVisible(!queueing);
		backSprite->SetVisible(queueing);
		myEntity = CSCore::Entity::Create();
		myEntity->AddComponent(frontSprite);
		myEntity->AddComponent(backSprite);
		myEntity->GetTransform().SetPosition(shopPosition);
		myEntity->GetTransform().ScaleBy(10);
		hopAmt = rand() % 4;
		nextInLine = nullptr;

		//Init sean citizen class
		internalCustomerStuff = new Customer();
		internalCustomerStuff->setCustomerValues();
	}

	void ShopCustomer::Update(float dt)
	{
		if (thoughtBubble != nullptr)
		{
			thoughtBubble->Update(dt);
		}
		// Make sure the visibility of the sprites is right; when a person stops queueing (and starts getting chilli) they face the camera.
		frontSprite->SetVisible(!queueing);
		backSprite->SetVisible(queueing);

		if (rand() % 1000 == 0 && thoughtBubble == nullptr)
		{
			thoughtBubble = new ThoughtBubble(renderComponentFactory, targetFood, shopPosition, myEntity->GetScene());
		}
		
		// Always move towards the target position
		CSCore::Vector3 moveAmt = ((targetPosition - shopPosition) * dt);
		moveAmt.Clamp(CSCore::Vector3(-1.0, -1.0, -1.0), CSCore::Vector3(1.0f, 1.0f, 1.0f));
		//hop
		shopPosition.y = sin((shopPosition.x + shopPosition.z + hopAmt) * (5)) * 0.5f;
		shopPosition += moveAmt;

		// Target position z = their position in the queue
		if (queueing)
		{
			targetPosition.z = (-16.0f) - posInQueue * 2;
		}
		if (queueing && !hasChilli && posInQueue == 0)
		{
			// head towards cart
			targetPosition = CSCore::Vector3(-20, 0, -16);

			// Are we close to it?
			if (shopPosition.x < -19.5f && shopPosition.x > -20.5f && shopPosition.z < -15.5f && shopPosition.z > -16.5f)
			{
				// get some chilli
				queueing = false;
				hasChilli = true;
				AddFood(targetFood);
				GoNext();
			}
		}
		if (!queueing && hasChilli)
		{
			// you need to leave, sir
			targetPosition = CSCore::Vector3(-25, 0, -60);

			if (shopPosition.z < -55 && nextInLine == nullptr)
			{
				// last one has left the shop... O:
				
				// set up external state application stuff (like moneys, how many people today etc)

				// go next state
				CSCore::Application::Get()->GetStateManager()->Change((CSCore::StateSPtr) new State_DayBegin());

			}
		}

		

		// Update position in scene
		myEntity->GetTransform().SetPosition(shopPosition);
	}

	void ShopCustomer::AddFood(FoodStruct food)
	{
		//Create a sprite for a food item
		CSRendering::SpriteComponentSPtr foodSprite = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2::k_one, food.foodAtlas, food.foodType, food.foodMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		myEntity->AddComponent(foodSprite);

		// If they got their wanted food, add a smiley face above them or something & pay double moneys
		if (gotwantedfood)
		{
			//chaching!!
		}
	}

	void ShopCustomer::GoNext()
	{
		// move people in the queue up
		if (nextInLine != nullptr)
		{
			nextInLine->posInQueue--;
			nextInLine->GoNext();
		}
	}

	ThoughtBubble::ThoughtBubble(CSRendering::RenderComponentFactory* n_renderComponentFactory, FoodStruct foodToThinkOf, CSCore::Vector3 startPos, CSCore::Scene* scenePointer)
	{
		renderComponentFactory = n_renderComponentFactory;
		bubblePos = CSCore::Vector3(startPos);
		auto theResourcePool = CSCore::Application::Get()->GetResourcePool();
		auto theMaterialFactory = CSCore::Application::Get()->GetSystem<CSRendering::MaterialFactory>();
		xOffset = bubblePos.x;
		// Get materials from resource pool
		auto bubbleAtlas = theResourcePool->LoadResource<CSRendering::TextureAtlas>(CSCore::StorageLocation::k_package, "TextureAtlases/thoughtbubbles/thoughtbubbles.csatlas");

		auto bubbleMaterial = theResourcePool->GetResource<CSRendering::Material>("bubbleMat");


		// Create thought bubble elements
		bubbleSpriteSmallest = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2(0.12, 0.12), bubbleAtlas, "bubble", bubbleMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		bubbleSpriteSmaller = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2(0.25, 0.25), bubbleAtlas, "bubble", bubbleMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		bubbleSpriteSmall = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2(0.5, 0.5), bubbleAtlas, "bubble", bubbleMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		bubbleSpriteMain = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2(1.0, 1.0), bubbleAtlas, "bubble", bubbleMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);
		foodInsideBubble = renderComponentFactory->CreateSpriteComponent(CSCore::Vector2::k_one, foodToThinkOf.foodAtlas, foodToThinkOf.foodType, foodToThinkOf.foodMaterial, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect);

		bubbleMainEntity = CSCore::Entity::Create();
		bubbleSmall = CSCore::Entity::Create();
		bubbleSmaller = CSCore::Entity::Create();
		bubbleSmallest = CSCore::Entity::Create();

		bubbleMainEntity->AddComponent(bubbleSpriteMain);
		bubbleMainEntity->AddComponent(foodInsideBubble);
		bubbleSmall->AddComponent(bubbleSpriteSmall);
		bubbleSmall->AddComponent(bubbleSpriteSmaller);
		bubbleSmall->AddComponent(bubbleSpriteSmallest);

		bubbleMainEntity->GetTransform().SetPosition(bubblePos);
		bubbleSmall->GetTransform().SetPosition(bubblePos - CSCore::Vector3(0, 2, 0));
		bubbleSmaller->GetTransform().SetPosition(bubblePos - CSCore::Vector3(0, 2.5, 0));
		bubbleSmallest->GetTransform().SetPosition(bubblePos - CSCore::Vector3(0, 2.25, 0));
		bubbleMainEntity->GetTransform().ScaleBy(5);
		bubbleSmall->GetTransform().ScaleBy(5);
		bubbleSmaller->GetTransform().ScaleBy(5);
		bubbleSmallest->GetTransform().ScaleBy(5);
	
		scenePointer->Add(bubbleMainEntity);
		scenePointer->Add(bubbleSmall);
		scenePointer->Add(bubbleSmaller);
		scenePointer->Add(bubbleSmallest);

	}

	void ThoughtBubble::Update(float deltaTime)
	{
		bubblePos.y += 0.1f;
		bubblePos.x = xOffset + sin(bubblePos.y / 5.0f) * 1.0f;
		bubbleMainEntity->GetTransform().SetPosition(bubblePos);
		bubbleSmall->GetTransform().SetPosition(bubblePos - CSCore::Vector3(0, 5, 0));
		bubbleSmaller->GetTransform().SetPosition(bubblePos - CSCore::Vector3(0, 7.5, 0));
		bubbleSmallest->GetTransform().SetPosition(bubblePos - CSCore::Vector3(0, 10.25, 0));

	}

}
