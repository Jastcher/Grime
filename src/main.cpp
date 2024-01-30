#include "application.h"
#include "window.h"
#include <memory>

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
	{
		std::unique_ptr<Application> app = std::make_unique<Application>(Props { 1, 1, "Grime", nullptr });

		app->Run();
	}

	std::cout << inited << " " << deleted << std::endl; // 50 memories get freed after main() scope ?????????????? wtf

	glfwTerminate(); // cant be in app for some reason
}
