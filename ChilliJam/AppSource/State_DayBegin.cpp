//
//  State_DayBegin.cpp
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
#include <State_DayBegin.h>

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
#include <ChilliSource/UI/ProgressBar.h>

// Required Application Header
#include <App.h>
#include <DayPlaying.h>
#include <State_SpecialBegin.h>

namespace ChilliJam
{
	void State_DayBegin::CreateSystems()
	{
		AudioPlayer = CreateSystem<CSAudio::CkAudioPlayer>();
	}

	void State_DayBegin::OnInit()
	{
		Recipe = 0;
		ButtonConnection = 0;
		LastStartRecipe = 0;

		StartRecipe[0] = "";
		StartRecipe[1] = "";

		// Proceed to next day
		App* application = (App*) CSCore::Application::Get();
		{
			application->AddDay();
		}

		Initialize_Camera();
		Initialize_GUI();
		Initialize_Recipe();

		auto resourcePool = CSCore::Application::Get()->GetResourcePool();
		AudioBank = resourcePool->LoadResource<CSAudio::CkBank>( CSCore::StorageLocation::k_package, "Audio/bank.ckb" );
		// Begin playing music.
		AudioPlayer->PlayMusic(CSCore::StorageLocation::k_package, "Audio/ChilliTime.ogg");
	}

	void State_DayBegin::OnUpdate( f32 in_deltaTime )
	{
		
	}

	void State_DayBegin::OnDestroy()
	{
		delete[] Recipe;
		Recipe = 0;

		Ingredient.clear();

		delete[] ButtonConnection;
	}

