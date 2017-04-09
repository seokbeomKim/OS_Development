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
 *  \n, \t�� ���� ���ڰ� ���Ե� ���ڿ��� ����� ��, ȭ����� ���� ����� ��ġ��
 *  ��ȯ
 */
int kConsolePrintString( const char* pcBuffer )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDR;
    int i, j;
    int iLength;
    int iPrintOffset;

    // ���ڿ��� ����� ��ġ�� ����
    iPrintOffset = gs_stConsoleManager.iCurrentPrintOffset;

    // ���ڿ��� ���̸�ŭ ȭ�鿡 ���
    iLength = kStrLen( pcBuffer );
    for( i = 0 ; i < iLength ; i++ )
    {
        // ���� ó��
        if( pcBuffer[ i ] == '\n' )
        {
            // ����� ��ġ�� 80�� ��� �÷����� �ű�
            // ���� ������ ���� ���ڿ��� ����ŭ ���ؼ� ���� �������� ��ġ��Ŵ
            iPrintOffset += ( CONSOLE_WIDTH - ( iPrintOffset % CONSOLE_WIDTH ) );
        }
        // �� ó��
        else if( pcBuffer[ i ] == '\t' )
        {
            // ����� ��ġ�� 8�� ��� �÷����� �ű�
            iPrintOffset += ( 8 - ( iPrintOffset % 8 ) );
        }
        // �Ϲ� ���ڿ� ���
        else
        {
            // ���� �޸𸮿� ���ڿ� �Ӽ��� �����Ͽ� ���ڸ� ����ϰ�
            // ����� ��ġ�� �������� �̵�
            pstScreen[ iPrintOffset ].bCharacter = pcBuffer[ i ];
            pstScreen[ iPrintOffset ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
            iPrintOffset++;
        }

        // ����� ��ġ�� ȭ���� �ִ�(80 * 25)�� ������� ��ũ�� ó��
        if( iPrintOffset >= ( CONSOLE_HEIGHT * CONSOLE_WIDTH ) )
        {
            // ���� ������ ������ �������� ���� ���� ����
            kMemCpy( CONSOLE_VIDEOMEMORYADDR,
                     CONSOLE_VIDEOMEMORYADDR + CONSOLE_WIDTH * sizeof( CHARACTER ),
                     (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH * sizeof( CHARACTER ) );

            // ���� ������ ������ �������� ä��
            for( j = ( CONSOLE_HEIGHT - 1 ) * ( CONSOLE_WIDTH ) ;
                 j < ( CONSOLE_HEIGHT * CONSOLE_WIDTH ) ; j++ )
            {
                // ���� ���
                pstScreen[ j ].bCharacter = ' ';
                pstScreen[ j ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
            }

            // ����� ��ġ�� ���� �Ʒ��� ������ ó������ ����
            iPrintOffset = ( CONSOLE_HEIGHT - 1 ) * CONSOLE_WIDTH;
        }
    }
    return iPrintOffset;
}

/**
 *  ��ü ȭ���� ����
 */
void kClearScreen( void )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDR;
    int i;

    // ȭ�� ��ü�� �������� ä���, Ŀ���� ��ġ�� 0, 0���� �ű�
    for( i = 0 ; i < CONSOLE_WIDTH * CONSOLE_HEIGHT ; i++ )
    {
        pstScreen[ i ].bCharacter = ' ';
        pstScreen[ i ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
    }

    // Ŀ���� ȭ�� ������� �̵�
    kSetCursor( 0, 0 );
}

/**
 *  getch() �Լ��� ����
 */
BYTE kGetCh( void )
{
    KEYDATA stData;

    // Ű�� ������������ �����
    while( 1 )
    {
        // Ű ť�� �����Ͱ� ���ŵ� ������ ���
        while( kGetKeyFromKeyQueue( &stData ) == FALSE )
        {
            ;
        }

        // Ű�� ���ȴٴ� �����Ͱ� ���ŵǸ� ASCII �ڵ带 ��ȯ
        if( stData.bFlags & KEY_FLAGS_DOWN )
        {
            return stData.bASCIICode;
        }
    }
}

/**
 *  ���ڿ��� X, Y ��ġ�� ���
 */
void kPrintStringXY( int iX, int iY, const char* pcString )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDR;
    int i;

    // ���� �޸� ��巹������ ���� ����� ��ġ�� ���
    pstScreen += ( iY * CONSOLE_WIDTH ) + iX;
    // ���ڿ��� ���̸�ŭ ������ ���鼭 ���ڿ� �Ӽ��� ����
    for( i = 0 ; pcString[ i ] != 0 ; i++ )
    {
        pstScreen[ i ].bCharacter = pcString[ i ];
        pstScreen[ i ].bAttribute = CONSOLE_DEFAULT_TEXT_COLOR;
    }
}

