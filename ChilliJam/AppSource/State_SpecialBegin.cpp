//
//  State_SpecialBegin.cpp
//  ChilliJam
//  Created by Matthew Cormack on 17/04/2015
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
#include <State_SpecialBegin.h>

// Required Engine Header
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/Resource.h>

#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>

#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>
#include <ChilliSource/UI/Button.h>

// Required Application Header
#include <App.h>

namespace ChilliJam
{
	void State_SpecialBegin::CreateSystems()
	{
		AudioPlayer = CreateSystem<CSAudio::CkAudioPlayer>();
	}

	void State_SpecialBegin::OnInit()
	{
		ButtonConnection = 0;

		Initialize_Camera();
		Initialize_GUI();
		Initialize_Button();

		auto resourcePool = CSCore::Application::Get()->GetResourcePool();
		AudioBank = resourcePool->LoadResource<CSAudio::CkBank>( CSCore::StorageLocation::k_package, "Audio/bank.ckb" );

		AudioPlayer->PlayEffect( AudioBank, "UI_Click" );
	}

	void State_SpecialBegin::OnUpdate( f32 in_deltaTime )
	{
		
	}

	void State_SpecialBegin::OnDestroy()
	{
		delete[] ButtonConnection;
	}

	// Initialize the orthographic camera
	// IN: N/A
	// OUT: N/A
	void State_SpecialBegin::Initialize_Camera()
	{
		// Create the camera entity and add the camera component
		CSCore::EntitySPtr Camera = CSCore::Entity::Create();
		{
			// Create the camera component
			CSRendering::RenderComponentFactory* rendercomponentfactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
			CSRendering::CameraComponentSPtr cameracomponent = rendercomponentfactory->CreateOrthographicCameraComponent( CSCore::Vector2( 1280, 720 ), 1.0f, 100.0f );
			Camera->AddComponent( cameracomponent );

			// Move camera for viewing scene
			Camera->GetTransform().SetLookAt( CSCore::Vector3::k_unitNegativeZ * (f32) 10, CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY );
		}
		// Add the camera to the scene
		GetScene()->Add( Camera );
	}

	// Load the base GUI menu in
	// IN: N/A
	// OUT: N/A
	void State_SpecialBegin::Initialize_GUI()
	{
		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();
		auto widgetfactory = CSCore::Application::Get()->GetWidgetFactory();

		// Load ui widget
		auto templatewidget = resourcepool->LoadResource<CSUI::WidgetTemplate>( CSCore::StorageLocation::k_package, "UI/SpecialBegin.csui" );

		UI = widgetfactory->Create( templatewidget );
		GetUICanvas()->AddWidget( UI );
	}

	// Initialize the GUI buttons & add events to them
	// IN: N/A
	// OUT: N/A
	void State_SpecialBegin::Initialize_Button()
	{
		unsigned int button = 0;
		ButtonConnection = new CSCore::EventConnectionUPtr[BUTTONS];

		ButtonConnection[button++] = UI->GetWidget( "Panel" )->GetWidget( "Panel_Adjust_Money" )->GetWidget( "Button_Plus" )->GetReleasedInsideEvent().OpenConnection(
			[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
			{
				State_SpecialBegin* state = (State_SpecialBegin*) CSCore::Application::Get()->GetStateManager()->GetActiveState().get();
				state->PlaySound( "UI_Click" );
			}
		);
		ButtonConnection[button++] = UI->GetWidget( "Panel" )->GetWidget( "Panel_Adjust_Money" )->GetWidget( "Button_Minus" )->GetReleasedInsideEvent().OpenConnection(
			[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
			{
				State_SpecialBegin* state = (State_SpecialBegin*) CSCore::Application::Get()->GetStateManager()->GetActiveState().get();
				state->PlaySound( "UI_Click" );
			}
		);
		ButtonConnection[button++] = UI->GetWidget( "Panel" )->GetWidget( "Panel_Adjust_Juice" )->GetWidget( "Button_Plus" )->GetReleasedInsideEvent().OpenConnection(
			[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
			{
				State_SpecialBegin* state = (State_SpecialBegin*) CSCore::Application::Get()->GetStateManager()->GetActiveState().get();
				state->PlaySound( "UI_Click" );
			}
		);
		ButtonConnection[button++] = UI->GetWidget( "Panel" )->GetWidget( "Panel_Adjust_Juice" )->GetWidget( "Button_Minus" )->GetReleasedInsideEvent().OpenConnection(
			[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
			{
				State_SpecialBegin* state = (State_SpecialBegin*) CSCore::Application::Get()->GetStateManager()->GetActiveState().get();
				state->PlaySound( "UI_Click" );
			}
		);
	}

	// Add or remove money to/from the mix
	// IN: (amount) The amount to add/remove
	// OUT: N/A
	void State_SpecialBegin::AddMoney( int amount )
	{
		Resource_Money -= amount;
		Affect_Taste += amount * AFFECT_MONEY_TASTE;
		Affect_Juice += amount * AFFECT_MONEY_JUICE;
	}

	// Add or remove juice to/from the mix
	// IN: (amount) The amount to add/remove
	// OUT: N/A
	void State_SpecialBegin::AddJuice( int amount )
	{
		Resource_Juice -= amount;
		Affect_Taste += amount * AFFECT_JUICE_TASTE;
		Affect_Juice += amount * AFFECT_JUICE_JUICE;
	}

	// Move on to the next state in the game, if the requirements of this one have been met
	// NOTE: For daybegin, the player must have selected two recipes
	// IN: N/A
	// OUT: N/A
	void State_SpecialBegin::Continue()
	{
		
	}

	// Play a sound effect using the audioplayer of this state
	// IN: (string) The event bank sound to play
	// OUT: N/A
	void State_SpecialBegin::PlaySound( string name )
	{
		AudioPlayer->PlayEffect( AudioBank, "UI_Click" );
	}
}