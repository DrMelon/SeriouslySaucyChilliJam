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
#define RECIPES 4
// INGREDIENTS is defined as the last index of the INGREDIENT enum

// Enumerations
enum INGREDIENT
{
	INGREDIENT_CHILLI = 0,
	INGREDIENT_BEEF,
	INGREDIENT_TACO,
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

		private:
			RecipeStruct* Recipe;
			IngredientStruct* Ingredient;
	};
}

#endif // CHILLIJAM_APP_H