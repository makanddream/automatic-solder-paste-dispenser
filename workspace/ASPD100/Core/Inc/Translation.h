/*
 * Translation.h
 *
 *  Created on: 28 Sept. 2020
 *      Author: Alan
 */

#ifndef TRANSLATION_H_
#define TRANSLATION_H_
#include "stdint.h"
extern const uint8_t USER_FONT_12x16[];
extern const uint8_t USER_FONT_6x8[];
/*
 * When SettingsShortNameType is SHORT_NAME_SINGLE_LINE
 * use SettingsShortNames as SettingsShortNames[16][1].. second column undefined
 */
extern const char *SettingsShortNames[28][2];
extern const char SettingsDescriptions[28][50];
extern const char *RootMenuEntries[2];

extern const char *SettingsResetWarning;
extern const char *UVLOWarningString;
extern const char *SleepingSimpleString;
extern const char *SleepingAdvancedString;
extern const char *WarningSimpleString;
extern const char *WarningAdvancedString;
extern const char *IdleTipString;
extern const char *IdleSetString;
extern const char *OffString;
extern const char *ResetOKMessage;
extern const char *YourGainMessage;
extern const char *SettingsResetMessage;
extern const char *SettingTrueChar;
extern const char *SettingFalseChar;
extern const char *SettingRightChar;
extern const char *SettingLeftChar;
extern const char *SettingAutoChar;
extern const char *SettingStartSolderingChar;
extern const char *SettingStartSleepChar;
extern const char *SettingStartSleepOffChar;
extern const char *SettingStartNoneChar;
extern const char *SettingFastChar;
extern const char *SettingSlowChar;
extern const char *TipModelStrings[];
extern const char *DebugMenu[];

extern const char *SymbolPlus;
extern const char *SymbolPrc;
extern const char *SymbolMinus;
extern const char *SymbolSpace;
extern const char *SymbolDot;
extern const char *SymbolDegC;
extern const char *SymbolMinutes;
extern const char *SymbolSeconds;
extern const char *SymbolWatts;
extern const char *SymbolVolts;
extern const char *SymbolDC;
extern const char *SymbolCellCount;
extern const char *SymbolVersionNumber;

extern const char* ImperialCode[12];
extern const char* MetricCode[12];

extern const char *Mode1;
extern const char *Mode2;
extern const char *FastString;
extern const char *SlowString;
extern const char *UPString;
extern const char *DOWNString;
extern const char *UserConfirmation;

extern const char *DebugMenu[];
#endif /* TRANSLATION_H_ */
