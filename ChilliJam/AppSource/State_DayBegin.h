//
//  State_DayBegin.h
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

#ifndef CHILLIJAM_STATE_DAYBEGIN_H
#define CHILLIJAM_STATE_DAYBEGIN_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>

#include <vector>

using std::vector;
using std::string;

namespace ChilliJam
{
	class State_DayBegin : public CSCore::State
	{
		public:
			void CreateSystems() override;
			void OnInit() override;
			void OnUpdate( f32 in_deltaTime ) override;
			void OnDestroy() override;

			// Add a recipe to the selected, if two are already selected then deselect the oldest one
			// NOTE: Must also deselect toggle button for deselecting
			// IN: (recipe) The name of the new recipe to use
			// OUT: N/A
			void AddRecipe( string recipe );

			// Remove a recipe from the selection, must already be selected
			// IN: (recipe) The name of the old recipe to remove
			// OUT: N/A
			void RemoveRecipe( string recipe );

			// Move on to the next state in the game, if the requirements of this one have been met
			// NOTE: For daybegin, the player must have selected two recipes
			// IN: N/A
			// OUT: N/A
			void Continue();
		private:
			// Initialize the orthographic camera
			// IN: N/A
			// OUT: N/A
			void Initialize_Camera();

			// Load the base GUI menu in
			// IN: N/A
			// OUT: N/A
			void Initialize_GUI();

			// Populate the GUI with recipes loaded from the main application
			// IN: N/A
			// OUT: N/A
			void Initialize_Recipe();

			// The menu orthographic camera
			CSCore::EntitySPtr Camera;

			// The menu UI widget
			CSUI::WidgetSPtr UI;
			CSUI::WidgetSPtr* Recipe;
			vector<CSUI::WidgetSPtr> Ingredient;

			// Array of button events
			CSCore::EventConnectionUPtr* ButtonConnection;

			// Array of selected recipes for beginning the game with
			unsigned int LastStartRecipe;
			string StartRecipe[2];
	};
}

#endif // CHILLIJAM_STATE_DAYBEGIN_H