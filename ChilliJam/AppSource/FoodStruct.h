#pragma once
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Core/Tween.h>

////////
// @Author: J. Brown / DrMelon
// @Date: 18/04/2015
// @Purpose: For ChilliJam:
// 
// FoodStruct.h
// -----------------------
// MAKES A FOODS FOR VISUAL DISPLAYING
//
//

struct FoodStruct
{
	std::shared_ptr<CSRendering::Material> foodMaterial;
	std::shared_ptr<const CSRendering::TextureAtlas> foodAtlas;
	std::string foodType;
};

