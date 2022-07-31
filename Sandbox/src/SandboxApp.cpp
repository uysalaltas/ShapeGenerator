#include "Teapot.h"
#include "ShapeGenerator.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox() 
	{
		//Contructor
	}
	~Sandbox()
	{
		//Destructor
	}

	void OnUpdate() override
	{
		//Loop
	}
};

int main()
{
	Shapes::Shapes shp;

	Sandbox* s = new Sandbox();
	s->Run();
	s->OnUpdate();
	delete(s);
}