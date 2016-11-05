#include <DeviceDriver/DeviceDriver.h>
#include <Asm/AsmHelper.h>

#define MAXIMUM_WAIT_COUNT	0xFFFF

static KEYBOARD_MANAGER keyboardManager = {0,};	// 키보드 상태 관리

/*
 * Enable keyboard and initialize the device
 */
void kInitializeKeyboard(void)
{
	if (kActivateKeyboard() == TRUE) {
		kPrintString(0, 11, "[KERNEL] Activate Keyboard");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);

		// KeyboardManager 초기화
		keyboardManager.bShiftDown		= FALSE;
		keyboardManager.bCapsLockOn 	= FALSE;
		keyboardManager.bNumLockOn		= FALSE;
		keyboardManager.bScrollLockOn	= FALSE;
		keyboardManager.bExtendedCodeIn	= FALSE;
		keyboardManager.iSkipCountForPause = 0;
	}
	else {
		kPrintString(0, 11, "[ERROR] Failed to activate keyboard");
		while (1);
	}
}

BOOL kIsWriteBufferAvailable(void) {
	int i;

	for (i = 0; i < MAXIMUM_WAIT_COUNT; i++) {
		if (kIsWriteBufferFull() == FALSE) {
			return TRUE;
		}
	}
	return TRUE;
}

BOOL kIsReadBufferAvailable(void) {
	int i;

	for (i = 0; i < MAXIMUM_WAIT_COUNT; i++) {
		if (kIsReadBufferFull() == TRUE) {
			return TRUE;
		}
	}
	return TRUE;
}

/*
 * In/Out은 모두 키보드를 기준으로 하기 때문에 OutBuffer라고 하면 키보드가 전송할 때 사용되는 버퍼이다.
 * Read buffer라고 명명한 이유이다.
 */
BOOL kIsReadBufferFull(void)	// OutBuffer
{
	/*
	 * 0x64 에서 읽은 값에 출력 버퍼 상태 비트[0] 이 1로 설정되어 있으면
	 * Output Buffer에 키보드가 전송한 데이터가 존재한다.
	 */
	if (kReadFromPort(0x64) & 0x01) {
		return TRUE;
	}
	return FALSE;
}

BOOL kIsWriteBufferFull(void)	// InBuffer
{
	/*
	 * 0x64 에서 읽은 값의 비트[1] 이 1로 설정되어 있으면 키보드 컨트롤러가 아직 입력
	 * buffer의 데이터를 가져가지 않았음을 나타내며 0으로 설정되면 컨트롤러가 데이터를 가져가
	 * Input Buffer가 비었음을 나타낸다.
	 */
	if (kReadFromPort(0x64) & 0x02) {
		kPrintString(0, 0, "ReadFromPort");
		return TRUE;
	}
	return FALSE;
}

