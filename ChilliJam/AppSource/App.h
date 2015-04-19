//
//  App.h
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

#ifndef CHILLIJAM_APP_H
#define CHILLIJAM_APP_H

// Required Engine Header
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base.h>

// Required Application Header
#include <Recipe.h>

// Defines
#define RECIPES 8
// INGREDIENTS is defined as the last index of the INGREDIENT enum

// Enumerations
enum INGREDIENT
{
	INGREDIENT_CHILLI = 0,
	INGREDIENT_BEEF,
	INGREDIENT_TACO,
	INGREDIENT_JALAPENO,
	INGREDIENT_SEASONING,
	INGREDIENT_LOCKED,
	INGREDIENTS // Last in the enum
};

// Namespaces
using std::string;

namespace ChilliJam
{
	class App final : public CSCore::Application
	{
		public:
			void CreateSystems() override;
			void OnInit() override;
			void PushInitialState() override;
			void OnDestroy() override;

			int GetRecipeIndex( string name );

			inline RecipeStruct* GetRecipes() { return Recipe; };
			inline IngredientStruct* GetIngredients() { return Ingredient; };

			inline void SetDayRecipe( unsigned int recipe, unsigned int index ) { DayRecipe[recipe] = index; };
			inline unsigned int GetDayRecipe( unsigned int recipe ) { return DayRecipe[recipe]; };

			inline void AddDay() { Day++; };
			inline unsigned int GetDay() { return Day; };

			inline void AddDolla(float amt) { Dolla += amt; };
			inline float GetDolla() { return Dolla; };

			inline void AddJuice( int amount ) { Juice += amount; };
			inline int GetJuice() { return Juice; };

			inline void AddConsumers( int amount ) { Consumers += amount; };
			inline void SetConsumers( int amount ) { Consumers = amount; };
			inline int GetConsumers() { return Consumers; };

			inline void SetAffect( float affect ) { Affect = affect; };
			inline float GetAffect() { return Affect; };

			inline void SetPreviousDolla( int amount ) { PreviousDolla = amount; };
			inline int GetPreviousDolla() { return PreviousDolla; };

			inline void SetPreviousJuice( int amount ) { PreviousJuice = amount; };
			inline int GetPreviousJuice() { return PreviousJuice; };

		private:
			RecipeStruct* Recipe;
			IngredientStruct* Ingredient;

			unsigned int DayRecipe[2];

			unsigned int Day;
			float Dolla;
			int Juice;
			unsigned int Consumers;

			int PreviousDolla;
			int PreviousJuice;

			unsigned int Affect;
	};
}

#endif // CHILLIJAM_APP_H