#include <DeviceDriver/Console.h>
#include <Include/Mem.h>
#include <Asm/AsmHelper.h>
#include <Include/Converter.h>
#include <stdarg.h>

CONSOLEMANAGER gs_stConsoleManager = {0, };

void kInitializeConsole(int iX, int iY)
{
	kMemSet(&gs_stConsoleManager, 0, sizeof(gs_stConsoleManager));
	kSetCursor(iX, iY);
}

/*
 * Set a position of cursor, using CRTC Controller
 */
void kSetCursor(int iX, int iY)
{
	int iLinearValue;

	iLinearValue = iY * CONSOLE_WIDTH + iX;
	kWriteToPort(VGA_PORT_INDEX,	VGA_INDEX_UPPERCURSOR);
	kWriteToPort(VGA_PORT_DATA,		iLinearValue >> 8);

	kWriteToPort(VGA_PORT_INDEX,	VGA_INDEX_LOWERCURSOR);
	kWriteToPort(VGA_PORT_DATA,		iLinearValue & 0xFF);

	gs_stConsoleManager.iCurrentPrintOffset = iLinearValue;
}

void kGetCursor(int *piX, int *piY)
{
	*piX = gs_stConsoleManager.iCurrentPrintOffset % CONSOLE_WIDTH;
	*piY = gs_stConsoleManager.iCurrentPrintOffset / CONSOLE_WIDTH;
}

/*
 * Used within printf()
 */
void kPrintf(const char* pcFormatString, ...)
{
	char	vcBuffer[BUFFER_SIZE];
	int		iNextPrintOffset;
	va_list ap;

	va_start(ap, pcFormatString);
	kVSPrintf(vcBuffer, pcFormatString, ap);
	va_end(ap);

	iNextPrintOffset = kConsolePrintString(vcBuffer);
	kSetCursor(iNextPrintOffset % CONSOLE_WIDTH,
			iNextPrintOffset / CONSOLE_WIDTH);
}

/**
 *  \n, \t와 같은 문자가 포함된 문자열을 출력한 후, 화면상의 다음 출력할 위치를
 *  반환
 */
int kConsolePrintString( const char* pcBuffer )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDR;
    int i, j;
    int iLength;
    int iPrintOffset;

    // 문자열을 출력할 위치를 저장
    iPrintOffset = gs_stConsoleManager.iCurrentPrintOffset;

    // 문자열의 길이만큼 화면에 출력
    iLength = kStrLen( pcBuffer );
    for( i = 0 ; i < iLength ; i++ )
    {
        // 개행 처리
        if( pcBuffer[ i ] == '\n' )
        {
            // 출력할 위치를 80의 배수 컬럼으로 옮김
            // 현재 라인의 남은 문자열의 수만큼 더해서 다음 라인으로 위치시킴
            iPrintOffset += ( CONSOLE_WIDTH - ( iPrintOffset % CONSOLE_WIDTH ) );
        }
        // 탭 처리
        else if( pcBuffer[ i ] == '\t' )
        {
            // 출력할 위치를 8의 배수 컬럼으로 옮김
            iPrintOffset += ( 8 - ( iPrintOffset % 8 ) );
        }
        // 일반 문자열 출력
        else
        {
            // 비디오 메모리에 문자와 속성을 설정하여 문자를 출력하고
            // 출력할 위치를 다음으로 이동
            pstScreen[ iPrintOffset ].bCharacter = pcBuffer[ i ];
            pstScreen[ iPrintOffset ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
            iPrintOffset++;
        }

        // 출력할 위치가 화면의 최댓값(80 * 25)을 벗어났으면 스크롤 처리
        if( iPrintOffset >= ( CONSOLE_HEIGHT * CONSOLE_WIDTH ) )
        {
            // 가장 윗줄을 제외한 나머지를 한줄 위로 복사
            kMemCpy( CONSOLE_VIDEOMEMORYADDR,
                     CONSOLE_VIDEOMEMORYADDR + CONSOLE_WIDTH * sizeof( CHARACTER ),
                     (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH * sizeof( CHARACTER ) );

            // 가장 마지막 라인은 공백으로 채움
            for( j = ( CONSOLE_HEIGHT - 1 ) * ( CONSOLE_WIDTH ) ;
                 j < ( CONSOLE_HEIGHT * CONSOLE_WIDTH ) ; j++ )
            {
                // 공백 출력
                pstScreen[ j ].bCharacter = ' ';
                pstScreen[ j ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
            }

            // 출력할 위치를 가장 아래쪽 라인의 처음으로 설정
            iPrintOffset = ( CONSOLE_HEIGHT - 1 ) * CONSOLE_WIDTH;
        }
    }
    return iPrintOffset;
}

/**
 *  전체 화면을 삭제
 */
void kClearScreen( void )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDR;
    int i;

    // 화면 전체를 공백으로 채우고, 커서의 위치를 0, 0으로 옮김
    for( i = 0 ; i < CONSOLE_WIDTH * CONSOLE_HEIGHT ; i++ )
    {
        pstScreen[ i ].bCharacter = ' ';
        pstScreen[ i ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
    }

    // 커서를 화면 상단으로 이동
    kSetCursor( 0, 0 );
}

/**
 *  getch() 함수의 구현
 */
BYTE kGetCh( void )
{
    KEYDATA stData;

    // 키가 눌러질때까지 대기함
    while( 1 )
    {
        // 키 큐에 데이터가 수신될 때까지 대기
        while( kGetKeyFromKeyQueue( &stData ) == FALSE )
        {
            ;
        }

        // 키가 눌렸다는 데이터가 수신되면 ASCII 코드를 반환
        if( stData.bFlags & KEY_FLAGS_DOWN )
        {
            return stData.bASCIICode;
        }
    }
}

/**
 *  문자열을 X, Y 위치에 출력
 */
void kPrintStringXY( int iX, int iY, const char* pcString )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDR;
    int i;

    // 비디오 메모리 어드레스에서 현재 출력할 위치를 계산
    pstScreen += ( iY * CONSOLE_WIDTH ) + iX;
    // 문자열의 길이만큼 루프를 돌면서 문자와 속성을 저장
    for( i = 0 ; pcString[ i ] != 0 ; i++ )
    {
        pstScreen[ i ].bCharacter = pcString[ i ];
        pstScreen[ i ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
    }
}

