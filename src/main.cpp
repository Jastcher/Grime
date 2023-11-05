#include "application.h"

static int inited = 0;
void* operator new(size_t size)
{
	inited++;
	auto memory = malloc(size);
	// std::cout << "Initialing memory: " << memory << " of size: " << size << std::endl;
	return memory;
}

static int deleted = 0;
void operator delete(void* memory)
{
	deleted++;
	// std::cout << "Deleting memory: " << memory << std::endl;
	free(memory);
}

int main()
{

	Application* app = new Application({ 1, 1, "Grime" });

	app->Run();

	delete app;

	std::cout << inited << " " << deleted << std::endl;
}
