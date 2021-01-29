var def =
{
	"messages": [
		{
			"id": "SettingsCalibrationDone"
		},
		{
			"id": "SettingsCalibrationWarning"
		},
		{
			"id": "SettingsResetWarning"
		},
		{
			"id": "UVLOWarningString",
			"maxLen": 8
		},
		{
			"id": "UndervoltageString",
			"maxLen": 16
		},
		{
			"id": "InputVoltageString",
			"maxLen": 11,
			"note": "Preferably end with a space"
		},
		{
			"id": "WarningTipTempString",
			"maxLen": 12,
			"note": "Preferably end with a space"
		},
		{
			"id": "BadTipString",
			"maxLen": 8
		},
		{
			"id": "SleepingSimpleString",
			"maxLen": 4
		},
		{
			"id": "SleepingAdvancedString",
			"maxLen": 16
		},
		{
			"id": "WarningSimpleString",
			"maxLen": 4
		},
		{
			"id": "WarningAdvancedString",
			"maxLen": 16
		},
		{
			"id": "SleepingTipAdvancedString",
			"maxLen": 6
		},
		{
			"id": "IdleTipString",
			"lenSum":
			{
				"fields": ["IdleTipString", "IdleSetString"],
				"maxLen": 10
			}
		},
		{
			"id": "IdleSetString",
			"lenSum":
			{
				"fields": ["IdleTipString", "IdleSetString"],
				"maxLen": 10
			},
			"note": "Preferably start with a space"
		},
		{
			"id": "TipDisconnectedString",
			"maxLen": 16
		},
		{
			"id": "SolderingAdvancedPowerPrompt",
			"maxLen": null
		},
		{
			"id": "OffString",
			"maxLen": 3
		},
		{
			"id": "ResetOKMessage",
			"maxLen": 8
		},
		{
			"id": "YourGainMessage",
			"maxLen": 8,
			"default": "Your Gain"
		},
		{
			"id": "SettingsResetMessage",
			"maxLen": 16,
			"default": "Settings were\nreset!"
		},
		{
			"id": "Mode1",
			"maxLen": 8,
			"default": "Mode 1"
		},
		{
			"id": "Mode2",
			"maxLen": 8,
			"default": "Mode 2"
		},
		{
			"id": "FastString",
			"maxLen": 5,
			"default": "FAST"
		},
		{
			"id": "SlowString",
			"maxLen": 5,
			"default": "SLOW"
		},
		{
			"id": "UPString",
			"maxLen": 5,
			"default": "UP"
		},
		{
			"id": "DOWNString",
			"maxLen": 5,
			"default": "DOWN"
		},
		{
			"id": "UserConfirmation",
			"maxLen": 8
		}		
	],
	"characters": [
		{
			"id": "SettingRightChar",
			"len": 1
		},
		{
			"id": "SettingLeftChar",
			"len": 1
		},
		{
			"id": "SettingAutoChar",
			"len": 1
		},
		{
			"id": "SettingFastChar",
			"len": 1
		},
		{
			"id": "SettingSlowChar",
			"len": 1
		},
		{
			"id": "SettingStartSolderingChar",
			"len": 1
		},
		{
			"id": "SettingStartSleepChar",
			"len": 1
		},
		{
			"id": "SettingStartSleepOffChar",
			"len": 1
		},
		{
			"id": "SettingStartNoneChar",
			"len": 1
		}
	],
	"imperialCode": [
		{
			"id": "01005",
			"maxLen": 5,
			"default": "01005"
		},
		{
			"id": "0201",
			"maxLen": 4,
			"default": "0201"
		},
		{
			"id": "0402",
			"maxLen": 4,
			"default": "0402"
		},
		{
			"id": "0603",
			"maxLen": 4,
			"default": "0603"
		},
		{
			"id": "0805",
			"maxLen": 4,
			"default": "0805"
		},
		{
			"id": "1008",
			"maxLen": 4,
			"default": "1008"
		},
		{
			"id": "1206",
			"maxLen": 4,
			"default": "1206"
		},
		{
			"id": "1210",
			"maxLen": 4,
			"default": "1210"
		},
		{
			"id": "1806",
			"maxLen": 4,
			"default": "1806"
		},
		{
			"id": "1812",
			"maxLen": 4,
			"default": "1812"
		},
		{
			"id": "2010",
			"maxLen": 4,
			"default": "2010"
		},
		{
			"id": "2512",
			"maxLen": 4,
			"default": "2512"
		}
	],
	"metricCode": [
		{
			"id": "0402",
			"maxLen": 4,
			"default": "0402"
		},
		{
			"id": "0603",
			"maxLen": 4,
			"default": "0603"
		},
		{
			"id": "1005",
			"maxLen": 4,
			"default": "1005"
		},
		{
			"id": "1608",
			"maxLen": 4,
			"default": "1608"
		},
		{
			"id": "2012",
			"maxLen": 4,
			"default": "2012"
		},
		{
			"id": "2520",
			"maxLen": 4,
			"default": "2520"
		},
		{
			"id": "3216",
			"maxLen": 4,
			"default": "3216"
		},
		{
			"id": "3225",
			"maxLen": 4,
			"default": "3225"
		},
		{
			"id": "4516",
			"maxLen": 4,
			"default": "4516"
		},
		{
			"id": "4532",
			"maxLen": 4,
			"default": "4532"
		},
		{
			"id": "5025",
			"maxLen": 4,
			"default": "5025"
		},
		{
			"id": "6332",
			"maxLen": 4,
			"default": "6332"
		}
	],
	"menuRootGroups": [
		{
			"id": "ModeMenu",
			"maxLen": 11
		},
		{
			"id": "SettingsMenu",
			"maxLen": 11
		}
	],
	"menuOptions": [
		{
			"id": "AutomaticSolderPasteDispenser",
			"maxLen": 20,
			"maxLen2": 30
		},
		{
			"id": "VacuumPickUp",
			"maxLen": 10,
			"maxLen2": 20
		},
		{
			"id": "Contrast",
			"maxLen": 4,
			"maxLen2": 9
		},
		{
			"id": "ScrollingSpeed",
			"maxLen": 6,
			"maxLen2": 11
		},
		{
			"id": "SettingsReset",
			"maxLen": 5,
			"maxLen2": 11
		}
	]
}
