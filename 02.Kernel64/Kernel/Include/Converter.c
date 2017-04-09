#include <Include/Converter.h>

/**
 *  atoi() �Լ��� ���� ����
 */
long kAToI( const char* pcBuffer, int iRadix )
{
    long lReturn;

    switch( iRadix )
    {
        // 16����
    case 16:
        lReturn = kHexStringToQword( pcBuffer );
        break;

        // 10���� �Ǵ� ��Ÿ
    case 10:
    default:
        lReturn = kDecimalStringToLong( pcBuffer );
        break;
    }
    return lReturn;
}

/**
 *  16���� ���ڿ��� QWORD�� ��ȯ
 */
QWORD kHexStringToQword( const char* pcBuffer )
{
    QWORD qwValue = 0;
    int i;

    // ���ڿ��� ���鼭 ���ʷ� ��ȯ
    for( i = 0 ; pcBuffer[ i ] != '\0' ; i++ )
    {
        qwValue *= 16;
        if( ( 'A' <= pcBuffer[ i ] )  && ( pcBuffer[ i ] <= 'Z' ) )
        {
            qwValue += ( pcBuffer[ i ] - 'A' ) + 10;
        }
        else if( ( 'a' <= pcBuffer[ i ] )  && ( pcBuffer[ i ] <= 'z' ) )
        {
            qwValue += ( pcBuffer[ i ] - 'a' ) + 10;
        }
        else
        {
            qwValue += pcBuffer[ i ] - '0';
        }
    }
    return qwValue;
}

/**
 *  10���� ���ڿ��� long���� ��ȯ
 */
long kDecimalStringToLong( const char* pcBuffer )
{
    long lValue = 0;
    int i;

    // �����̸� -�� �����ϰ� �������� ���� long���� ��ȯ
    if( pcBuffer[ 0 ] == '-' )
    {
        i = 1;
    }
    else
    {
        i = 0;
    }

    // ���ڿ��� ���鼭 ���ʷ� ��ȯ
    for( ; pcBuffer[ i ] != '\0' ; i++ )
    {
        lValue *= 10;
        lValue += pcBuffer[ i ] - '0';
    }

    // �����̸� - �߰�
    if( pcBuffer[ 0 ] == '-' )
    {
        lValue = -lValue;
    }
    return lValue;
}

/**
 *  itoa() �Լ��� ���� ����
 */
int kIToA( long lValue, char* pcBuffer, int iRadix )
{
    int iReturn;

    switch( iRadix )
    {
        // 16����
    case 16:
        iReturn = kHexToString( lValue, pcBuffer );
        break;

        // 10���� �Ǵ� ��Ÿ
    case 10:
    default:
        iReturn = kDecimalToString( lValue, pcBuffer );
        break;
    }

    return iReturn;
}

/**
 *  16���� ���� ���ڿ��� ��ȯ
 */
int kHexToString( QWORD qwValue, char* pcBuffer )
{
    QWORD i;
    QWORD qwCurrentValue;

    // 0�� ������ �ٷ� ó��
    if( qwValue == 0 )
    {
        pcBuffer[ 0 ] = '0';
        pcBuffer[ 1 ] = '\0';
        return 1;
    }

    // ���ۿ� 1�� �ڸ����� 16, 256, ...�� �ڸ� ������ ���� ����
    for( i = 0 ; qwValue > 0 ; i++ )
    {
        qwCurrentValue = qwValue % 16;
        if( qwCurrentValue >= 10 )
        {
            pcBuffer[ i ] = 'A' + ( qwCurrentValue - 10 );
        }
        else
        {
            pcBuffer[ i ] = '0' + qwCurrentValue;
        }

        qwValue = qwValue / 16;
    }
    pcBuffer[ i ] = '\0';

    // ���ۿ� ����ִ� ���ڿ��� ����� ... 256, 16, 1�� �ڸ� ������ ����
    kReverseString( pcBuffer );
    return i;
}

/**
 *  10���� ���� ���ڿ��� ��ȯ
 */
int kDecimalToString( long lValue, char* pcBuffer )
{
    long i;

    // 0�� ������ �ٷ� ó��
    if( lValue == 0 )
    {
        pcBuffer[ 0 ] = '0';
        pcBuffer[ 1 ] = '\0';
        return 1;
    }

    // ���� �����̸� ��� ���ۿ� '-'�� �߰��ϰ� ����� ��ȯ
    if( lValue < 0 )
    {
        i = 1;
        pcBuffer[ 0 ] = '-';
        lValue = -lValue;
    }
    else
    {
        i = 0;
    }

    // ���ۿ� 1�� �ڸ����� 10, 100, 1000 ...�� �ڸ� ������ ���� ����
    for( ; lValue > 0 ; i++ )
    {
        pcBuffer[ i ] = '0' + lValue % 10;
        lValue = lValue / 10;
    }
    pcBuffer[ i ] = '\0';

    // ���ۿ� ����ִ� ���ڿ��� ����� ... 1000, 100, 10, 1�� �ڸ� ������ ����
    if( pcBuffer[ 0 ] == '-' )
    {
        // ������ ���� ��ȣ�� �����ϰ� ���ڿ��� ������
        kReverseString( &( pcBuffer[ 1 ] ) );
    }
    else
    {
        kReverseString( pcBuffer );
    }

    return i;
}

/**
 *  ���ڿ��� ������ ������
 */
void kReverseString( char* pcBuffer )
{
   int iLength;
   int i;
   char cTemp;


   // ���ڿ��� ����� �߽����� ��/�츦 �ٲ㼭 ������ ������
   iLength = kStrLen( pcBuffer );
   for( i = 0 ; i < iLength / 2 ; i++ )
   {
       cTemp = pcBuffer[ i ];
       pcBuffer[ i ] = pcBuffer[ iLength - 1 - i ];
       pcBuffer[ iLength - 1 - i ] = cTemp;
   }
}
