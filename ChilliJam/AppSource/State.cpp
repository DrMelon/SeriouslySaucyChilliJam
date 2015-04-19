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

// Associated Header
#include <State.h>

// Required Engine Header
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

// Required Application Header
#include <App.h>
#include <State_DayBegin.h>
#include <State_Score.h>

using std::string;

namespace ChilliJam
{
	void State::CreateSystems()
	{
		AudioPlayer = CreateSystem<CSAudio::CkAudioPlayer>();
	}

	void State::OnInit()
	{
		CurrentTime = 0;

		App* application = (App*) CSCore::Application::Get();

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

		// Begin playing music.
		AudioPlayer->PlayMusic(CSCore::StorageLocation::k_package, "Audio/Nightlife.ogg");

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

		// Add people to the screen
		for ( unsigned int person = 0; person < application->GetConsumers(); person++ )
		{
			char randomface[2];
			{
				int random = ( (int) rand() % 3 ) + 1;
				sprintf( randomface, "%i", random );
			}
			CSRendering::SpriteComponentSPtr spritecomponent = rendercomponentfactory->CreateSpriteComponent(
				CSCore::Vector2( 32, 32 ),
				textureatlas_face,
				randomface,
				material_face,
				CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect
				);
			CSCore::EntitySPtr spriteentity = CSCore::Entity::Create();
			spriteentity->AddComponent( spritecomponent );
			spriteentity->GetTransform().SetPosition( CSCore::Vector3( ( rand() % 64 ) + 32, ( rand() % 64 ) - 96, 0 ) );

			GetScene()->Add( spriteentity );
			PersonStruct personinfo;
			{
				personinfo.Sprite = spriteentity;
				personinfo.Affected_Time = 5 + ( (float) ( rand() % 100 ) / 10 );
				personinfo.Sound_Time = 0;
				personinfo.Direction = CSCore::Vector2( ( rand() % 3 ) - 1, ( rand() % 3 ) - 1 );
				{
					// Ensure they move in a direction
					if ( ( personinfo.Direction.x == 0 ) && ( personinfo.Direction.y == 0 ) )
					{
						personinfo.Direction.x = 1;
					}
					// Add some more random movement
					float randomadd = (float) ( ( ( rand() % 2 ) == 0 ) ? 1 : -1 ) * 0.5f / ( ( rand() % 10 ) + 1 );
					if ( personinfo.Direction.x == 0 )
					{
						personinfo.Direction.x = randomadd;
					}
					if ( personinfo.Direction.y == 0 )
					{
						personinfo.Direction.y = randomadd;
					}
					// Random speed decrease
					personinfo.Direction.x /= ( rand() % 3 ) + 1;
					personinfo.Direction.y /= ( rand() % 3 ) + 1;
				}
				personinfo.Face[0] = randomface[0];
				personinfo.Face[1] = '\0'; // Terminate string
			}
			Person.push_back( personinfo );
		}

		// Initialize affected logic
		Affected_Current = 0;
		Affected_Target = Person.size() / ( ( rand() % 3 ) + 1 );
		{
			if ( application->GetDay() == 1 )
			{
				Affected_Target = 1;
			}
		}

		AudioBank = resourcepool->LoadResource<CSAudio::CkBank>( CSCore::StorageLocation::k_package, "Audio/bank.ckb" );

		// Load the HUD ui widget (THIS APPEARS IN EVERY STATE BECAUSE I'M A BAD PERSON -M)
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
		char dollabuffer[50];
		std::sprintf( dollabuffer, "$%.2f", amt );
		string dolla( dollabuffer );

		UI_HUD->GetWidget( "Dolla" )->GetComponent<CSUI::TextComponent>()->SetText( dolla );
	}

