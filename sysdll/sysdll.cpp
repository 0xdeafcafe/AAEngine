#include "stdafx.h"
#include "SysDllMain.h"
#include <stdio.h>			// Required for printf functions
#include "SystemFunctions.h"// Kernel/XAM imports + other related functions
#include <xinput2.h>
#include <ctime>
#include <time.h>
#include <xbdm.h>

// Easier way of working with memcpy and DmSetMemory (Thanks Lander ;))
#define SetMemory(Dest, Source, Size) { DmSetMemory(Dest, Size, Source, NULL); }
#define GetMemory(Source, Destination, Size) { DmGetMemory(Source, Size, Destination, NULL); }

// Debug Print
void DebugPrint(const char* format, ...)
{
	va_list pArgList;
    va_start( pArgList, format );

    CHAR buffer[ 1000 ];
    vsprintf( buffer, format, pArgList );

    printf("[%s]: %s\n", MODULE_NAME, buffer );
}

void AdjustMPGravity(ActionType type)
{
	// Get Current Gravity
	float currentMPGravity = 4.17;
	GetMemory((void*)0x82128CC8, (void*)&currentMPGravity, 4);

	// Update Gravity
	if (type == ActionType::UP)
		currentMPGravity += 0.02;
	else
		currentMPGravity -= 0.02;
	
	// Set New Gravity
	SetMemory((void*)0x82128CC8, (void*)&currentMPGravity, 4);

	// Tell user to restart the map
	DebugPrint("Gravity Modified, Now Reload the map: Back + Start + X");
}
void ResetMPGravity()
{
	// Reset Gravity
	float defaultMPGravity = 4.17;
	SetMemory((void*)0x82128CC8, (void*)&defaultMPGravity, 4);
}
void AdjustGameStateSpeed(ActionType type)
{
	// Get Current GameState Speed
	float currentGamestateSpeed = 30;
	GetMemory((void*)0x82128AD0, (void*)&currentGamestateSpeed, 4);

	// Update Value
	if (type == ActionType::UP)
		currentGamestateSpeed += 0.5;
	else
		currentGamestateSpeed -= 0.5;

	if (currentGamestateSpeed < 0)
		currentGamestateSpeed = 0;

	// Set new Gamestate SPeed
	SetMemory((void*)0x82128AD0, (void*)&currentGamestateSpeed, 4);
}
void ResetGameStateSpeed()
{
	// Reset GamestateSpeed
	float defaultMPGameStateSpeed = 30;
	SetMemory((void*)0x82128AD0, (void*)&defaultMPGameStateSpeed, 4);
}
void AdjustMPJumpHeight(ActionType type)
{
	// Get Current Jump Height
	float currentMPJumpHeight = 0.75;
	GetMemory((void*)0x82128AEC, (void*)&currentMPJumpHeight, 4);

	// Update Value
	if (type == ActionType::UP)
		currentMPJumpHeight += 0.02;
	else
		currentMPJumpHeight -= 0.02;

	if (currentMPJumpHeight < 0)
		currentMPJumpHeight = 0;

	// Set new Jump Height
	SetMemory((void*)0x82128AEC, (void*)&currentMPJumpHeight, 4);
}
void ResetMPJumpHeight()
{
	// Reset MP Jump Height
	float defaultMPJumpHeight = 0.75;
	SetMemory((void*)0x82128AEC, (void*)&defaultMPJumpHeight, 4);
}
void ReloadMap( void )
{
	byte reloadMapByte[1] = { 0x01 };
	SetMemory((void*)0x842663B7, (void*)&reloadMapByte, 1);
}
bool isPlayerRandomizationInEffect = false;
void RandomizePlayer( void )
{
	// Inialize Random 
	srand(time(NULL));
	byte tmp = 0x00;

	// Set Armour Primary Colour
	tmp = (byte)(rand() & 0x1C);
	SetMemory((void*)0x847FFB3B, (void*)&tmp, 1);

	// Set Armour Secondary Colour
	tmp = (byte)(rand() & 0x1C);
	SetMemory((void*)0x847FFB3F, (void*)&tmp, 1);

	// Set Armour Helmet
	tmp = (byte)(rand() & 0x47);
	if (tmp == 0x47)
		tmp = 0xff;
	SetMemory((void*)0x847FFB4E, (void*)&tmp, 1);

	// Set Armour Torso
	tmp = (byte)(rand() & 0x47);
	if (tmp == 0x47)
		tmp = 0xff;
	SetMemory((void*)0x847FFB4F, (void*)&tmp, 1);

	// Set Armour Shoulders
	tmp = (byte)(rand() & 0x47);
	if (tmp == 0x47)
		tmp = 0xff;
	SetMemory((void*)0x847FFB51, (void*)&tmp, 1);
	SetMemory((void*)0x847FFB52, (void*)&tmp, 1);

	// Set Armour ForeArms
	tmp = (byte)(rand() & 0x12);
	if (tmp == 0x12)
		tmp = 0xff;
	SetMemory((void*)0x847FFB53, (void*)&tmp, 1);

	// Set Armour Legs
	tmp = (byte)(rand() & 0x12);
	if (tmp == 0x12)
		tmp = 0xff;
	SetMemory((void*)0x847FFB54, (void*)&tmp, 1);

	// Set Armour Visor
	tmp = (byte)(rand() & 0x17);
	if (tmp == 0x17)
		tmp = 0xff;
	SetMemory((void*)0x847FFB43, (void*)&tmp, 1);
}
void InitialXexSetup( void )
{
	// Remove Microsoft's Fucking Breakpoints (tmp until lander can get the offset of the function)
	byte nopOpcode[4] = { 0x60, 0x00, 0x00, 0x00 };
	SetMemory((void*)0x81B0C8B0, (void*)&nopOpcode, sizeof(nopOpcode));

	// Remove Intro bik and Credits
	byte removeBIK[4] = { 0x2B, 0x09, 0x00, 0x01 };
	SetMemory((void*)0x82666110, (void*)&removeBIK, sizeof(removeBIK));

	// Unlock Everything
	byte unlockEverything[4] = { 0x39, 0x60, 0x00, 0x01 };
	SetMemory((void*)0x82548EF8, (void*)&unlockEverything, sizeof(unlockEverything));

	// Remove RSA Checks
	byte removeRSA[4] = { 0x38, 0x60, 0x00, 0x01 };
	SetMemory((void*)0x8233392C, (void*)&removeRSA, sizeof(removeRSA));

	// Unlock every Campaign Level
	byte unlockCampaignLevels[30];
	for (int i = 0; i < 30; i++)
		unlockCampaignLevels[i] = 0xff;
	SetMemory((void*)0xC274FAD0, (void*)&unlockCampaignLevels, sizeof(unlockCampaignLevels));

	// Remove Fall Damage
	float noFallDamage = 200;
	SetMemory((void*)0x82014864, (void*)&noFallDamage, sizeof(noFallDamage));
}

