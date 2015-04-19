//
//  State_Score.cpp
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

#include <State_Score.h>

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

namespace ChilliJam
{
	
	void State_Score::CreateSystems()
	{
		//Add systems here.
	}

	void State_Score::OnInit()
	{
		//Create the camera component
		CSRendering::RenderComponentFactory* renderComponentFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
		CSRendering::CameraComponentSPtr cameraComponent = renderComponentFactory->CreatePerspectiveCameraComponent(CSCore::MathUtils::k_pi / 2.0f, 1.0f, 100.0f);

		//create the camera entity and add the camera component
		CSCore::EntitySPtr cameraEntity = CSCore::Entity::Create();
		cameraEntity->AddComponent(cameraComponent);

		//add the camera to the scene
		GetScene()->Add(cameraEntity);

		string text = "score";
			

		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();
		auto widgetfactory = CSCore::Application::Get()->GetWidgetFactory();

		// Load ui widget
		auto templatewidget = resourcepool->LoadResource<CSUI::WidgetTemplate>(CSCore::StorageLocation::k_package, "UI/scoreBoard.csui");
			
		scoreboard = widgetfactory->Create(templatewidget);
		
		GetUICanvas()->AddWidget(scoreboard);

	//	scoreboard->GetWidget("PlayButton")->GetWidget("PlayButtonLabel")->GetComponent<CSUI::TextComponent>()->SetText(text);
		//Test

		UI_moneyValue = 0;
		UI_juiceValue = 0;

	}

	void State_Score::OnUpdate(f32 in_deltaTime)
	{
		

		int money_Difference = scoreController.getTotalMoney() - scoreController.getPrevTotalMoney();
		if (UI_moneyValue != money_Difference)
		{
			UI_moneyValue += 1;
			std::string moneyText = std::to_string(UI_moneyValue);
			scoreboard->GetWidget("MoneyScore")->GetComponent<CSUI::TextComponent>()->SetText(moneyText);
		}
	int juice_Difference = scoreController.getTotalJuice() - scoreController.getPrevTotalJuice();
		if (UI_juiceValue != juice_Difference)
		{
			UI_juiceValue += 1;
			std::string juiceText = std::to_string(UI_juiceValue);
			scoreboard->GetWidget("AlienScore")->GetComponent<CSUI::TextComponent>()->SetText(juiceText);
		}

		std::string moneyGained = std::to_string(money_Difference);
		std::string juiceGained = std::to_string(juice_Difference);

		//scoreboard->GetWidget("MoneyUpdate")->GetComponent<CSUI::TextComponent>()->SetText("+" + moneyGained);
	//	scoreboard->GetWidget("AlienUpdate")->GetComponent<CSUI::TextComponent>()->SetText("+" + juiceGained);
		
	}

	void State_Score::OnDestroy()
	{
		//Destruction stuff here.
	}

	CSUI::WidgetSPtr CreateScoreScreen()
	{
		auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();
		auto resPool = CSCore::Application::Get()->GetResourcePool();

		auto scoreWidget = resPool->LoadResource<CSUI::WidgetTemplate>(CSCore::StorageLocation::k_package, "UI/scoreBoard.csui");

		CSUI::WidgetSPtr widget = widgetFactory->Create(scoreWidget);

		return widget;
	}

}

