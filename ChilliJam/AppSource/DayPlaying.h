#pragma once
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Core/Tween.h>
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
// Also in this class is a customer subclass/struct thing.
// The customers are stored in a list & will link to seans class
//
// And there is also a food item class, so you can tell a food what kind it is and it'll load the right one
// ye

namespace ChilliJam
{
	class Customer
	{
		public:
			// Members
			int alienType; //just the 4 colours of alien in kenneys thing, generates itself
			bool queueing;
			bool hasChilli;
			bool moving; // if the person is moving, wobble them up and down in the update etc
			bool facingCam;
			int posInQueue;
			CSCore::Vector3 shopPosition;
			CSCore::Vector3 targetPosition;
			CSRendering::SpriteComponentSPtr frontSprite;
			CSRendering::SpriteComponentSPtr backSprite;
			CSCore::EntitySPtr myEntity;
			// SeanCitizenClass 
			// Held Food Item Class -- just add sprite to entity? ye


			// Functions
			Customer(CSRendering::RenderComponentFactory* renderComponentFactory, std::shared_ptr<CSRendering::Material> alienMaterial, std::shared_ptr<const CSRendering::TextureAtlas> alienAtlas);
			//void MoveInQueue(); // This function sets where in the room they should move to in order to be part of the queue.
			void Update(float dt); // Called in update ofc, moving towards target pos
	};

	class DayPlayingState : public CSCore::State
	{
	public:
		// Public Members
		CSCore::EntitySPtr cameraEntity;
		float cameraYValue;
		CSCore::EaseOutBackTween<f32> cameraTween;

		// Resources

		// State stuff
		int currentDay;

		// Crowd Stuff
		int customersToday;
		std::vector<Customer*> customersList;

		// Public Methods
		void CreateSystems() override;
		void OnInit() override;
		void OnUpdate(f32 in_deltaTime) override;
		void OnDestroy() override;

	};
}
