#pragma once
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Model.h>

////////
// @Author: J. Brown / DrMelon
// @Date: 17/04/2015
// @Purpose: For ChilliJam:
// 
// DayPlaying.h
// -----------------------
// The DayPlaying state is where the main gameplay of the game happens.
// Customers come and buy things, and you sell them bowls of chilli.
//

namespace ChilliJam
{
	class DayPlayingState : public CSCore::State
	{
		public:
			// Public Members
			// Resources


			// Public Methods
			void CreateSystems() override;
			void OnInit() override;
			void OnUpdate(f32 in_deltaTime) override;
			void OnDestroy() override;
	};
}

