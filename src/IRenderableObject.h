#pragma once
#include "IRenderEngine.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class IRenderableObject
	{
	public:
		virtual void render()=0;
	};
}