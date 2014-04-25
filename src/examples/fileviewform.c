//Compile with:
// gcc -o fileviewform fileviewform.c `pkg-config --cflags --libs emodel`

# include "../../elementary_config.h"

#include <Elementary.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <stdio.h>

#define EMODEL_TEST_FILENAME_PATH "/tmp"

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   ecore_init();
   ecore_shutdown();
   return 0;
}
ELM_MAIN()

