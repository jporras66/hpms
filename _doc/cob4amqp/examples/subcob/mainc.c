#include <stdio.h>
int main (int argc, char **argv) {
	int returnCode;
	char arg1[7] = "Arg1";
	char arg2[7] = "Arg2";
	unsigned long arg3 = 123456789;
	printf("Starting mainc...\n");
	//cob_init (argc, argv);
	cob_init(0, NULL);
	returnCode = subcob(arg1,arg2,&arg3);
	printf("Back\n");
	printf("Arg1=%s\n",arg1);
	printf("Arg2=%s\n",arg2);
	printf("Arg3=%d\n",arg3);
	printf("Returned value=%d\n",returnCode);
	return returnCode;
}