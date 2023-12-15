
#include "assignment.h"

int main(int argc, char *argv[])
{
    AssignementApplication application("Assignement", "1.0");

    application.Init();

    application.Run();

    application.stop();
}
