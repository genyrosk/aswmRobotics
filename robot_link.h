#ifndef _ROBOT_LINK_INCLUDED
#define _ROBOT_LINK_INCLUDED
#ifndef _ROBOT_INSTR_INCLUDED
#include "robot_instr.h"
#endif

enum link_type { none, local, remote };

enum link_err {
	LINKERR_NONE        =  0,      // no error
	LINKERR_COMMS       =  1,      // fatal comms error
	LINKERR_READ        =  2,      // read error
	LINKERR_WRITE       =  3,      // write error
	LINKERR_CHKSUM      =  4,      // checksum error
	LINKERR_NOTINIT     =  5,      // link not initialised
	LINKERR_NOHOST      =  6,      // no remote host specified
	LINKERR_BADHOST     =  7,      // unknown remote host
	LINKERR_BADSERV     =  8,      // rs232d service unknown
	LINKERR_SKT1        =  9,      // error creating socket
	LINKERR_SKT2        = 10,      // error connecting socket
	LINKERR_SKT3        = 11,      // error setting non-blocking IO
	LINKERR_OVERFLOW    = 12       // error buffer overflow
};
const int ERR_BUFLEN = 16;
const int REQUEST_ERROR = -1;

class robot_link {
public:
// Initialisation
    robot_link(void);
    ~robot_link(void);
    bool initialise(void);
    bool initialise(baud_rate speed);
    bool initialise(const char *path, baud_rate speed);
    void change_baud_rate(baud_rate speed);
// Input output
    robot_link operator<< (robot_command cmd);
    robot_link operator>> (robot_request &req);
    bool command (command_instruction cmd, int arg);
    int request (request_instruction);
    bool transact (unsigned char cmd, unsigned char &arg);
    void flush();
// Error handling
    void clear_errs(void);
    link_err get_err (void);
    link_err lookup_err (int n);
    const char *err_string(void);
    const char *err_string(link_err code);
    void print_errs(const char *prefix);
    void print_errs(void);
    bool any_errs (void) { return (err_bufcnt > 0); };
    bool fatal_err (void) { return (fatal_errflag); };
    int  send_errs;
    int  recv_errs;
    int  cmd_errs;
private:
    int init_local(const char *, baud_rate);
    int init_net(const char *, baud_rate);
    bool send(int, link_type, unsigned char, unsigned char);
    bool recv(int, link_type, unsigned char &, unsigned char &);
    void record_err(link_err);
    baud_rate speed;
    int  fd;
    link_type mode;
    link_err err_buf[ERR_BUFLEN];
    int err_bufcnt;
    bool fatal_errflag;
};

#endif /* _ROBOT_LINK_INCLUDED */
