//
//  State.cpp
//  ChilliJam
//  Created by Ian Copland on 15/10/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>

#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/Rendering/Material.h>

#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

#include <App.h>

using std::string;

namespace ChilliJam
{
	void State::CreateSystems()
	{
		//Add systems here.
	}

	void State::OnInit()
	{
		// Create the camera entity and add the camera component
		CSRendering::RenderComponentFactory* rendercomponentfactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
		CSCore::EntitySPtr Camera = CSCore::Entity::Create();
		{
			// Create the camera component
			CSRendering::CameraComponentSPtr cameraComponent = rendercomponentfactory->CreateOrthographicCameraComponent( CSCore::Vector2( 1920, 1080 ), 1.0f, 100.0f );

			Camera->AddComponent( cameraComponent );
			Camera->GetTransform().SetLookAt( CSCore::Vector3::k_unitNegativeZ * (f32) 10, CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY );
		}
		GetScene()->Add( Camera );

		// Get a reference to the material creation factory
		CSRendering::MaterialFactory* materialfactory = CSCore::Application::Get()->GetSystem<CSRendering::MaterialFactory>();

		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();

		CSRendering::TextureAtlasCSPtr textureatlas = resourcepool->LoadResource<CSRendering::TextureAtlas>( CSCore::StorageLocation::k_package, "TextureAtlases/city/city.csatlas" );

		// Get the material if it already exists
		if ( resourcepool->GetResource<CSRendering::Material>( "SpriteMaterial_City" ) == 0 )
		{
			// Load the texture and texture atlas
			CSRendering::TextureCSPtr texture = resourcepool->LoadResource<CSRendering::Texture>( CSCore::StorageLocation::k_package, "TextureAtlases/city/city.csimage" );

			// Create the material
			CSRendering::MaterialSPtr material = materialfactory->CreateSprite( "SpriteMaterial_City", texture );
			material->SetBlendModes( CSRendering::BlendMode::k_one, CSRendering::BlendMode::k_oneMinusSourceAlpha );
			material->SetTransparencyEnabled( true );
		}
		CSRendering::MaterialCSPtr material = resourcepool->GetResource<CSRendering::Material>( "SpriteMaterial_City" );

		CSRendering::TextureAtlasCSPtr textureatlas_face = resourcepool->LoadResource<CSRendering::TextureAtlas>( CSCore::StorageLocation::k_package, "TextureAtlases/alienfaces/alienfaces.csatlas" );

		// Get the material if it already exists
		if ( resourcepool->GetResource<CSRendering::Material>( "SpriteMaterial_Faces" ) == 0 )
		{
			// Load the texture and texture atlas
			CSRendering::TextureCSPtr texture = resourcepool->LoadResource<CSRendering::Texture>( CSCore::StorageLocation::k_package, "TextureAtlases/alienfaces/alienfaces.csimage" );

			// Create the material
			CSRendering::MaterialSPtr material = materialfactory->CreateSprite( "SpriteMaterial_Faces", texture );
			material->SetBlendModes( CSRendering::BlendMode::k_one, CSRendering::BlendMode::k_oneMinusSourceAlpha );
			material->SetTransparencyEnabled( true );
		}
		CSRendering::MaterialCSPtr material_face = resourcepool->GetResource<CSRendering::Material>( "SpriteMaterial_Faces" );

		{
			// City base
			CSRendering::SpriteComponentSPtr spritecomponent = rendercomponentfactory->CreateSpriteComponent(
				CSCore::Vector2( 1920, 1080 ),
				textureatlas,
				"city",
				material,
				CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect
			);
			CSCore::EntitySPtr spriteentity = CSCore::Entity::Create();
			spriteentity->AddComponent( spritecomponent );

			GetScene()->Add( spriteentity );

			// City tree tops & building tops above other objects
			CSRendering::SpriteComponentSPtr spritecomponent2 = rendercomponentfactory->CreateSpriteComponent(
				CSCore::Vector2( 1920, 1080 ),
				textureatlas,
				"city2",
				material,
				CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect
			);
			CSCore::EntitySPtr spriteentity2 = CSCore::Entity::Create();
			spriteentity2->AddComponent( spritecomponent2 );
			spriteentity2->GetTransform().SetPosition( CSCore::Vector3( 0, 0, -9 ) );

			GetScene()->Add( spriteentity2 );
		}

		{
			// City base
			CSRendering::SpriteComponentSPtr spritecomponent = rendercomponentfactory->CreateSpriteComponent(
				CSCore::Vector2( 32, 32 ),
				textureatlas_face,
				"1",
				material_face,
				CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect
			);
			CSCore::EntitySPtr spriteentity = CSCore::Entity::Create();
			spriteentity->AddComponent( spritecomponent );
			spriteentity->GetTransform().SetPosition( CSCore::Vector3( 64, -118, 0 ) );

			GetScene()->Add( spriteentity );
		}

		// Load the HUD ui widget (THIS APPEARS IN EVERY STATE BECAUSE I'M A BAD PERSON -M)
		App* application = (App*) CSCore::Application::Get();

		// Get a reference to the resource pool for this application
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

		// Convert $$$$ number to string and display on HUD
		float amt = application->GetDolla();
		string dolla = std::to_string(amt);
		UI_HUD->GetWidget("Dolla")->GetComponent<CSUI::TextComponent>()->SetText(dolla);
	}

	void State::OnUpdate( f32 in_deltaTime )
	{
		//Update stuff here.
	}

	void State::OnDestroy()
	{
		//Destruction stuff here.
	}
}
