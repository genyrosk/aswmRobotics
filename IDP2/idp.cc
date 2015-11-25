#include "idp.h"


Idp::Idp(){
	connected = false;
	robot_link  rlink; 
	connect_remotely();
}

int Idp::connect_remotely(){
	cout << "connection_status: " << connected << endl;
	if(connected == true){
		return 1;
	}
	
	watch.start();
	cout << "..." << endl;

	int   val;
	// data from microprocessor
	if (!rlink.initialise(ROBOT_NUM)) {      // setup the link
		cout << "Cannot initialise link" << endl;
		rlink.print_errs("    ");
		return -1;
	}
	
	val = rlink.request(TEST_INSTRUCTION);   // send test instruction
	if (val == TEST_INSTRUCTION_RESULT) {     // check result
		cout << "Connected" << endl;
		connected = true;
		//return 0;                             // all OK, finish
	}
	else if (val == REQUEST_ERROR) {
		cout << "Fatal errors on link:" << endl;
		rlink.print_errs();
		return -1;
	}
	else{
		cout << "Test failed (bad value returned)" << endl;
		return -1;
	// error, finish
	}
	cout << "connection time " << watch.stop() << endl;
	return 1;
}
