#pragma once
#include "../Core.h"
#include "../Layer/Layer.h"
#include "../Layer/LLog.h"

namespace Silent
{
	class SILENT_API LDebug : public Layer
	{
	protected:
		LLog * log;
	public:
		LDebug() = default;
		~LDebug() = default;

		void ShowLog();

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
	};
}