void GetControllerInput( void )
{
	DWORD dwResult;    
	for (DWORD i=0; i< 3; i++ )
	{
		XINPUT_STATE state;
		ZeroMemory( &state, sizeof(XINPUT_STATE) );

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState( i, &state );

		if( dwResult == ERROR_SUCCESS )
		{ 
			// Controller is connected
			std::vector<DWORD> Buttons;
			DWORD wButtons = state.Gamepad.wButtons;
			if (wButtons & XINPUT_GAMEPAD_A)
				Buttons.push_back(ControllerButtons::A);
			if (wButtons & XINPUT_GAMEPAD_B)
				Buttons.push_back(ControllerButtons::B);
			if (wButtons & XINPUT_GAMEPAD_X)
				Buttons.push_back(ControllerButtons::X);
			if (wButtons & XINPUT_GAMEPAD_Y)
				Buttons.push_back(ControllerButtons::Y);
			if (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
				Buttons.push_back(ControllerButtons::RIGHT_BUMPER);
			if (wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
				Buttons.push_back(ControllerButtons::LEFT_BUMPER);
			if (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
				Buttons.push_back(ControllerButtons::RIGHT_BUMPER);
			if (wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
				Buttons.push_back(ControllerButtons::LEFT_THUMBSTICK);
			if (wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
				Buttons.push_back(ControllerButtons::RIGHT_THUMBSTICK);
			if (wButtons & XINPUT_GAMEPAD_DPAD_UP)
				Buttons.push_back(ControllerButtons::DPAD_UP);
			if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				Buttons.push_back(ControllerButtons::DPAD_DOWN);
			if (wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
				Buttons.push_back(ControllerButtons::DPAD_LEFT);
			if (wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
				Buttons.push_back(ControllerButtons::DPAD_RIGHT);
			if (wButtons & XINPUT_GAMEPAD_START)
				Buttons.push_back(ControllerButtons::START);
			if (wButtons & XINPUT_GAMEPAD_BACK)
				Buttons.push_back(ControllerButtons::BACK);

			// Now that we've gotten all of the button inputs, let's see if this combination matches any
			// of the ones we defined
			for (int i = 0; i < ControllerInputActions.size(); i++)
			{
				InputResponse ir = ControllerInputActions[i];
				if (ir.ButtonPresses.size() != Buttons.size())
					continue;

				bool ValidInput = true;
				for (int j = 0; j < ir.ButtonPresses.size(); j++)
				{
					bool Found = false;
					for (int k = 0; k < Buttons.size(); k++)
					{
						if (ir.ButtonPresses[j] == Buttons[k])
						{
							Found = true;
							break;
						}
					}
					if (!Found)
					{
						ValidInput = false;
						break;
					}
				}

				// If we got a matching controller input
				if (ValidInput)
				{
					ir.Action();
					break;
				}
			}
		}
		else
		{
			// Controller is not connected 
		}
	}
}

// Add code you want to be executed during every update while in the game
void Update( void )
{
	// Check for Button Combos
	GetControllerInput();

	// Check for Player Randomization
	if (isPlayerRandomizationInEffect)
		RandomizePlayer();
}

void SetUpButtonPresses( void )
{
	// Setup MP Gravity Up Combo
	InputResponse mpGravityUp;
	mpGravityUp.Action = [] () { AdjustMPGravity(ActionType::UP); };
	mpGravityUp.ButtonPresses.push_back(ControllerButtons::BACK);
	mpGravityUp.ButtonPresses.push_back(ControllerButtons::DPAD_UP);
	// Setup MP Gravity Down Combo
	InputResponse mpGravityDown;
	mpGravityDown.Action = [] () { AdjustMPGravity(ActionType::DOWN); };
	mpGravityDown.ButtonPresses.push_back(ControllerButtons::BACK);
	mpGravityDown.ButtonPresses.push_back(ControllerButtons::DPAD_DOWN);
	// Setup Reset MP Gravity Combo
	InputResponse mpGravityReset;
	mpGravityReset.Action = [] () { ResetMPGravity(); };
	mpGravityReset.ButtonPresses.push_back(ControllerButtons::BACK);
	mpGravityReset.ButtonPresses.push_back(ControllerButtons::RIGHT_THUMBSTICK);

	// Setup Gamestate Speed Up Combo
	InputResponse gamestateSpeedUp;
	gamestateSpeedUp.Action = [] () { AdjustGameStateSpeed(ActionType::UP); };
	gamestateSpeedUp.ButtonPresses.push_back(ControllerButtons::X);
	gamestateSpeedUp.ButtonPresses.push_back(ControllerButtons::DPAD_RIGHT);
	// Setup Gamestate Speed Down Combo
	InputResponse gamestateSpeedDown;
	gamestateSpeedDown.Action = [] () { AdjustGameStateSpeed(ActionType::DOWN); };
	gamestateSpeedDown.ButtonPresses.push_back(ControllerButtons::X);
	gamestateSpeedDown.ButtonPresses.push_back(ControllerButtons::DPAD_LEFT);
	// Setup Reset Gamestate Speed Combo
	InputResponse gamestateSpeedReset;
	gamestateSpeedReset.Action = [] () { ResetGameStateSpeed(); };
	gamestateSpeedReset.ButtonPresses.push_back(ControllerButtons::X);
	gamestateSpeedReset.ButtonPresses.push_back(ControllerButtons::RIGHT_THUMBSTICK);

	// Setup MP JumpHeight Up Combo
	InputResponse mpJumpHeightUp;
	mpJumpHeightUp.Action = [] () { AdjustMPJumpHeight(ActionType::UP); };
	mpJumpHeightUp.ButtonPresses.push_back(ControllerButtons::X);
	mpJumpHeightUp.ButtonPresses.push_back(ControllerButtons::DPAD_UP);
	// Setup MP JumpHeight Down Combo
	InputResponse mpJumpHeightDown;
	mpJumpHeightDown.Action = [] () { AdjustMPJumpHeight(ActionType::DOWN); };
	mpJumpHeightDown.ButtonPresses.push_back(ControllerButtons::X);
	mpJumpHeightDown.ButtonPresses.push_back(ControllerButtons::DPAD_DOWN);
	// Setup Reset MP JumpHeight Combo
	InputResponse mpJumpHeightReset;
	mpJumpHeightReset.Action = [] () { ResetMPJumpHeight(); };
	mpJumpHeightReset.ButtonPresses.push_back(ControllerButtons::X);
	mpJumpHeightReset.ButtonPresses.push_back(ControllerButtons::LEFT_THUMBSTICK);

	// Setup Reload Map Combo
	InputResponse reloadMap;
	reloadMap.Action = [] () { ReloadMap(); };
	reloadMap.ButtonPresses.push_back(ControllerButtons::BACK);
	reloadMap.ButtonPresses.push_back(ControllerButtons::START);
	reloadMap.ButtonPresses.push_back(ControllerButtons::X);

	// Randomize Player Combo
	InputResponse playerRandomization;
	playerRandomization.Action = [] () 
	{ 
		// Update boolean
		isPlayerRandomizationInEffect ? 
			isPlayerRandomizationInEffect = false : isPlayerRandomizationInEffect = true;
	};
	playerRandomization.ButtonPresses.push_back(ControllerButtons::LEFT_THUMBSTICK);
	playerRandomization.ButtonPresses.push_back(ControllerButtons::RIGHT_THUMBSTICK);
	
	// Add to the detection
	ControllerInputActions.push_back(mpGravityUp);
	ControllerInputActions.push_back(mpGravityDown);
	ControllerInputActions.push_back(mpGravityReset);
	ControllerInputActions.push_back(gamestateSpeedUp);
	ControllerInputActions.push_back(gamestateSpeedDown);
	ControllerInputActions.push_back(gamestateSpeedReset);
	ControllerInputActions.push_back(mpJumpHeightUp);
	ControllerInputActions.push_back(mpJumpHeightDown);
	ControllerInputActions.push_back(mpJumpHeightReset);
	ControllerInputActions.push_back(reloadMap);
	ControllerInputActions.push_back(playerRandomization);
}

void GameCheck( void )
{
	SetUpButtonPresses();

	// Set up a label so we can jump back to this code if we leave the title id
Start:
	while (XamGetCurrentTitleId() != GAME_TITLE_ID) { Sleep(0); }
	DebugPrint("Game has been launched");

	// Poke initial bytes into the xex in Memory
	InitialXexSetup();

	// We have entered the game, do our update code
	while (XamGetCurrentTitleId() == GAME_TITLE_ID) { Update(); }

	// The loop broke, which means they aren't on the game anymore
	DebugPrint("Game has been exited");
	goto Start;
}

// Enter SystemDllMain.h and change the game title ID before making any other edits!
DWORD Initialize( void )
{
	//if (CreateSymbolicLink("hdd1:", CONTENT_PARTITION_PATH, true) != S_OK)
	//	return S_FALSE;

	//Create our new thread to wait on halo for. 
	DWORD threadId;
	ExCreateThread(&ModuleHandle, 0, &threadId, (VOID*)GameCheck, (LPTHREAD_START_ROUTINE)GameCheck, NULL, 0x02);

	return S_OK;
}

BOOL APIENTRY DllMain(HANDLE hInstDLL, DWORD reason, LPVOID lpReserved)
{
	ModuleHandle = hInstDLL;
	switch(reason)
	{
		case DLL_PROCESS_ATTACH:
			if(Initialize() != S_OK)
				return false;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return true;
}