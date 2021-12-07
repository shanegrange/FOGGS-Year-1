#include "Pacman.h"
#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.15f), _cPacmanFrameTime(200)
{
	_pacman = new Player();
	_pacman->currentFrameTime = 0;
	_pacmanFrame = 0;
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;
	_pacmanDirection = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->_pacmanTexture;
	delete _pacman->_pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->_pacmanTexture = new Texture2D();
	_pacman->_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacman->_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacman->_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);


	// Set Menu Paramters

	_menuBackground = new Texture2D();

	_menuBackground->Load("Textures/Transparency.png", false);

	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (!_started)
	{
		//Check for start
		if (keyboardState->IsKeyDown(Input::Keys::RETURN))
		{
			_started = true;
		}
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_paused)
		{
			Input(elapsedTime, keyboardState);
			UpdatePacman(elapsedTime);
			UpdateMunchie(elapsedTime);
			CheckViewportCollision();
		}
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacmanFrame++;

		if (_pacmanFrame >= 2)
			_pacmanFrame = 0;

		_pacmanCurrentFrameTime = 0;
	}
	_pacmanCurrentFrameTime += elapsedTime;
	_pacman->_pacmanSourceRect->Y = _pacman->_pacmanSourceRect->Height * _pacmanDirection;
	_pacman->_pacmanSourceRect->X = _pacman->_pacmanSourceRect->Width * _pacmanFrame;
}

void Pacman::UpdateMunchie(int elapsedTime)
{

}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	if (!_paused)
	{
		_frameCount++;

		// Pacman Change Direction
		if (state->IsKeyDown(Input::Keys::D))
		{
			_pacman->_pacmanPosition->X += _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 0;
		}

		else if (state->IsKeyDown(Input::Keys::A))
		{
			_pacman->_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 2;
		}

		else if (state->IsKeyDown(Input::Keys::W))
		{
			_pacman->_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 3;
		}

		else if (state->IsKeyDown(Input::Keys::S))
		{
			_pacman->_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
			_pacmanDirection = 1;
		}
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}

	if (state->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;
}

void Pacman::CheckViewportCollision()
{
	// Checks if Pacman is trying to disappear 
	if (_pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width > 1024)  //1024 is game width 
	{
		//Pacman hit right wall - reset his position 
		_pacman->_pacmanPosition->X = 30 - _pacman->_pacmanSourceRect->Width;
	}


	// Checks if Pacman is trying to disappear 
	if (_pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width < 30)  //0 is game width 
	{
		//Pacman hit left wall - reset his position 
		_pacman->_pacmanPosition->X = 1024 - _pacman->_pacmanSourceRect->Width;
	}

	// Checks if Pacman is trying to disappear 
	if (_pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Width > 768)  //768 is game width 
	{
		//Pacman hit right wall - reset his position 
		_pacman->_pacmanPosition->Y = 30 - _pacman->_pacmanSourceRect->Width;
	}


	// Checks if Pacman is trying to disappear 
	if (_pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Width < 30)  //0 is game width 
	{
		//Pacman hit left wall - reset his position 
		_pacman->_pacmanPosition->Y = 768 - _pacman->_pacmanSourceRect->Width;
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->_pacmanPosition->X << " Y: " << _pacman->_pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacman->_pacmanTexture, _pacman->_pacmanPosition, _pacman->_pacmanSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	if (_paused)
	{

		std::stringstream menuStream; menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);

		//SpriteBatch::EndDraw(); // Ends Drawing
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}