#include "main.h"

using namespace ydleMaster;

int main(int argc, char** argv)
{

	if (setuid(0)) {
		perror("setuid");
		return 1;
	}

	Master master (argc, argv) ;

	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = exit_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	YDLE_DEBUG << "Program start";

  master.Start() ;

  // TODO: (Denia) Temporaire
  while (1) {
    sleep(1000);
  }

	YDLE_INFO << "program end"; // end execution .
	return 0 ;
}

void exit_handler(int s) {
	YDLE_INFO << "Caught signal: " << s;
	scheduler_standard();
	exit(1);
}