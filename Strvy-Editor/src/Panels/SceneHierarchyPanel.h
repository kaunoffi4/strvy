#pragma once

#include "strvy/Core/Core.h"
#include "strvy/Core/Log.h"
#include "strvy/Scene/Scene.h"
#include "strvy/Scene/Entity.h"

namespace strvy {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void setContext(const Ref<Scene>& context);

		void onImGuiRender();
		Entity getSelectedEntity() const { return m_selectionContext; }
	private:
		void drawEntityNode(Entity entity);
		void drawComponents(Entity entity);

	private:
		Ref<Scene> m_context;
		Entity m_selectionContext;
	};

}