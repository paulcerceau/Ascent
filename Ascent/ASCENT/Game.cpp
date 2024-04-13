#include "Game.h"

#include "Timer.h"

#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "AudioComponent.h"

#include "Actor.h"
#include "FPSActor.h"
#include "FollowActor.h"
#include "ShipActor.h"

#include "Assets.h"
#include "Consts.h"

#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"


bool Game::initialize()
{
	const bool isWindowInit = window.initialize();
	const bool isRendererInit = renderer.initialize(window);
	const bool isAudioInit = audioSystem.initialize();
	const bool isInputInit = inputSystem.initialize();

	return isWindowInit && isRendererInit && isAudioInit && isInputInit; 
}

void Game::load()
{
	inputSystem.setMouseRelativeMode(true);

	#pragma region Loads
	//v =============================================================╗
	//v Load Shaders/Textures/Meshes                                 ║


	// TODO: create new textures

	// SHADERS ==============================
	Assets::loadShader("GameRes\\Shaders\\Sprite.vert", "GameRes\\Shaders\\Sprite.frag", "", "", "", "Sprite");
	Assets::loadShader("GameRes\\Shaders\\Phong.vert", "GameRes\\Shaders\\Phong.frag", "", "", "", "Phong");
	Assets::loadShader("GameRes\\Shaders\\BasicMesh.vert", "GameRes\\Shaders\\BasicMesh.frag", "", "", "", "BasicMesh");

	// TEXTURES =============================
	// -- Basic textures --
	Assets::loadTexture(renderer, "GameRes\\Textures\\Default.png", "Default");
	Assets::loadTexture(renderer, "GameRes\\Textures\\Cube.png", "Cube");
	Assets::loadTexture(renderer, "GameRes\\Textures\\Plane.png", "Plane");
	Assets::loadTexture(renderer, "GameRes\\Textures\\Sphere.png", "Sphere");
	Assets::loadTexture(renderer, "GameRes\\Textures\\RacingCar.png", "RacingCar");
	Assets::loadTexture(renderer, "GameRes\\Textures\\ShipCockpit.png", "ShipCockpit");

	// -- Game specific textures --

	// MESHES ===============================
	// -- Basic meshes --
	Assets::loadMesh(R"(GameRes\Meshes\BasicMeshes\Cube.gpmesh)", "Mesh_Cube");
	Assets::loadMesh(R"(GameRes\Meshes\BasicMeshes\Plane.gpmesh)", "Mesh_Plane");
	Assets::loadMesh(R"(GameRes\Meshes\BasicMeshes\Sphere.gpmesh)", "Mesh_Sphere");
	Assets::loadMesh(R"(GameRes\Meshes\RacingCar.gpmesh)", "Mesh_Car");

	// -- Game specific meshes --

	//^ Load Shaders/Textures/Meshes                                 ║
	//^ =============================================================╝
	#pragma endregion Loads
	#pragma region Place actors
	//v =============================================================╗
	//v Place actors                                                 ║

	fps = new FPSActor();
	follow = new FollowActor();
	ship = new ShipActor();

	Cube* a = new Cube();
	a->setPosition(Vector3(200.0f, 105.0f, 0.0f));
	a->setScale(100.0f);
	Quaternion q(Vector3::unitY, -Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi / 4.0f));
	a->setRotation(q);

	Sphere* b = new Sphere();
	b->setPosition(Vector3(200.0f, -75.0f, 0.0f));
	b->setScale(3.0f);

	// -- ROTATIONS TESTS --
	const Vector3 cubesOriginPosX{ 1000.0f, 0.0f, 0.0f };
	const Vector3 cubesOriginPosY{ 0.0f, 500.0f, 0.0f };
	Vector3 offset{ 0.0f, 0.0f, 0.0f };

	const Quaternion yRot{ Vector3::unitY, -Maths::piOver2 };
	const Quaternion zRot{ Vector3::unitZ, Maths::pi };
	const Quaternion cubesOriginRot = Quaternion::concatenate(yRot, zRot);
	
	Quaternion additionalRot{ Vector3::unitZ, 0.0f };
	const Quaternion zRotPiOverFour{ Vector3::unitZ, -Maths::pi / 4.0f };

	for (Uint8 i = 0; i < 8; i++)
	{
		for (Uint8 i = 0; i < 2; i++) {
			Cube* newCube = new Cube();

			if (i % 2 == 0) {
				newCube->setPosition(cubesOriginPosX + offset);
			}
			else {
				newCube->setPosition(cubesOriginPosY + offset);
			}
			newCube->setScale(100.0f);

			const Quaternion newCubeRotation = Quaternion::concatenate(cubesOriginRot, additionalRot);
			newCube->setRotation(newCubeRotation);
		}
		offset += Vector3(0.0, 0.0f, 100.0f);
		additionalRot = Quaternion::concatenate(additionalRot, zRotPiOverFour);
	}

	// -- SPRITES TESTS --
	Actor* hud = new Actor();
	hud->setScale(1.6f);
	SpriteComponent* hudSprite = new SpriteComponent(hud, Assets::getTexture("ShipCockpit"));

	//v Place floor / walls ==========================================
	// Setup floor ==========================
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Plane* p = new Plane();
			p->setPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// Left/right walls =====================
	q = Quaternion(Vector3::unitX, Maths::piOver2);
	for (int i = 0; i < 10; i++)
	{
		Plane* p = new Plane();
		p->setPosition(Vector3(start + i * size, start - size, 0.0f));
		p->setRotation(q);

		p = new Plane();
		p->setPosition(Vector3(start + i * size, -start + size, 0.0f));
		p->setRotation(q);
	}

	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::piOver2));

	// Forward/back walls ===================
	for (int i = 0; i < 10; i++)
	{
		Plane* p = new Plane();
		p->setPosition(Vector3(start - size, start + i * size, 0.0f));
		p->setRotation(q);

		p = new Plane();
		p->setPosition(Vector3(-start + size, start + i * size, 0.0f));
		p->setRotation(q);
	}
	//^ Place floor / walls ==========================================
	//v Sound actors =================================================

	//// Create spheres with audio components playing different sounds
	//Sphere* soundSphere = new Sphere();
	//soundSphere->setPosition(Vector3(500.0f, -75.0f, 0.0f));
	//soundSphere->setScale(1.0f);
	//AudioComponent* ac = new AudioComponent(soundSphere);
	//ac->playEvent("event:/FireLoop");

	//^ Sound actors =================================================

	//^ Place actors                                                 ║
	//^ =============================================================╝
	#pragma endregion Place actors

	// Setup lights
	renderer.setAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = renderer.getDirectionalLight();
	dir.direction = Vector3(0.0f, -0.707f, -0.707f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

	// Start music
	//musicEvent = audioSystem.playEvent("event:/Music");

	// Set camera mode to "Ship"
	changeCamera(3);
}

