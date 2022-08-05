#include "ShapeGenerator.h"
#include "Teapot.h"

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
		shp.ShapesUpdate();
		if (Teapot::Input::IsKeyPressed(TEA_KEY_A))
		{
			std::cout << "A pressed" << std::endl;
		}
	}

private:
	Shapes::Shapes shp;
};

int main()
{
	Sandbox* s = new Sandbox();
	s->Run();
	s->OnUpdate();
	delete(s);
}