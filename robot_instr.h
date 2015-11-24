#ifndef _ROBOT_INSTR_INCLUDED
#define _ROBOT_INSTR_INCLUDED
enum command_instruction {
               MOTOR_1_GO                  = 139,
			   MOTOR_2_GO                  = 140,
			   BOTH_MOTORS_GO_SAME         = 141,
			   BOTH_MOTORS_GO_OPPOSITE     = 142,
			   ACTIVATE_ULTRA_SONIC_1      = 143,
			   ACTIVATE_ULTRA_SONIC_2      = 144,
			   SHUTDOWN_ROBOT              = 145,
			   RAMP_TIME                   = 146,
			   WRITE_PORT_0                = 147,
			   WRITE_PORT_1                = 148,
			   WRITE_PORT_2                = 149,
			   WRITE_PORT_3                = 150,
			   WRITE_PORT_4                = 151,
			   WRITE_PORT_5                = 152,
			   WRITE_PORT_6                = 153,
			   WRITE_PORT_7                = 154,
			   STOP_SELECT                 = 155,
			   STOP_IF_HIGH                = 156,
			   STOP_IF_LOW                 = 157,
			   SET_BAUD_RATE               = 159,
               MOTOR_3_GO                  = 168,
               MOTOR_4_GO                  = 169
};

enum request_instruction {
               ADC0                        = 9,
			   ADC1                        = 10,
			   ADC2                        = 11,
			   ADC3                        = 12,
			   ADC4                        = 13,
			   ADC5                        = 14,
			   ADC6                        = 15,
			   ADC7                        = 16,
			   READ_PORT_0                 = 17,
			   READ_PORT_1                 = 18,
			   READ_PORT_2                 = 19,
			   READ_PORT_3                 = 20,
			   READ_PORT_4                 = 21,
			   READ_PORT_5                 = 22,
			   READ_PORT_6                 = 23,
			   READ_PORT_7                 = 24,
			   MOTOR_1                     = 25,
			   MOTOR_2                     = 26,
			   ULTRA_SONIC_1               = 27,
			   ULTRA_SONIC_2               = 28,
			   TEST_INSTRUCTION            = 29,
			   STATUS                      = 31,
               MOTOR_3                     = 41,
               MOTOR_4                     = 42
};

const unsigned char TEST_INSTRUCTION_RESULT=123;

enum baud_rate { B_300,B_600,B_1200,B_2400,B_4800 };

struct robot_command {
    command_instruction opcode;
    unsigned char       parameter;
};

struct robot_request {
    request_instruction opcode;
    unsigned char       parameter;
};

#endif /* _ROBOT_INSTR_INCLUDED */