void Game::processInput()
{
	inputSystem.preUpdate();

	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		isRunning = inputSystem.processEvent(event);
	}

	inputSystem.update();
	const InputState& input = inputSystem.getInputState();

	//v Game states ==================================================
	if (state == GameState::Gameplay)
	{
		// Escape: quit game
		if (input.keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Released)
		{
			isRunning = false;
		}

		// Change camera mode
		if (input.keyboard.getKeyState(SDL_SCANCODE_1) == ButtonState::Pressed)
		{
			changeCamera(1);
		}
		else if (input.keyboard.getKeyState(SDL_SCANCODE_2) == ButtonState::Pressed)
		{
			changeCamera(2);
		}
		else if (input.keyboard.getKeyState(SDL_SCANCODE_3) == ButtonState::Pressed)
		{
			changeCamera(3);
		}

		// Actor input
		isUpdatingActors = true;
		for (const auto actor : actors)
		{
			actor->processInput(input);
		}
		isUpdatingActors = false;
	}

	//^ Game states ==================================================
}

void Game::update(float dt)
{
	// TODO: add collisions

	// Update audio
	audioSystem.update(dt);

	if (state == GameState::Gameplay)
	{
		// Update actors 
		isUpdatingActors = true;
		for (const auto actor : actors)
		{
			actor->update(dt);
		}
		isUpdatingActors = false;

		// Move pending actors to actors
		for (auto pendingActor : pendingActors)
		{
			pendingActor->computeWorldTransform();
			actors.emplace_back(pendingActor);
		}
		pendingActors.clear();

		// Delete dead actors
		vector<Actor*> deadActors;
		for (auto actor : actors)
		{
			if (actor->getState() == Actor::ActorState::Dead)
			{
				deadActors.emplace_back(actor);
			}
		}
		for (const auto deadActor : deadActors)
		{
			delete deadActor;
		}
	}
}

void Game::changeCamera(int mode)
{
	// Disable everything
	fps->setState(Actor::ActorState::Paused);
	fps->setVisible(false);
	follow->setState(Actor::ActorState::Paused);
	follow->setVisible(false);
	ship->setState(Actor::ActorState::Paused);

	// Enable the camera specified by the mode
	switch (mode)
	{
	case 1:
	default:
		fps->setState(Actor::ActorState::Active);
		fps->setVisible(true);
		break;
	case 2:
		follow->setState(Actor::ActorState::Active);
		follow->setVisible(true);
		break;
	case 3:
		ship->setState(Actor::ActorState::Active);
		break;
	}
}

#pragma region Engine
void Game::render()
{
	renderer.beginDraw();
	renderer.draw();
	renderer.endDraw();
}

void Game::loop()
{
	Timer timer;
	float dt = 0;
	while (isRunning)
	{
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
}

void Game::unload()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!actors.empty())
	{
		delete actors.back();
	}

	// Resources
	Assets::clear();
}

void Game::close()
{
	inputSystem.close();
	renderer.close();
	audioSystem.close();
	window.close();
	SDL_Quit();
}

void Game::setState(GameState stateP)
{
	state = stateP;
}

void Game::addActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::removeActor(Actor* actor)
{
	// Erase actor from the two vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors))
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors))
	{
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}

#pragma endregion
