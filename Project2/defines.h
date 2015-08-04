/* here we have a list of all our digital pin mappings to make things easier on us.
http://arduino.cc/en/uploads/Hacking/PinMap2560big.png
Each digital port corresponds to some hardware pin which we need to identify to speed this up.
For instance, Digital Pin 53 = PB0 and its bit mask would then be 0b00000001, and PB1 would be
0b00000010, etc. This is fun.
VIM MARKS
	'D - this file
	'n - NodeStatus
	'e - entropy conditions
	'c - calibration
	'p - protocol variable
	't - time variable
*/
//#define DEBUG_MIN
//#define DEBUG_MAX
//#define DEBUG_TIME

#define DEBUG_TEMP
#ifdef DEBUG_MAX
	#define DEBUG_MIN
#endif
#define IMAGE_PATH "C:\\Users\\Jordan\\Dropbox\\GTD\\Research\\FES Project\\Code\\GUI\\Motor Unit Rotation Managed\\Debug\\images\\"
namespace Custom {
	enum DebugVars {
		NONE,
		INCREMENT_TIME,
		CALCULATE_CHARGE,
		RECRUIT_NODE,
		GET_ELAPSED_NODES,
		ENCODE_ELAPSED_PULSES,
		SET_CURRENT_VARIABLES
	};
	enum RampType {
		LINEAR,
		EXPONENTIAL
	};
	enum Shape {
		CIRCLE,
		RECTANGLE
	};
	enum NodeStatus {
		ACTIVE,
		DORMANT,
		RAMPING_UP,
		RAMPING_DOWN,
		RAMPING_ALL,
		ACTIVE_ONLY,
		CALIBRATION_SET
	};
	enum EntropyCondition {
		RECRUITING_NODE,
		DEACTIVATING_NODE
	};
	enum Property {
		X_COORDINATE,
		Y_COORDINATE,
		SHAPE,
		WIDTH,
		HEIGHT,
		INDEX,
		STATUS,
	};
	enum MVCVariable{
		WITHIN_RANGE,
		ABOVE_RANGE,
		BELOW_RANGE,
		TOLERANCE,
		DESIRED,
		CURRENT
	};
	enum ProtocolPriority{ // our priorities for recruitment and de-recruitment
		LONGEST_DORMANT,
		LONGEST_ACTIVE,
		MAXIMIZE_CHARGE_ENTROPY,
		MAXIMIZE_CHARGE_DISTANCE,
		RANDOM_SELECT
	};
	enum TimeVariable {
		CURRENT_TIME,
		TOTAL_TIME,
		SINCE_LAST_PULSE,
		ACTIVE_TIME_BOTH,
		ACTIVE_TIME_CURRENT,
		ACTIVE_TIME_TOTAL,
		DORMANT_TIME_BOTH,
		DORMANT_TIME_CURRENT,
		DORMANT_TIME_TOTAL,
		RAMPING_DOWN_CURRENT,
		TO_NEXT_PULSE
	};
	enum ChargeVariable {
		TOTAL,
		THIS_NODE_ONLY
	};
	enum ProtocolVariable{
		NOMINAL_FREQUENCY,
		MAXIMUM_FREQUENCY,
		FREQUENCY_RAMP_TIME,
		FREQUENCY_RAMP_DELAY,

		NOMINAL_AMPLITUDE,
		MAXIMUM_AMPLITUDE,
		AMPLITUDE_RAMP_TIME,
		AMPLITUDE_RAMP_DELAY,

		LONGEST_DORMANT_CONDITION,
		MAXIMIZE_CHARGE,
		RANDOM_SELECTION,

		TIME_THRESHOLD,
		CHARGE_THRESHOLD,

		PRIORITY

	};
	enum ThreadType {
		MAIN,
		CALIBRATION,
		GET_MAX_MVC
	};
	enum MotorUnitSignal {
		NO_SIGNAL,
		ANOTHER_RECRUITMENT,
		ANOTHER_DEACTIVATION
	};
	enum CalibrationCurves {
		NINE_VOLTS_FIVE_OHM,
		NINE_VOLTS_TEN_OHM,
		NINE_VOLTS_TWENTY_OHM,
		FIVE_VOLTS_FIVE_OHM,
		FIVE_VOLTS_TEN_OHM,
		FIVE_VOLTS_TWENTY_OHM
	};
}

#define CALIBRATION_CURVE FIVE_VOLTS_TEN_OHM
