#include <iostream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define WHITEBG "\033[47m"      /* White */
#define BOLD		"\033[1m"		 /* Bold White */
#define BOLDBLACK   BOLD << BLACK    /* Bold Black */
#define BOLDRED     BOLD << RED      /* Bold Red */
#define BOLDGREEN   BOLD << GREEN    /* Bold Green */
#define BOLDYELLOW  BOLD << YELLOW   /* Bold Yellow */
#define BOLDBLUE    BOLD << BLUE     /* Bold Blue */
#define BOLDMAGENTA BOLD << MAGENTA  /* Bold Magenta */
#define BOLDCYAN    BOLD << CYAN     /* Bold Cyan */
#define BOLDWHITE   BOLD << WHITE    /* Bold White */

#define ITALIC  "\033[3m"

// MAKROS
#define PRINT_RED(p) std::cout << RED p RESET << std::endl
#define TEST_PASSED std::cout << GREEN "PASSED" RESET << std::endl
#define TEST_FAILED PRINT_RED("FAILED")
