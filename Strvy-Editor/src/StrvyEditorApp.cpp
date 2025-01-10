#include "strvy.h"
#include <strvy/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace strvy {

	class StrvyEditor : public Application
	{
	public:
		StrvyEditor()
			: Application("strvy editor")
		{
			//pushLayer(new ExampleLayer());
			pushLayer(new EditorLayer());
		}

		~StrvyEditor()
		{

		}
	};

	Application* createApplication()
	{
		return new StrvyEditor();
	}
}