//
//  App.cpp
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
#include <App.h>
#include <time.h>

// Required Engine Header
#include <ChilliSource/Rendering/Model/CSModelProvider.h>

#include <ChilliSource/Audio/CricketAudio.h>

// Required Application Header
#include <State.h>
#include <State_DayBegin.h>
#include <DayPlaying.h>

CSCore::Application* CreateApplication()
{
	srand(time(NULL));
	return new ChilliJam::App();
}

namespace ChilliJam
{
	void App::CreateSystems()
	{
		//Create systems here.
		CreateSystem<CSRendering::CSModelProvider>();
		CreateSystem<CSAudio::CricketAudioSystem>();
		CreateSystem<CSAudio::CkBankProvider>();
	}

	void App::OnInit()
	{
		// Initialize day counter
		Day = 0;

		// Create the chilli ingredients
		Ingredient = new IngredientStruct[INGREDIENTS];
		{
			IngredientStruct* ingredientinfo;

			ingredientinfo = new IngredientStruct;
			{
				ingredientinfo->ID = INGREDIENT_CHILLI;
				ingredientinfo->Name = "Chilli Pepper";
				ingredientinfo->ImagePath = "";
				ingredientinfo->Unlocked = true;
			}
			Ingredient[INGREDIENT_CHILLI] = *ingredientinfo;

			ingredientinfo = new IngredientStruct;
			{
				ingredientinfo->ID = INGREDIENT_BEEF;
				ingredientinfo->Name = "Beef Meat";
				ingredientinfo->ImagePath = "";
				ingredientinfo->Unlocked = true;
			}
			Ingredient[INGREDIENT_BEEF] = *ingredientinfo;

			ingredientinfo = new IngredientStruct;
			{
				ingredientinfo->ID = INGREDIENT_TACO;
				ingredientinfo->Name = "Taco Bread";
				ingredientinfo->ImagePath = "";
				ingredientinfo->Unlocked = true;
			}
			Ingredient[INGREDIENT_TACO] = *ingredientinfo;

			ingredientinfo = new IngredientStruct;
			{
				ingredientinfo->ID = INGREDIENT_JALAPENO;
				ingredientinfo->Name = "Jalapenos";
				ingredientinfo->ImagePath = "";
				ingredientinfo->Unlocked = true;
			}
			Ingredient[INGREDIENT_JALAPENO] = *ingredientinfo;

			ingredientinfo = new IngredientStruct;
			{
				ingredientinfo->ID = INGREDIENT_SEASONING;
				ingredientinfo->Name = "Seasoning";
				ingredientinfo->ImagePath = "";
				ingredientinfo->Unlocked = true;
			}
			Ingredient[INGREDIENT_SEASONING] = *ingredientinfo;

			ingredientinfo = new IngredientStruct;
			{
				ingredientinfo->ID = INGREDIENT_LOCKED;
				ingredientinfo->Name = "Locked";
				ingredientinfo->ImagePath = "";
				ingredientinfo->Unlocked = false;
			}
			Ingredient[INGREDIENT_LOCKED] = *ingredientinfo;
		}

		// Create the chilli recipes
		Recipe = new RecipeStruct[RECIPES];
		{
			unsigned int recipe = 0;
			RecipeStruct* recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Chilli";
				unsigned int ingredient = 2; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_CHILLI;
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_BEEF;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Chilli Taco";
				unsigned int ingredient = 3; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_CHILLI;
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_BEEF;
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_TACO;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Yum Taco";
				unsigned int ingredient = 1; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_TACO;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Yes Meal";
				unsigned int ingredient = 1; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_BEEF;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Saviour";
				unsigned int ingredient = 2; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_JALAPENO;
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_JALAPENO;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Ref1";
				unsigned int ingredient = 1; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_LOCKED;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Ref2";
				unsigned int ingredient = 1; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_LOCKED;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;

			recipeinfo = new RecipeStruct;
			{
				recipeinfo->Name = "Ref2";
				unsigned int ingredient = 1; // Max ingredients for this recipe
				recipeinfo->Ingredient = new unsigned int[ingredient];
				{
					ingredient = 0; // Reset incremental ingredient index
					recipeinfo->Ingredient[ingredient++] = INGREDIENT_LOCKED;
					recipeinfo->Ingredients = ingredient;
				}
			}
			Recipe[recipe++] = *recipeinfo;
		}
	}

	void App::PushInitialState()
	{
		GetStateManager()->Push( CSCore::StateSPtr( new State_DayBegin() ) );
	}

	void App::OnDestroy()
	{
		//destruction stuff here.
	}

	int App::GetRecipeIndex( string name )
	{
		for ( unsigned int recipe = 0; recipe < RECIPES; recipe++ )
		{
			if ( Recipe[recipe].Name == name )
			{
				return recipe;
			}
		}
		return -1;
	}
}