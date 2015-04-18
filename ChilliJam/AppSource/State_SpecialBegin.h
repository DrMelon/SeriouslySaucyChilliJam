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

#ifndef CHILLIJAM_STATE_SPECIALBEGIN_H
#define CHILLIJAM_STATE_SPECIALBEGIN_H

// Required Engine Header
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>

// Required C++ Header
#include <vector>

// Defines
#define AFFECT_MONEY_TASTE 1
#define AFFECT_MONEY_JUICE 0

#define AFFECT_JUICE_TASTE -1
#define AFFECT_JUICE_JUICE 1

// Namespaces
using std::vector;
using std::string;

namespace ChilliJam
{
	class State_SpecialBegin : public CSCore::State
	{
		public:
			void CreateSystems() override;
			void OnInit() override;
			void OnUpdate( f32 in_deltaTime ) override;
			void OnDestroy() override;

			// Add or remove money to/from the mix
			// IN: (amount) The amount to add/remove
			// OUT: N/A
			void AddMoney( int amount );

			// Add or remove juice to/from the mix
			// IN: (amount) The amount to add/remove
			// OUT: N/A
			void AddJuice( int amount );

			// Move on to the next state in the game
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

			// The menu orthographic camera
			CSCore::EntitySPtr Camera;

			// The menu UI widget
			CSUI::WidgetSPtr UI;

			// Array of button events
			CSCore::EventConnectionUPtr* ButtonConnection;

			// GUI altered values of taste & juice for the chilli
			int Affect_Taste;
			int Affect_Juice;

			// Temporary resource changes during this state before finalizing
			int Resource_Money;
			int Resource_Juice;
	};
}

#endif // CHILLIJAM_STATE_SPECIALBEGIN_H