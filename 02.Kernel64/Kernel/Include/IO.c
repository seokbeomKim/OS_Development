#include <Include/IO.h>
#include <Include/Converter.h>

/**
 *  sprintf() �Լ��� ���� ����
 */
int kSPrintf( char* pcBuffer, const char* pcFormatString, ... )
{
    va_list ap;
    int iReturn;

    // ���� ���ڸ� ������ vsprintf() �Լ��� �Ѱ���
    va_start( ap, pcFormatString );
    iReturn = kVSPrintf( pcBuffer, pcFormatString, ap );
    va_end( ap );

    return iReturn;
}

/**
 *  vsprintf() �Լ��� ���� ����
 *      ���ۿ� ���� ���ڿ��� ���� �����͸� ����
 */
int kVSPrintf( char* pcBuffer, const char* pcFormatString, va_list ap )
{
    QWORD i, j;
    int iBufferIndex = 0;
    int iFormatLength, iCopyLength;
    char* pcCopyString;
    QWORD qwValue;
    int iValue;

    // ���� ���ڿ��� ���̸� �о ���ڿ��� ���̸�ŭ �����͸� ��� ���ۿ� ���
    iFormatLength = kStrLen( pcFormatString );
    for( i = 0 ; i < iFormatLength ; i++ )
    {
        // %�� �����ϸ� ������ Ÿ�� ���ڷ� ó��
        if( pcFormatString[ i ] == '%' )
        {
            // % ������ ���ڷ� �̵�
            i++;
            switch( pcFormatString[ i ] )
            {
                // ���ڿ� ���
            case 's':
                // ���� ���ڿ� ����ִ� �Ķ���͸� ���ڿ� Ÿ������ ��ȯ
                pcCopyString = ( char* ) ( va_arg(ap, char* ));
                iCopyLength = kStrLen( pcCopyString );
                // ���ڿ��� ���̸�ŭ�� ��� ���۷� �����ϰ� ����� ���̸�ŭ
                // ������ �ε����� �̵�
                kMemCpy( pcBuffer + iBufferIndex, pcCopyString, iCopyLength );
                iBufferIndex += iCopyLength;
                break;

                // ���� ���
            case 'c':
                // ���� ���ڿ� ����ִ� �Ķ���͸� ���� Ÿ������ ��ȯ�Ͽ�
                // ��� ���ۿ� �����ϰ� ������ �ε����� 1��ŭ �̵�
                pcBuffer[ iBufferIndex ] = ( char ) ( va_arg( ap, int ) );
                iBufferIndex++;
                break;

                // ���� ���
            case 'd':
            case 'i':
                // ���� ���ڿ� ����ִ� �Ķ���͸� ���� Ÿ������ ��ȯ�Ͽ�
                // ��� ���ۿ� �����ϰ� ����� ���̸�ŭ ������ �ε����� �̵�
                iValue = ( int ) ( va_arg( ap, int ) );
                iBufferIndex += kIToA( iValue, pcBuffer + iBufferIndex, 10 );
                break;

                // 4����Ʈ Hex ���
            case 'x':
            case 'X':
                // ���� ���ڿ� ����ִ� �Ķ���͸� DWORD Ÿ������ ��ȯ�Ͽ�
                // ��� ���ۿ� �����ϰ� ����� ���̸�ŭ ������ �ε����� �̵�
                qwValue = ( DWORD ) ( va_arg( ap, DWORD ) ) & 0xFFFFFFFF;
                iBufferIndex += kIToA( qwValue, pcBuffer + iBufferIndex, 16 );
                break;

                // 8����Ʈ Hex ���
            case 'q':
            case 'Q':
            case 'p':
                // ���� ���ڿ� ����ִ� �Ķ���͸� QWORD Ÿ������ ��ȯ�Ͽ�
                // ��� ���ۿ� �����ϰ� ����� ���̸�ŭ ������ �ε����� �̵�
                qwValue = ( QWORD ) ( va_arg( ap, QWORD ) );
                iBufferIndex += kIToA( qwValue, pcBuffer + iBufferIndex, 16 );
                break;

                // ���� �ش����� ������ ���ڸ� �״�� ����ϰ� ������ �ε�����
                // 1��ŭ �̵�
            default:
                pcBuffer[ iBufferIndex ] = pcFormatString[ i ];
                iBufferIndex++;
                break;
            }
        }
        // �Ϲ� ���ڿ� ó��
        else
        {
            // ���ڸ� �״�� ����ϰ� ������ �ε����� 1��ŭ �̵�
            pcBuffer[ iBufferIndex ] = pcFormatString[ i ];
            iBufferIndex++;
        }
    }

    // NULL�� �߰��Ͽ� ������ ���ڿ��� ����� ����� ������ ���̸� ��ȯ
    pcBuffer[ iBufferIndex ] = '\0';
    return iBufferIndex;
}
