#ifndef _FCCLIENT_CONSTS_H_
#define _FCCLIENT_CONSTS_H_

/*
 *  Used to define the application object's status
 */
enum e_AppState
{
  AppStateNone = 0,
  AppStateLoading,
  AppStateConnecting,
  AppStateLogin,
  AppStateCharacterSelection,
  AppStatePlaying,
  AppStateShuttingDown
};

/*
 *	Sub-states for each app state
 */

// e_AppState::Loading
enum e_AppStateLoading
{
  AppState_Loading_None,
	AppState_Loading_Text,
  AppState_Loading_Graphics,
  AppState_Loading_Sounds
};

// e_AppState::Connecting
enum e_AppStateConnecting
{
  AppState_Connecting_None,
  AppState_Connecting_Connecting,
  AppState_Connecting_Connected,
  AppState_Connecting_FetchingInfo,
  AppState_Connecting_Retry,
  AppState_Connecting_FinalFail
};

// e_AppState::Login
enum e_AppStateLogin
{
	AppState_Login_None,
	AppState_Login_LoginFailed,
	AppState_Login_LoginFailed_AccountInUse,
	AppState_Login_LoginSucceeded,
};

// e_AppState::CharacterSelection
enum e_AppStateCharacterSelection
{
	AppState_CharacterSelection_None,
	AppState_CharacterSelection_CharacterSelected,
	AppState_CharacterSelection_NewCharacter,
	AppState_CharacterSelection_DeleteCharacter,
	AppState_CharacterSelection_Failed
};

// e_AppState::ShuttingDown
enum e_AppStateShuttingDown
{
  AppState_ShuttingDown_None = 0,
  AppState_ShuttingDown_InProgress
};

#endif//_FCCLIENT_CONSTS_H_