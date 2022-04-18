#include "ecs/composer.h"
#include "ecs/entity_array.h"
#include "ecs/system.h"
#include "definitions.h"

ztech::ecs::composer global_composer;

int main( int argc, char* argv[] )
{
    printf( "Delta Seconds: %f\n", global_composer.get_delta_seconds( ) );

    printf( "Success\n" );
    return 0;
}