	// Initialize the orthographic camera
	// IN: N/A
	// OUT: N/A
	void State_DayBegin::Initialize_Camera()
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
	void State_DayBegin::Initialize_GUI()
	{
		ButtonConnection = new CSCore::EventConnectionUPtr[RECIPES + 1];

		App* application = (App*) CSCore::Application::Get();

		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();
		auto widgetfactory = CSCore::Application::Get()->GetWidgetFactory();

		// Load ui widget
		auto templatewidget = resourcepool->LoadResource<CSUI::WidgetTemplate>( CSCore::StorageLocation::k_package, "UI/DayBegin.csui" );

		UI = widgetfactory->Create( templatewidget );
		GetUICanvas()->AddWidget( UI );

		ButtonConnection[0] = UI->GetWidget( "Panel" )->GetWidget( "Button_Continue" )->GetReleasedInsideEvent().OpenConnection(
			[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
			{
				State_DayBegin* state = (State_DayBegin*) CSCore::Application::Get()->GetStateManager()->GetActiveState().get();
				state->Continue();
			}
		);

		// Load the HUD ui widget (THIS APPEARS IN EVERY STATE BECAUSE I'M A BAD PERSON -M)
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
		std::sprintf(dollabuffer, "$%.2f", amt);
		string dolla(dollabuffer);
		UI_HUD->GetWidget("Dolla")->GetComponent<CSUI::TextComponent>()->SetText(dolla);
	}

	// Populate the GUI with recipes loaded from the main application
	// IN: N/A
	// OUT: N/A
	void State_DayBegin::Initialize_Recipe()
	{
		Recipe = new CSUI::WidgetSPtr[RECIPES];

		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();
		auto widgetfactory = CSCore::Application::Get()->GetWidgetFactory();

		App* application = (App*) CSCore::Application::Get();
		unsigned int currentrecipe = 0;
		for ( unsigned int recipe = 0; recipe < RECIPES; recipe++ )
		{
			RecipeStruct recipeinfo = application->GetRecipes()[recipe];
			string name = recipeinfo.Name;

			// Check that all required ingredients have been unlocked
			bool unlocked = true;
			{
				for ( unsigned int ingredient = 0; ingredient < recipeinfo.Ingredients; ingredient++ )
				{
					if ( !application->GetIngredients()[recipeinfo.Ingredient[ingredient]].Unlocked )
					{
						unlocked = false;
						break;
					}
				}
			}
			if ( !unlocked )
			{
				continue;
			}

			// Load ui widget
			auto templatewidget = resourcepool->LoadResource<CSUI::WidgetTemplate>( CSCore::StorageLocation::k_package, "UI/Recipe.csui" );

			// Add a new widget to the list for each recipe
			Recipe[recipe] = widgetfactory->Create( templatewidget );
			{
				Recipe[recipe]->GetWidget( "Panel" )->GetWidget( "Label_Title" )->GetComponent<CSUI::TextComponent>()->SetText( name );
				Recipe[recipe]->GetWidget( "Panel" )->SetRelativePosition( CSCore::Vector2( 0, ( ( (float) currentrecipe ) - 3.5f ) / -( RECIPES + 4 ) ) );

				// Add recipe toggle button logic
				ButtonConnection[recipe + 1] = Recipe[recipe]->GetWidget( "Panel" )->GetWidget( "Checkbox" )->GetReleasedInsideEvent().OpenConnection(
					[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
					{
						State_DayBegin* state = (State_DayBegin*) CSCore::Application::Get()->GetStateManager()->GetActiveState().get();
						// Has just been toggled on
						string name = in_widget->GetParent()->GetWidget( "Label_Title" )->GetComponent<CSUI::TextComponent>()->GetText();
						if ( in_widget->GetComponent<CSUI::ToggleHighlightComponent>()->IsToggledOn() )
						{
							// Add the name of the recipe to the currently selected
							state->AddRecipe( name );
						}
						// Has just been toggled off
						else
						{
							// Remove the name of the recipe to the currently selected
							state->RemoveRecipe( name );
						}
						state->PlaySound( "UI_Click" );
					}
				);

				// Add individual ingredients
				for ( unsigned int ingredient = 0; ingredient < recipeinfo.Ingredients; ingredient++ )
				{
					IngredientStruct ingredientinfo = application->GetIngredients()[recipeinfo.Ingredient[ingredient]];
					name = ingredientinfo.Name;

					// Load ui widget
					auto templatewidget = resourcepool->LoadResource<CSUI::WidgetTemplate>( CSCore::StorageLocation::k_package, "UI/Ingredient.csui" );

					// Add a new widget to the list for each recipe
					Ingredient.push_back( widgetfactory->Create( templatewidget ) );
					{
						Ingredient.back()->GetWidget( "Panel" )->GetWidget( "Label_Title" )->GetComponent<CSUI::TextComponent>()->SetText( name );
						Ingredient.back()->GetWidget( "Panel" )->SetRelativePosition( CSCore::Vector2( ( ( (float) ingredient ) - 0.75f ) / 3.4f, 0 ) );
					}
					Recipe[recipe]->GetWidget( "Panel" )->AddWidget( Ingredient.back() );
				}
			}
			UI->GetWidget( "Panel" )->AddWidget( Recipe[recipe] );

			currentrecipe++;
		}
	}

	// Add a recipe to the selected, if two are already selected then deselect the oldest one
	// NOTE: Must also deselect toggle button for deselecting
	// IN: (recipe) The name of the new recipe to use
	// OUT: N/A
	void State_DayBegin::AddRecipe( string recipe )
	{
		// If the recipe wasn't blank, deselect the UI widget
		if ( StartRecipe[LastStartRecipe] != "" )
		{
			// Find the recipe toggle box widget
			CSUI::WidgetSPtr panel = UI->GetWidget( "Panel" );
			vector<CSUI::WidgetSPtr> list = panel->GetWidgets();
			for ( unsigned int widget = 0; widget < list.size(); widget++ )
			{
				// Is a recipe
				if ( list[widget]->GetName() == "Recipe" )
				{
					// Is THE recipe
					if ( list[widget]->GetWidget( "Panel" )->GetWidget( "Label_Title" )->GetComponent<CSUI::TextComponent>()->GetText() == StartRecipe[LastStartRecipe] )
					{
						// Find the checkbox and disable it
						auto toggle = list[widget]->GetWidget( "Panel" )->GetWidget( "Checkbox" )->GetComponent<CSUI::ToggleHighlightComponent>();
						toggle->SetToggleOn( false );
						break;
					}
				}
			}
		}
		// Update to new start recipes
		StartRecipe[LastStartRecipe] = recipe;
		LastStartRecipe = !LastStartRecipe;

		// Update the progress bar
		UpdateRecipe();
	}

	// Remove a recipe from the selection, must already be selected
	// IN: (recipe) The name of the old recipe to remove
	// OUT: N/A
	void State_DayBegin::RemoveRecipe( string recipe )
	{
		for ( unsigned int index = 0; index < 2; index++ )
		{
			if ( StartRecipe[index] == recipe )
			{
				StartRecipe[index] = "";
				LastStartRecipe = index;
			}
		}

		// Update the progress bar
		UpdateRecipe();
	}

	// Update the progress bar
	// IN: N/A
	// OUT: N/A
	void State_DayBegin::UpdateRecipe()
	{
		unsigned int count = 0;
		for ( unsigned int index = 0; index < 2; index++ )
		{
			if ( StartRecipe[index] != "" )
			{
				count++;
			}
		}

		float progress = 0;
		string text = "0 of 2";
		CSCore::Colour colour( 1.0f, 0.5f, 0.1f, 1.0f );
		{
			if ( count > 1 )
			{
				progress = 1;
				text = "Continue";
				colour = CSCore::Colour( 0.9f, 0.9f, 0.9f, 1.0f );
			}
			else if ( count > 0 )
			{
				progress = 0.5f;
				text = "1 of 2";
			}
		}
		UI->GetWidget( "Panel" )->GetWidget( "Button_Continue" )->GetWidget( "Label_Button_Continue" )->GetComponent<CSUI::TextComponent>()->SetText( text );
		UI->GetWidget( "Panel" )->GetWidget( "Button_Continue" )->GetWidget( "Label_Button_Continue" )->GetComponent<CSUI::TextComponent>()->SetTextColour( colour );
		UI->GetWidget( "Panel" )->GetWidget( "Button_Continue" )->GetWidget( "Progress" )->GetComponent<CSUI::ProgressBarComponent>()->SetProgress( progress );
	}

	// Move on to the next state in the game, if the requirements of this one have been met
	// NOTE: For daybegin, the player must have selected two recipes
	// IN: N/A
	// OUT: N/A
	void State_DayBegin::Continue()
	{
		// User has chosen two recipes
		if ( ( StartRecipe[0] != "" ) && ( StartRecipe[1] != "" ) )
		{
			App* application = (App*) CSCore::Application::Get();

			// Add selected recipes to application for passing to the day state
			for ( unsigned int recipe = 0; recipe < 2; recipe++ )
			{
				application->SetDayRecipe( recipe, application->GetRecipeIndex( StartRecipe[recipe] ) );
			}

			// Later days can add special ingredients
			if ( application->GetDay() > 1 )
			{
				CSCore::Application::Get()->GetStateManager()->Change( (CSCore::StateSPtr) new State_SpecialBegin() );
			}
			// First day skip from recipes to playing
			else
			{
				CSCore::Application::Get()->GetStateManager()->Change( ( CSCore::StateSPtr ) new DayPlayingState() );
			}
		}
	}

	// Play a sound effect using the audioplayer of this state
	// IN: (string) The event bank sound to play
	// OUT: N/A
	void State_DayBegin::PlaySound( string name )
	{
		AudioPlayer->PlayEffect( AudioBank, "UI_Click" );
	}
}