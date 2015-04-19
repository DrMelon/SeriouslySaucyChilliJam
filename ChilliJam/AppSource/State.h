//
//  State.h
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

#ifndef _CSEMPTYTEMPLATE_STATE_H_
#define _CSEMPTYTEMPLATE_STATE_H_

// Required Engine Header
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Math.h>

// Required C++ Header
#include <vector>

// Namespaces
using std::vector;

// Defines
#define LIGHT_MIN 0.4f
#define LIGHT_CHANGE 0.1f

namespace ChilliJam
{
	typedef struct
	{
		CSCore::EntitySPtr Sprite;
		long long int Affected_Time;
		long long int Sound_Time;
		CSCore::Vector2 Direction;
		char Face[2];
	} PersonStruct;

	typedef struct
	{
		vector<CSCore::EntitySPtr> Sprite;
		long long int Affected_Time;
		CSCore::Vector2 Direction;
	} EffectStruct;

	class State : public CSCore::State
	{
		public:
			void CreateSystems() override;
			void OnInit() override;
			void OnUpdate( f32 in_deltaTime ) override;
			void OnDestroy() override;

		private:
			// The menu UI widget
			CSUI::WidgetSPtr UI_HUD;

			// Array of people moving around
			vector<PersonStruct> Person;
			vector<EffectStruct> Effect;

			// The number of people currently affected, and those that must be affected for the state to end
			float CurrentTime;
			unsigned int Affected_Current;
			unsigned int Affected_Target;

			// Audio
			CSAudio::CkAudioPlayer* AudioPlayer;
			CSAudio::CkBankCSPtr AudioBank;
	};
}

#endif
