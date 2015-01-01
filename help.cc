#include "acejudge.h"

const char help_text[] = "\
acejudge [config file] [-fast] [-l] [-help] [-version] \n\
config file: load config file automaticly \n\
-fast: judge without configing (need config file) \n\
-l: show what is in the config file (need config file) \n\
-help: show this page \n\
-version: show version info \n\
";

void show_help() {
	setcolor(0);
	puts(help_text);
}

const char version_text[] = "\
\33[30m\33[42mAce Judge\33[0m \33[0mV3.0 Beta2 \33[35m(Release 01/01 2015) \n\
			\33[34mby CDQZ_\33[31ml\33[32ma\33[33me\33[34mk\33[35mo\33[36mv\33[0m \
";

void show_version() {
	setcolor(0);
	puts(version_text);
}
