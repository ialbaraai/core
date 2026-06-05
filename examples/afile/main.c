#include "bash.h"

int main(void)
{
    Bash_Initialize();
    Write_Initialize();

    object_t bash = {0};
    Bash_Class.constructor(&bash, 0, NULL);

    core_object_call(&bash, "run", 0, NULL);

    Bash_Class.destructor(&bash);

    core_class_destroy(&Write_Class);
    core_class_destroy(&Bash_Class);
}