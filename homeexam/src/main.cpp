
#include "homeexam.h"

int main(int argc, char* argv[])
{
    HomeExamApplication application("HomeExam", "1.0");

    application.Init();

    application.Run();

    application.stop();
}