	void State::OnUpdate( f32 in_deltaTime )
	{
		CurrentTime += in_deltaTime;

		// Update the people
		for ( unsigned int person = 0; person < Person.size(); person++ )
		{
			// Move the people
			CSCore::EntitySPtr personsprite = Person[person].Sprite;
			personsprite->GetTransform().MoveBy( CSCore::Vector3( ( (float) ( rand() % 5 ) - 2.5 ) * Person[person].Direction.x, ( (float) ( rand() % 5 ) - 2.5 ) * Person[person].Direction.y, 0 ) );

			// Randomly affect the people that haven't been flagged affected, when there are still people in the target
			if ( ( CurrentTime > Person[person].Affected_Time ) && ( Person[person].Affected_Time != 0 ) && ( Affected_Current < Affected_Target ) )
			{
				Affected_Current++;
				personsprite->RemoveFromParent(); // Remove from scene
				Person[person].Affected_Time = 0; // Flag to not affect again

				// Add juice to count
				App* application = (App*) CSCore::Application::Get();
				application->AddJuice( 10 );

				// Play sound
				AudioPlayer->PlayEffect( AudioBank, "Person_Pop" );
				Person[person].Sound_Time = CurrentTime + 1;

				// Create effect
				EffectStruct effect;
				{
					for ( unsigned int piece = 0; piece < ( rand() % 3 ) + 6; piece++ )
					{
						CSCore::EntitySPtr sprite = CSCore::Entity::Create();
						{
							// Get resources
							auto resourcepool = CSCore::Application::Get()->GetResourcePool();
							CSRendering::RenderComponentFactory* rendercomponentfactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
							CSRendering::TextureAtlasCSPtr textureatlas_face = resourcepool->LoadResource<CSRendering::TextureAtlas>( CSCore::StorageLocation::k_package, "TextureAtlases/alienfaces/alienfaces.csatlas" );
							CSRendering::MaterialCSPtr material_face = resourcepool->GetResource<CSRendering::Material>( "SpriteMaterial_Faces" );

							// Create component
							CSRendering::SpriteComponentSPtr spritecomponent = rendercomponentfactory->CreateSpriteComponent(
								CSCore::Vector2( 16, 16 ),
								textureatlas_face,
								string( Person[person].Face ),
								material_face,
								CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect
							);
							sprite->AddComponent( spritecomponent );
							sprite->GetTransform().SetPosition( personsprite->GetTransform().GetWorldPosition() ); // Position at parent

							// Colour correctly for lerped darkness
							CSRendering::SpriteComponentSPtr parentsprite = Person[person].Sprite->GetComponent<CSRendering::SpriteComponent>();
							spritecomponent->SetColour( parentsprite->GetColour() );

							// Add to scene
							GetScene()->Add( sprite );
						}
						effect.Sprite.push_back( sprite );
					}
					effect.Affected_Time = CurrentTime + 4;
					effect.Direction = Person[person].Direction;
				}
				Effect.push_back( effect );
			}

			// Play cheering after a delay
			if ( ( Person[person].Sound_Time != 0 ) && ( Person[person].Sound_Time <= CurrentTime ) )
			{
				AudioPlayer->PlayEffect( AudioBank, "Person_Yay" );
				Person[person].Sound_Time = 0;
			}
		}

		// Update the effects
		for ( unsigned int effect = 0; effect < Effect.size(); effect++ )
		{
			// Move the people
			for ( unsigned int sprite = 0; sprite < Effect[effect].Sprite.size(); sprite++ )
			{
				CSCore::EntitySPtr personsprite = Effect[effect].Sprite[sprite];
				personsprite->GetTransform().MoveBy( CSCore::Vector3( ( (float) ( rand() % 5 ) - 2.5 ) * Effect[effect].Direction.x, ( (float) ( rand() % 5 ) - 2.5 ) * Effect[effect].Direction.y, 0 ) );
			}
		}

		// Update the darkness of the screen
		CSCore::SharedEntityList entities = GetScene()->GetEntities();
		for ( unsigned int entity = 0; entity < entities.size(); entity++ )
		{
			// Is renderable as a sprite
			CSRendering::SpriteComponentSPtr sprite = entities[entity]->GetComponent<CSRendering::SpriteComponent>();
			if ( sprite )
			{
				CSCore::Colour oldcolour = sprite->GetColour();
				{
					oldcolour.r -= in_deltaTime * LIGHT_CHANGE;
					oldcolour.g -= in_deltaTime * LIGHT_CHANGE;
					oldcolour.b -= in_deltaTime * LIGHT_CHANGE;
					// Clamp
					if ( oldcolour.r < LIGHT_MIN )
					{
						oldcolour.r = LIGHT_MIN;
						oldcolour.g = LIGHT_MIN;
						oldcolour.b = LIGHT_MIN;
					}
				}
				sprite->SetColour( oldcolour );
			}
		}

		// Move to score when all people have been affected
		if ( Affected_Current >= Affected_Target )
		{
			// Also check that effects are finished
			bool finished = true;
			{
				for ( unsigned int effect = 0; effect < Effect.size(); effect++ )
				{
					// Hasn't timed out the effect yet
					if ( Effect[effect].Affected_Time > CurrentTime )
					{
						finished = false;
						break;
					}
				}
			}
			if ( finished )
			{
				App* application = (App*) CSCore::Application::Get();
				{
					application->SetConsumers( Effect.size() );
				}
				State_Score* state = new State_Score();
				{
					state->scoreController.setPrevTotalMoney( application->GetPreviousDolla() );
					state->scoreController.setPrevTotalJuice( application->GetPreviousJuice() );
					state->scoreController.addMoneyToTotal( application->GetDolla() );
					state->scoreController.addJuiceToTotal( application->GetJuice() );

					// Afterwards set the new value to previous, in preporation for the next day
					application->SetPreviousDolla( application->GetDolla() );
					application->SetPreviousJuice( application->GetJuice() );
				}
				application->GetStateManager()->Change( (CSCore::StateSPtr) state );
			}
		}
	}

	void State::OnDestroy()
	{
		Person.clear();
	}
}