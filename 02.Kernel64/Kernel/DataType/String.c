#include <DataType/String.h>

int kStrLen( const char* pcBuffer )
{
    int i = 0;

    for( ; ; i++ )
    {
        if( pcBuffer[ i ] == '\0' )
        {
            break;
        }
    }
    return i;
}