BOOL kActivateKeyboard(void) {
	int i, j;

	// Activate keyboard (enable ps/2 port)
	kWriteToPort(0x64, 0xAE);

	// 현재는 0xFFFF 루프를 돌면서 활성화하지만 이 후에는 인터럽트로 바뀔 것이다.
	kIsWriteBufferAvailable();
	// 0x60, 0xF4 커맨드 전달하여 키보드로 전송
	kWriteToPort(0x60, 0xF4);

	for (j = 0; j < MAXIMUM_WAIT_COUNT; j++) {
		if (kIsReadBufferAvailable() == TRUE) {
			// 버퍼가 차있다면 0x60에서 ACK 읽음
			if (kReadFromPort(0x60) == 0xFA) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

BYTE kGetKeyboardScanCode(void)
{
	while (kIsReadBufferFull() == FALSE) {
		;
	}
	return kReadFromPort(0x60);	// 출력 버퍼 (0x60)에서 키 값을 읽어서 반환
}

BOOL kChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn)
{
	int i, j;

	// 데이터 전송하기 전에 버퍼 확인
	if (kIsWriteBufferAvailable() == TRUE) {
		// LED 상태 변경 커맨드 전송
		kWriteToPort(0x60, 0xED);
	}

	for (i = 0; i < 0xFFFF; i++) {
		if (kIsWriteBufferFull() == FALSE) {
			break;
		}
	}

	for (j = 0; j < 100; j++) {
		for (i = 0; i < 0xFFFF; i++) {
			if (kIsReadBufferFull() == TRUE) {
				break;
			}
		}

		if (kReadFromPort(0x60) == 0xFA) {
			// ACK 받은 경우
			break;
		}
	}
	if (j >= 100) {
		return FALSE;
	}

	// LED 변경 커맨드 전송
	kWriteToPort(0x60, (bCapsLockOn << 2) | (bNumLockOn << 1) | bScrollLockOn);
	for (i = 0; i < 0xFFFF; i++) {
		if (kIsWriteBufferFull() == FALSE) {
			break;
		}
	}

	for (j = 0; j < 100; j++) {
		for (i = 0; i < 0xFFFF; i++) {
			if (kIsReadBufferFull() == TRUE) {
				break;
			}
		}

		if (kReadFromPort(0x60) == 0xFA) {
			break;
		}
	}

	if (j >= 100) {
		return FALSE;
	}

	return TRUE;
}

void kReboot(void) {
	int i;

	if (kIsWriteBufferAvailable() == TRUE) {
		kWriteToPort(0x64, 0xD1);
		kWriteToPort(0x60, 0x00);
	}

	while (1);
}

static KEYMAPPING_ENTRY vstKeyMappingTable[KEY_MAPPING_TABLE_MAX] = {
	/* 0	*/	{	KEY_NONE	,	KEY_NONE	},
	/* 1	*/	{	KEY_ESC		,	KEY_ESC		},
	/* 2	*/	{	'1'			,	'!'			},
	/* 3	*/	{	'2'			,	'@'			},
	/* 4	*/	{	'3'			,	'#'			},
	/* 5	*/	{	'4'			,	'$'			},
	/* 6	*/	{	'5'			,	'%'			},
	/* 7	*/	{	'6'			,	'^'			},
	/* 8	*/	{	'7'			,	'&'			},
	/* 9	*/	{	'8'			,	'*'			},
	/* 10	*/	{	'9'			,	'('			},
	/* 11	*/	{	'0'			,	')'			},
	/* 12	*/	{	'-'			,	'_'			},
	/* 13	*/	{	'='			,	'+'			},
	/* 14	*/	{KEY_BACKSPACE	,KEY_BACKSPACE	},
	/* 15	*/	{	KEY_TAB		,	KEY_TAB		},
	/* 16	*/	{	'q'			,	'Q'			},
	/* 17	*/	{	'w'			,	'W'			},
	/* 18	*/	{	'e'			,	'E'			},
	/* 19	*/	{	'r'			,	'R'			},
	/* 20	*/	{	't'			,	'T'			},
	/* 21	*/	{	'y'			,	'Y'			},
	/* 22	*/	{	'u'			,	'U'			},
	/* 23	*/	{	'i'			,	'I'			},
	/* 24	*/	{	'o'			,	'O'			},
	/* 25	*/	{	'p'			,	'P'			},
	/* 26	*/	{	'['			,	'{'			},
	/* 27	*/	{	']'			,	'}'			},
	/* 28	*/	{	'\n'		,	'\n'		},
	/* 29	*/	{	KEY_CTRL	,	KEY_CTRL	},
	/* 30	*/	{	'a'			,	'A'			},
	/* 31	*/	{	's'			,	'S'			},
	/* 32	*/	{	'd'			,	'D'			},
	/* 33	*/	{	'f'			,	'F'			},
	/* 34	*/	{	'g'			,	'G'			},
	/* 35	*/	{	'h'			,	'H'			},
	/* 36	*/	{	'j'			,	'J'			},
	/* 37	*/	{	'k'			,	'K'			},
	/* 38	*/	{	'l'			,	'L'			},
	/* 39	*/	{	';'			,	':'			},
	/* 40	*/	{	'\''		,	'\"'		},
	/* 41	*/	{	'`'			,	'~'			},
	/* 42	*/	{	KEY_LSHIFT	,	KEY_LSHIFT	},
	/* 43	*/	{	'\\'		,	'|'			},
	/* 44	*/	{	'z'			,	'Z'			},
	/* 45	*/	{	'x'			,	'X'			},
	/* 46	*/	{	'c'			,	'C'			},
	/* 47	*/	{	'v'			,	'V'			},
	/* 48	*/	{	'b'			,	'B'			},
	/* 49	*/	{	'n'			,	'N'			},
	/* 50	*/	{	'm'			,	'M'			},
	/* 51	*/	{	','			,	'<'			},
	/* 52	*/	{	'.'			,	'>'			},
	/* 53	*/	{	'/'			,	'?'			},
	/* 54	*/	{	KEY_RSHIFT	,	KEY_RSHIFT	},
	/* 55	*/	{	'*'			,	'*'			},
	/* 56	*/	{	KEY_LALT	,	KEY_LALT	},
	/* 57	*/	{	' '			,	' '			},
	/* 58	*/	{	KEY_CAPSLOCK,	KEY_CAPSLOCK},
	/* 59	*/	{	KEY_F1		,	KEY_F1		},
	/* 60	*/	{	KEY_F2		,	KEY_F2		},
	/* 61	*/	{	KEY_F3		,	KEY_F3		},
	/* 62	*/	{	KEY_F4		,	KEY_F4		},
	/* 63	*/	{	KEY_F5		,	KEY_F5		},
	/* 64	*/	{	KEY_F6		,	KEY_F6		},
	/* 65	*/	{	KEY_F7		,	KEY_F7		},
	/* 66	*/	{	KEY_F8		,	KEY_F8		},
	/* 67	*/	{	KEY_F9		,	KEY_F9		},
	/* 68	*/	{	KEY_F10		,	KEY_F10		},
	/* 69	*/	{	KEY_NUMLOCK	,	KEY_NUMLOCK	},
	/* 70	*/	{KEY_SCROLLLOCK	,KEY_SCROLLLOCK	},

	/* 71	*/	{	KEY_HOME	,	'7'			},
	/* 72	*/	{	KEY_UP		,	'8'			},
	/* 73	*/	{	KEY_PAGEUP	,	'9'			},
	/* 74	*/	{	'-'			,	'-'			},
	/* 75	*/	{	KEY_LEFT	,	'4'			},
	/* 76	*/	{	KEY_CENTER	,	'5'			},
	/* 77	*/	{	KEY_RIGHT	,	'6'			},
	/* 78	*/	{	'+'			,	'+'			},
	/* 79	*/	{	KEY_END		,	'1'			},
	/* 80	*/	{	KEY_DOWN	,	'2'			},
	/* 81	*/	{	KEY_PAGEDOWN,	'3'			},
	/* 82	*/	{	KEY_INS		,	'0'			},
	/* 83	*/	{	KEY_DEL		,	'.'			},
	/* 84	*/	{	KEY_NONE	,	KEY_NONE	},
	/* 85	*/	{	KEY_NONE	,	KEY_NONE	},
	/* 86	*/	{	KEY_NONE	,	KEY_NONE	},
	/* 87	*/	{	KEY_F11		,	KEY_F11		},
	/* 88	*/	{	KEY_F12		,	KEY_F12		},
};

// scancode가 알파벳 범위인지 확인
BOOL kIsAlphabetscanCode (BYTE bScanCode) {
	if (('a' <= vstKeyMappingTable[bScanCode].bNormalCode) &&
			(vstKeyMappingTable[bScanCode].bNormalCode) <= 'z') {
		return TRUE;
	}

	return FALSE;
}

BOOL kIsNumberOrSymbolScanCode(BYTE bScanCode) {
	if ( (2 <= bScanCode) && (bScanCode <= 53) &&
			(kIsAlphabetscanCode(bScanCode) == FALSE)) {
		return TRUE;
	}
	return FALSE;
}

BOOL kIsNumberPadScanCode(BYTE bScanCode) {
	if ( (71 <= bScanCode) && (bScanCode <= 83) ) {
		return TRUE;
	}
	return FALSE;
}

BOOL kIsUseCombinedCode(BOOL bScanCode) {
	BYTE bDownScanCode;
	BOOL bUseCombinedKey;

	bDownScanCode = bScanCode & 0x7F;

	if (kIsAlphabetscanCode(bDownScanCode) == TRUE) {
		if ((keyboardManager.bShiftDown ^ keyboardManager.bCapsLockOn)) {
			bUseCombinedKey = TRUE;
		}
		else {
			bUseCombinedKey = FALSE;
		}
	}

	else if (kIsNumberOrSymbolScanCode (bDownScanCode) == TRUE) {
		if (keyboardManager.bShiftDown == TRUE) {
			bUseCombinedKey = TRUE;
		}
		else {
			bUseCombinedKey = FALSE;
		}
	}
	else if ( (kIsNumberPadScanCode(bDownScanCode) == TRUE) &&
			(keyboardManager.bExtendedCodeIn == FALSE)) {
		if (keyboardManager.bNumLockOn == TRUE) {
			bUseCombinedKey = TRUE;
		}
		else {
			bUseCombinedKey = FALSE;
		}
	}

	return bUseCombinedKey;
}

// 조합키 상태 갱신 및 LED 상태 동기화
void UpdateCombinationKeyStatusAndLED(BYTE bScanCode)
{
	BOOL	isDown;
	BYTE 	bDownScanCode;
	BOOL	isLEDStatusChanged = FALSE;

	if (bScanCode & 0x80) {	// upcode
		isDown	= FALSE;	// 눌려있는지 나타내는 플래그
		bDownScanCode = bScanCode & 0x7F;
	}
	else {
		isDown	= TRUE;
		bDownScanCode = bScanCode;
	}

	// 조합키인 경우
	if ( (bDownScanCode == 0x2A) || (bDownScanCode == 54) ) {	// {Left, Right} Shift
		keyboardManager.bShiftDown = isDown;
	}
	else if ( (bDownScanCode == 0X3A) && (isDown == TRUE) ) {	// Caps Lock 상태 갱신 및 LED 상태 변경
		keyboardManager.bCapsLockOn ^= 0xFF;
		isLEDStatusChanged = TRUE;
	}
	else if ( (bDownScanCode == 69) && (isDown == TRUE) ) {	// Num Lock 상태 갱신 및 LED 상태 변경
		keyboardManager.bNumLockOn ^= TRUE;
		isLEDStatusChanged = TRUE;
	}
	else if ( (bDownScanCode == 70) && (isDown == TRUE) ) {	// Scroll Lock 상태 갱신, LED 상태 변경
		keyboardManager.bScrollLockOn ^= TRUE;
		isLEDStatusChanged = TRUE;
	}

	if (isLEDStatusChanged == TRUE) {
		kChangeKeyboardLED(keyboardManager.bCapsLockOn,
							keyboardManager.bNumLockOn,
							keyboardManager.bScrollLockOn);
	}
}

BOOL kConvertScanCodeToASCIICode(BYTE bScanCode,BYTE* pbASCIICode, BOOL* pbFlags) {
	BOOL	bUseCombinedKey;
	if (keyboardManager.iSkipCountForPause > 0) {
		keyboardManager.iSkipCountForPause--;
		return FALSE;
	}

	if (bScanCode == 0xE1) {	// PAUSE 키 처리
		*pbASCIICode	= KEY_PAUSE;
		*pbFlags		= KEY_FLAGS_DOWN;
		keyboardManager.iSkipCountForPause = KEY_SKIPCOUNTFORPAUSE;
		return TRUE;
	}

	// 확장 키 코드가 들어왔을 때, 실제 키 값은 다음에 들어오므로 플래그 설정만 하고 종료
	else if (bScanCode == 0xE0) {
		keyboardManager.bExtendedCodeIn = TRUE;
		return FALSE;
	}

	bUseCombinedKey = kIsUseCombinedCode(bScanCode);

	// 키 값 설정
	if (bUseCombinedKey == TRUE) {
		*pbASCIICode = vstKeyMappingTable[bScanCode & 0x7F].bCombinedCode;
	}
	else {
		*pbASCIICode = vstKeyMappingTable[bScanCode & 0x7F].bNormalCode;
	}

	// 확장키 유무 설정
	if (keyboardManager.bExtendedCodeIn == TRUE) {
		*pbFlags = KEY_FLAGS_EXTENDEDKEY;
		keyboardManager.bExtendedCodeIn = FALSE;
	}
	else {
		*pbFlags = 0;
	}

	if ( (bScanCode & 0x80) == 0) {
		*pbFlags |= KEY_FLAGS_DOWN;
	}

	UpdateCombinationKeyStatusAndLED(bScanCode);
	return TRUE;
}


