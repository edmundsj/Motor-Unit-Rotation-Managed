/*
IMPORTANT DOCUMENTATION INFORMATION
Author: Jordan Louis Edmunds
License: GNU Public
Date Modified: 14 July 2015

------PROGRAM LOGIC FLOW------

The goal of this program is to autonomously change patterns of electrical stimulation sent to 
a microcontroller based on desired force output compared to achieved force output. The function
flow breaks down like this:

The threads Work::updateThread and Work::pulseThread are run asynchronously
Work::updateThread is responsible for modifying current variables, such as amplitude, frequency,
period, etc, as well as determining whether a new node needs to be recruited or deactivated.

Work::pulseThread is responsible for incrementing time variables across the main NodeArray class
and all Node subclasses.
------END LOGIC FLOW----------

Some of our important vim marks
	'b - bottom of page
	't - top of page
	'm - middle of page
	'D - defines file
	'M - motorUnit header file
	'F - myForm file
	'i - Misc Data Class
	'a - NodeArray Class
	'n - Node class
	'p - predictForce function
	'c - setCurrentVariables function
	'd - deactivate node
	'r - rotateMotorUnit function
	'v - vim settings
	'u - getUnion thingy 
	'e - extractMVCData function

*/


#include "defines.h"
#include <Windows.h>
#include <vector>

using namespace std;
using namespace System::IO::Ports;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace Custom;


//some global time variables to keep track of function CPU use.
#ifdef DEBUG_TIME
#include <chrono>
using namespace chrono;
auto start_debug_time = chrono::high_resolution_clock::now();
auto end_debug_time = chrono::high_resolution_clock::now();

unsigned long long add_time_func = 0;
unsigned long long get_nodes_func = 0;
unsigned long long increment_time_func = 0;
unsigned long long calculate_charge_distance_func = 0;
unsigned long long recruit_node_func = 0;
unsigned long long set_time_func = 0;
unsigned long long get_elapsed_nodes_func = 0;
unsigned long long encode_elapsed_pulses_func = 0;
unsigned long long set_current_variables_func = 0;


void BeginDebugTimer() {
	start_debug_time = chrono::high_resolution_clock::now();
}

long long EndDebugTimer(char type = 'u', Custom::DebugVars add_time = NONE) {
	end_debug_time = chrono::high_resolution_clock::now();
	long total_debug_time_ms = chrono::duration_cast<chrono::microseconds>(end_debug_time - start_debug_time).count();
	if (add_time != NONE) { // then we increment our variables
		switch (add_time) {
		case INCREMENT_TIME:
			increment_time_func += total_debug_time_ms;
			break;
		case CALCULATE_CHARGE:
			calculate_charge_distance_func += total_debug_time_ms;
			break;
		case RECRUIT_NODE:
			recruit_node_func += total_debug_time_ms;
			break;
		case GET_ELAPSED_NODES:
			get_elapsed_nodes_func += total_debug_time_ms;
			break;
		case ENCODE_ELAPSED_PULSES:
			encode_elapsed_pulses_func += total_debug_time_ms;
			break;
		case SET_CURRENT_VARIABLES:
			set_current_variables_func += total_debug_time_ms;
			break;
		default:
			OutputDebugMessage(L"Unable to find your time variable to add to in EndDebugTimer().\n");
		}
	}
	switch (type) {
	case 'n': {
		return chrono::duration_cast<chrono::nanoseconds>(end_debug_time - start_debug_time).count();
		break;
	}
	case 'u': {
		return chrono::duration_cast<chrono::microseconds>(end_debug_time - start_debug_time).count();
		break;
	}
	case 'm': {
		return chrono::duration_cast<chrono::milliseconds>(end_debug_time - start_debug_time).count();
		break;
	}
	case 's': {
		return chrono::duration_cast<chrono::seconds>(end_debug_time - start_debug_time).count();
		break;
	}
	default: {
		return chrono::duration_cast<chrono::milliseconds>(end_debug_time - start_debug_time).count();
		break; // lol don't need these, just need to stay in the habit of it.
	}
	}

}
#endif

void OutputDebugLong(long num) {
	TCHAR buf[256]; // where you put result
	swprintf(buf, TEXT("%d"), num);
	OutputDebugString(buf);
}

void OutputDebugMessage(LPCWSTR str, long num = -55, LPCWSTR str2 = L"") {
	OutputDebugString(str);
	if (num != -55) {
		OutputDebugLong(num);
	}
	if (str2 != L"") {
		OutputDebugString(str2);
	}
}

// gets the distance between two points as integers
float distanceFormula(int x1, int y1, int x2, int y2) {
	return (float)sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}


public ref class ThreadInfo {
private:
	Custom::ThreadType thread_to_call;
public:
	
	// choose between "main", "calibration", and perhaps in the future others.
	ThreadInfo(Custom::ThreadType type) {
		thread_to_call = type;
	}
	Custom::ThreadType getType() {
		return this->thread_to_call;
	}
};

// this encapsulates our MVC data.
public ref class MVCapsule {
public:
	List<double> timestamps;
	List<double> data;
	int start_index;
	int end_index;
	double total_data = 0;
	double average_data = 0;
};

public class MiscData {
public:
	int getPairIndex(int, int); // gets the index of the pair of two integers
	int getNodeFromIndex(int, int);
	vector<int> getUnionsContaining(vector<int>);

	vector<vector<int>> skeletal_connections;
	vector<double> redundancy_constant;	// the redundance constant R1-2
	vector<double> union_force_constant;		// the force generated by each N1 U N2 where N1 U N2 is a member of skeletal_connections
};
MiscData dat;

int MiscData::getPairIndex(int node1, int node2) {
	for (int i = 0; i < this->skeletal_connections.size(); i++) {
		if ((skeletal_connections[i][0] == node1 && skeletal_connections[i][1] == node2) ||
			(skeletal_connections[i][0] == node2 && skeletal_connections[i][1] == node1)) {
			return i;
		}
	}
}

// index is the index of skeletal_conections and node_number is 0 or 1.
int MiscData::getNodeFromIndex(int index, int node_number) {
	return skeletal_connections[index][node_number];
}

bool vectorPresent(vector<vector<int>> big_vec, vector<int> sub_vec) {
	for (vector<int> i : big_vec) {
		for (int n : i) {
			bool contains_element = false;
			for (int v : sub_vec) {
				// if any one of the elements is not equal, return false
				if (v == n) {
					contains_element = true;
				}
			}
			if (contains_element == false) {
				return false;
			}
		}
	}
	return true;
}

vector<int> MiscData::getUnionsContaining(vector<int> node_set) {
	vector<int> unions_to_return;
	// loop through each skeletal connection
	for (int i = 0; i < this->skeletal_connections.size(); i++) {
		bool already_present = false;
		int matched_nodes = 0;
		// loop through each individual node in our node_set combinatorially, so 0, 0; 0, 1; 0, 2; 1, 0; etc.
		for (int sub_node1 : node_set) {
			for (int sub_node2 : node_set) {
				// loop through each node in the skeletal connection
				for (int matchee : skeletal_connections[i]) {
					// if our skeletal connection node is equal to any of our selected nodes, we have a match!
					if (matchee == sub_node1 || matchee == sub_node2) {
						matched_nodes++;
						// if we have two matches, cool, we need to add it
						if (matched_nodes == 2) {
							// but only if it isn't already there!
							if (already_present == false) {
								unions_to_return.push_back(i);
								already_present = true;
							}
							break;
						}
					}
					if (already_present) break;
				}
				matched_nodes = 0;
				if (already_present) break;
			}
			// if any two sub_nodes are identical to 
			if (already_present) break;
		}
	}
	return unions_to_return;
}

public ref class Epoch {
public:
	double start_time;
	double end_time;
	int start_index;
	int end_index;
	int identifier;
	Epoch(double, double, int, int);
	Epoch(double, double, int, int, int);
};

Epoch::Epoch(double s, double e, int i_s, int i_e) {
	this->start_time = s;
	this->end_time = e;
	this->start_index = i_s;
	this->end_index = i_e;
}

Epoch::Epoch(double s, double e, int i_s, int i_e, int ident) {
	this->start_time = s;
	this->end_time = e;
	this->start_index = i_s;
	this->end_index = i_e;
	this->identifier = ident;
}

public ref class NodeArray {			// the superclass that we run the show from
public:
	NodeArray();					// our constructor, takes no arguments.
	ref class Node;					// the basis of all our stimulation
									// SETUP FUNCTIONS, including our all-important calibration function
	void addNode(int ind, int x_coor, 
		int y_coor, int shape,			// adds a node with the x coordinate, y coordinate, shape, radius/width and height
		int width, int height);	
	void destroyAllNodes();				// destroys all our nodes, yo
	void setConnectedNodes();			// connects our nodes and places the connections in this class and our MiscData class
	double predictForce(vector<int>);	// predicts the force based on a vector of the indices of all recruited nodes
	void calibrateForce();				// calibrates our node_force_constant vector with individual node forces

									// REAL-TIME FUNCTIONS
	void incrementTime(System::Diagnostics::Stopwatch^);			// the bread and butter of our timekeeping system.
	void setCurrentVariables();					// assigns current variables including amplitude and frequency based on the current time.
	int checkMVC();						// are we within our desired MVC range? If so, return true.
	double modifyMVC(int,
		Custom::CalibrationCurves, bool);		// modifies our MVC data from the arduino according to the calibration curve we need.
	MVCapsule^ extractMVCData(double, double);

	bool checkRampingNodes();		// checks to see if we have any nodes that are currently ramping up or down

	void recruitNode();					// autonomously recruits new node
	void recruitNode(vector<int>);		// recruits new nodes explicitly by their indices
	double recruitNode(bool);			// recruits a node close to the node_force of another active node that can be brought down.

	void deactivateNode();
	void deactivateNode(vector<int>);	// overloaded version of deactivateNode() specifying the nodes explicitly.
	Custom::MotorUnitSignal deactivateNode(double);		// deactivates a node close to node_force double.
	
	bool checkUnitRotation();						// checks to see if we need to rotate to a new motor unit
	void rotateMotorUnit();							// deactivates one node and activates another
	void rotateMotorUnit(int);						// overload +1, specifies the number of nodes to bring up/down
	void rotateMotorUnit(vector<int>, vector<int>); // overload +2, specifies the vectors explicitly.

	float calculateChargeDistance(int, 
		Custom::ChargeVariable);					// calculates the charge distance between the node with index int and all other active nodes.
	int calculateChargeEntropy(int, 
		Custom::EntropyCondition);
	

	vector<int> getElapsedNodes(vector<int>);		// gets which nodes we need to pulse
	unsigned long encodeElapsedPulses();			// encodes all the pulses that have elapsed in the given time period.
	void sendData(SerialPort ^ port, String^ data);
	// sends data over our serial port
	String^ encodeAmplitudes();
	String^ getLastAmplitude();

	void pictureBoxToNode(
		PictureBox^, int);							// resizes and moves the picture box to the node with index int.
	void updateNodeColors(List<PictureBox^>^);
	void updateNodeInformation(TextBox^, short);	// updates the text-field based node information for the user

										// GETTER AND SETTER FUNCTIONS
	vector<int> getNodes(Custom::NodeStatus);	// gets a vector of the node indices we want.
	double getRunningTime();			// not as important but w/e
	void setRunningTime(double);	
	float getMVC(Custom::MVCVariable);	// gets the current MVC 
	void setMVC(double);				// sets the current MVC	

	void reset();
	void clearData();
	
	List<Node^> all_nodes;			// all the nodes available 

//private:
	
	// this class encapsulates all our Node_Array data
	ref class Data {
	public:

		// CALIBRATION VARIABLES
		List<float> node_force_constant = {};			// the weights we will use for funcion Q
		float redudancy_constant_1 = 0;
		List<float> fatigue_function = {};		// where each element in the list is sampled at 0.1s and represents a % of maximum force potential
		List<double> updated_timestamps = {};

		List<double> mvc_data = {};				// our MVC data uses a different timestamp system than the 
		List<double> predicted_mvc = {};		// the force our model predicts 
		List<double> mvc_timestamps = {};		// node amplitudes, they are irreconcilable.
		
		List<double> calibration_mvc_data = {};
		List<double> calibration_predicted_mvc = {};
		List<double> calibration_mvc_timestamps = {};
		String^ last_mvc_file = "";
		String^ last_stim_file = "";
	};
	
	// this class encapsulates all our time-related variables
	ref class Time { // TIME VARIABLES
	public:
		double recent_time_elapsed = 0;
		double old_time = 0;
		double new_time = 0;
		double running_time = 0;		// how long our NodeArray has been active
		double time_since_last_amplitude = 0; // not currently used
		System::Diagnostics::Stopwatch^ watch;
	};

	ref class Configuration {
	public:
		List<int> recruitment_priorities;	// these are our recruitment priorities [0] is the highest
		List<int> deactivation_priorities;	// these are our deactivation priorities
		List<List<int>^> connected_nodes;	// connected_nodes[0][i] represents all nodes that node [0] is connected to
		List<List<int>^> connected_nodes_skeletal;

		int amplitude_nominal;			// the initial amplitude (out of 255)
		int amplitude_maximum;			// the maximum amplitude
		double amplitude_ramp_time;		// the ramp time to get to the maximum amplitude
		double amplitude_ramp_offset;	// the delay between initial and the ramp beginning
		int amplitude_ramp_type;		// linear, exponential, what?
										// FREQUENCY VARIABLES
		int frequency_nominal;			// the initial frequency, in Hz
		int frequency_maximum;			// the maximum frequency
		double frequency_ramp_time;		// the ramp time to get to the maximum frequency
		double frequency_ramp_offset;	// the delay between initial and the ramp beginning
		int frequency_ramp_type;		// linear, exponential, what?

		double time_threshold;			// the time after which a node becomes eligible for rotation
		int charge_threshold;			// the amount of charge after which a node becomes eligible for rotation, in Amplitude pulses

		int amplitude_frequency = 50;	// how many times per second do we send our amplitudes per second?
		double amplitude_period = 1.0 / amplitude_frequency;
	};
										// RECRUITMENT PRIORITY VARIABLES
	
									// MICROCONTROLLER AND MVC VARIABLES
	bool connected_to_MC = false;	// are we connected to the microcontroller?
	bool initialized_MC = false;	// have we initialized our variables? if so, this should be true.
	int tolerance_mvc = 10;			// the percent tolerance between our current and desired MVC
	int desired_mvc = 50;			// the percent MVC that is desired
	float current_mvc = 50;			// the current MVC, this should be updated as needed.
	double max_MVC = 1;				// our maximum MVC, in k
									// the load cell voltage can provide, hopefully 1023.
	bool send_amplitudes;			// do we need to send our amplitudes?							
									// STIMULATION PROTOCOL VARIABLES
		
	
	String^ last_amp_data = "";		// what was the last amplitude data sent?

	Data^ data = gcnew Data();
	Time^ time = gcnew Time();
	Configuration^ config = gcnew Configuration();

// our threading shennanigans
public:
	System::IO::Ports::SerialPort^ arduino_port;
	void updateThread(System::Object^);
	void pulseThread(System::Object^);
	void InitializeThreading();	// initializes all objects for use
	void DestroyThreading();		// destroys all objects

	// this gets the data from the arduino and reads it into the variable we want.
private: System::Void arduino_port_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
	try {
		if (arduino_port->IsOpen) {
			int mvc_temp = System::Convert::ToDouble(arduino_port->ReadLine()); // this is the raw MVC data, we will need to modify it at a later time.	
			// here we need to modify the number to give us something meaningful
			this->setMVC(this->modifyMVC(mvc_temp, CALIBRATION_CURVE, false));
			this->data->mvc_data.Add(this->modifyMVC(mvc_temp, CALIBRATION_CURVE, false));
			this->data->mvc_timestamps.Add(this->time->running_time);
			this->data->predicted_mvc.Add(this->predictForce(this->getNodes(ACTIVE)));
		}
	}
	catch (System::FormatException^ form_e) {
#ifdef DEBUG_MAX
		OutputDebugMessage(L"The arduino sent over some weird junk. Ignoring.");
#endif
	}
	catch (System::IO::IOException^ io_ex) {
#ifdef DEBUG_MAX
		OutputDebugMessage(L"We must have exited while receiving data. Awk.\n");
#endif
	}
	catch (System::OverflowException^ o_ex) {
#ifdef DEBUG_ERROR
		OutputDebugMessage(L"The arduino is trying to beat us to death with data.\n")
#endif
	}
}
};

ref class NodeArray::Node {
public:
	NodeArray^ parent;
	Node(int, int, int, int, int, int, NodeArray^);	// the constructor, takes the index as an argument

	int getProperty(Custom::Property);	// returns the value of any property including:
										// index, status, shape, width, height, x coordinate, y coordinate
										// THESE GET THEIR OWN GETTERS AND SETTERS CAUSE 
										// THEY NEED TO BE VERY OBVIOUS AND EASY
	double getPeriod();					// gets the current period of this node
	void setPeriod(double);				// sets the current period of this node
	int getFrequency();					// gets the current frequency
	void setFrequency(int);				// sets the current frequency to an integer value
	int getAmplitude();					// gets the current amplitude
	void setAmplitude(int);				// sets the current amplitude to an integer value
	int getIndex();						// returns the index (the channel) value of the node

	void setStatus(Custom::NodeStatus);
	Custom::NodeStatus getStatus();		// gets the current node status

	void addTime(Custom::TimeVariable, double);
	void setTime(Custom::TimeVariable, double);
	double getTime(Custom::TimeVariable);
	void resetTime();
	void resetToZero();
	
	// this stores the data we will use for analysis purposes
	ref class Data {
	public:
		List<short> updated_amplitudes; // these variables correspond to the timestamps stored in the parent NodeArray.
		List<short> updated_frequencies;
	};
	Data^ data = gcnew Data();

	double node_force_constant;					// the MVC this node is capable of generating
private:
													// THE CURRENT STATUS VARIABLES
	int amplitude_current;							// the current amplitude
	int frequency_current;							// the current frequency, initially set to the initial frequency#
	double period_current = 9999999;				// the current period, in seconds.											// STATUS VARIABLES
	
	int index;										// the vector index that a node will be referred to by.
	int x_coor;										// the x-coordinate on the surface of the skin
	int y_coor;										// the y-coordinate on the surface of the skin
	int shape;
	int width; 
	int height;

	Custom::NodeStatus status = Custom::NodeStatus::DORMANT;	// is the node active, dormant, never activated, ramping up, or ramping down?
	
	double active_time_total = 0;					// total running active time
	double active_time_current = 0;					// total time this node has been active, if currently active
	double dormant_time_total = 0;					// total time this node has been dormant
	double dormant_time_current = 0;				// total time this node has been dormant, if currently dormant
	double time_since_last_pulse = 0;				// the time since the last pulse happened. Initially set to zero
	double time_ramping_down_current = 0;
	
};

// our Node constructor, takes 6 arguments.
NodeArray::Node::Node(int ind, int x_co, int y_co, int sh, int wid, int hei, NodeArray^ p)
{
	this->index = ind;
	this->x_coor = x_co;
	this->y_coor = y_co;
	this->shape = sh;
	this->width = wid;
	this->height = hei;
	this->parent = p;
}

// gets any integer property of a node. Deprecated and should be removed.
int NodeArray::Node::getProperty(Custom::Property prop) {
#ifdef DEBUG_TIME
	BeginDebugTimer()
#endif
#ifdef DEBUG_ARE_YOU_FUCKING_INSANE
	OutputDebugMessage(L"Beginning getProperty() function...\n");
#endif
	switch (prop) {
	case X_COORDINATE: {
		return this->x_coor;
		break;
	}
	case Y_COORDINATE: {
		return this->y_coor;
		break;
	}
	case STATUS: {
		return this->status;
		break;
	}
	case SHAPE: {
		return this->shape;
		break;
	}
	case WIDTH: {
		return this->width;
		break;
	}
	case HEIGHT: {
		return this->height;
		break;
	}
	default:
		break;
	}
#ifdef DEBUG_MAX
	OutputDebugMessage(L"\tCould not find the property you requested. Terminating function with -1 value.\n");
#endif
	return -1;
#ifdef DEBUG_TIME
	OutputDebugMessage(L"getProperty() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}

// gets the real-world channel index of the node
int NodeArray::Node::getIndex() {
	return this->index;
}

// gets our current period of a node.
double NodeArray::Node::getPeriod() {
	return this->period_current;
}

// sets the period_current variable
void NodeArray::Node::setPeriod(double period) {
	this->period_current = period;
}

// gets the current frequency of a node
int NodeArray::Node::getFrequency() {
	return this->frequency_current;
}

// sets the current frequency of a node
void NodeArray::Node::setFrequency(int freq) {
	this->frequency_current = freq;
}

// gets the current amplitude on a node
int NodeArray::Node::getAmplitude() {
	return this->amplitude_current;
}

// sets the current amplitude on a node
void NodeArray::Node::setAmplitude(int ampl) {
	this->amplitude_current = ampl;
}

// sets the time of a node time variable (see defines.h for available types)
void NodeArray::Node::setTime(Custom::TimeVariable type, double time) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	switch (type) {
		// set the time since the last pulse has occured on this node
	case SINCE_LAST_PULSE: {
		this->time_since_last_pulse = time;
		break;
	}
		// set both the current and total active time
	case ACTIVE_TIME_BOTH: { 
		this->active_time_total = time;
		this->active_time_current = time;
		break;
	}
		// set only the current active time
	case ACTIVE_TIME_CURRENT: {
		this->active_time_current = time;
		break;
	}
		// set only the total active time
	case ACTIVE_TIME_TOTAL: {
		this->active_time_total = time;
		break;
	}
		// set both the current and total dormant times on this node
	case DORMANT_TIME_BOTH: {
		this->dormant_time_current = time;
		this->dormant_time_total = time;
		break;
	}
		// set only the current dormant time
	case DORMANT_TIME_CURRENT: {
		this->dormant_time_current = time;
		break;
	}
		// set only the total dormant time
	case DORMANT_TIME_TOTAL: {
		this->dormant_time_total = time;
		break;
	}
	default:
#ifdef DEBUG_ERROR
		OutputDebugMessage(L"Unable to find the requested variable in setTime() function\n");
#endif
		break;
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"setTime() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}

// adds time to a node time variable
void NodeArray::Node::addTime(Custom::TimeVariable type, double time) {
#ifdef DEBUG_TIME
	BeginDebugTimer()
#endif
	switch (type) {
	case SINCE_LAST_PULSE: {
		this->time_since_last_pulse += time;
		break;
	}
	case ACTIVE_TIME_BOTH: { // here we want to increment all active time variables
		this->active_time_total += time;
		this->active_time_current += time;
		break;
	}
	case ACTIVE_TIME_CURRENT: {
		this->active_time_current += time;
		break;
	}
	case ACTIVE_TIME_TOTAL: {
		this->active_time_total += time;
		break;
	}
	case DORMANT_TIME_BOTH: {
		this->dormant_time_current += time;
		this->dormant_time_total += time;
		break;
	}
	case DORMANT_TIME_CURRENT: {
		this->dormant_time_current += time;
		break;
	}
	case DORMANT_TIME_TOTAL: {
		this->dormant_time_total += time;
		break;
	}
	case RAMPING_DOWN_CURRENT: {
		this->time_ramping_down_current += time;
		break;
	}
	default:
#ifdef DEBUG_MIN
		OutputDebugMessage(L"Unable to add time to your requested variable. Variable not found.\n");
#endif
		break;
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"addTime() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}

// gets the time of a node time variable
double NodeArray::Node::getTime(Custom::TimeVariable type) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	switch (type) {
	case SINCE_LAST_PULSE: {
		return this->time_since_last_pulse;
	}
	case ACTIVE_TIME_CURRENT: {
		return this->active_time_current;
	}
	case ACTIVE_TIME_TOTAL: {
		return this->active_time_total;
	}
	case DORMANT_TIME_CURRENT: {
		return this->dormant_time_current;
	}
	case DORMANT_TIME_TOTAL: {
		return this->dormant_time_total;
	}
	case RAMPING_DOWN_CURRENT: {
		return this->time_ramping_down_current;
	}
	case TO_NEXT_PULSE: {
		return 0; // a null value, haven't coded this yet
	}
	default:
#ifdef DEBUG_ERRORS
		OutputDebugMessage(L"In function getTime() unable to find value to return.\n");
#endif
		return 0; // also a null value, haven't coded.
		break;
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"getTime() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}
// sets all the time variables to zero.
void NodeArray::Node::resetTime() {
	this->active_time_current = 0;
	this->active_time_total = 0;
	this->dormant_time_current = 0;
	this->dormant_time_total = 0;
	this->time_ramping_down_current = 0;
	this->time_since_last_pulse = 0;
}

void NodeArray::Node::resetToZero() {
	this->resetTime();
	this->amplitude_current = 0;
	this->frequency_current = 0;
	this->period_current = 999999;
	this->status = DORMANT;
}

void NodeArray::Node::setStatus(NodeStatus stat) {
	this->status = stat;
}

Custom::NodeStatus NodeArray::Node::getStatus() {
	return this->status;
}

NodeArray::NodeArray() {
	// constructor code goes here
}

void NodeArray::addNode(int ind, int x_co, int y_co, int shape, int width, int height) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MAX
	OutputDebugMessage(L"Beginning addNode() function...\n");
#endif
	this->data->node_force_constant.Add(0); // initialize our node_force_constant vector to the size of however many nodes we are adding.
	Node^ temp_node = gcnew Node(ind, x_co, y_co, shape, width, height, this);
	this->all_nodes.Add(temp_node);
#ifdef DEBUG_TIME
	OutputDebugMessage(L"addNode() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}

void NodeArray::destroyAllNodes() {
	this->all_nodes.Clear();
}

// this member function calibrates the force of each node and writes it into
// node_force_constant based on the data currently in the Data classes of Node and NodeArray.
void NodeArray::calibrateForce() {
	// first we need to extract the MVC data during the epoch where each node was activated individually
	List<Epoch^> individual_epochs;
	for (int i = 0; i < this->all_nodes.Count; i++) {
		// now we need to loop through and find where this node reaches it's maximum amplitude and frequency 
		// for the first time
		int reached_max_amplitude_index = 0;
		int reached_max_frequency_index = 0;
		int reached_max_combo = 0;
		for (int a = 0; a < this->all_nodes[i]->data->updated_amplitudes.Count; a++) {
			if (this->all_nodes[i]->data->updated_amplitudes[a] == this->config->amplitude_maximum) {
				reached_max_amplitude_index = a;
				break;
			}
		}
		// find when we first hit our maximum amplitude
		for (int f = 0; f < this->all_nodes[i]->data->updated_frequencies.Count; f++) {
			if (this->all_nodes[i]->data->updated_frequencies[f] == this->config->frequency_maximum) {
				reached_max_frequency_index = f;
				break;
			}
		}
		if (reached_max_amplitude_index >= reached_max_frequency_index) {
			reached_max_combo = reached_max_amplitude_index;
		}
		else {
			reached_max_combo = reached_max_frequency_index;
		}
		// set the temporary epoch to have the system's timestamps and the system's timestamp indices
		Epoch^ temp_epoch = gcnew Epoch(this->data->updated_timestamps[reached_max_combo], this->data->updated_timestamps[reached_max_combo+1],
			reached_max_combo, reached_max_combo+1, i);
		individual_epochs.Add(temp_epoch);
	}

	List<Epoch^> combinatorial_epochs;
	// now we need to extract the MVC data during the epoch where each node was activated as a pair.
	for (int i = 0; i < dat.skeletal_connections.size(); i++) {
		int reached_max_wombo_combo = 0;
		// loop through each time point and check if both of our amplitudes and frequencies are at max on both nodes.
		// This should only happen once.
		for (int t = 0; t < this->data->updated_timestamps.Count; t++) {
			if (this->all_nodes[dat.skeletal_connections[i][0]]->data->updated_frequencies[t] == this->config->frequency_maximum &&
				this->all_nodes[dat.skeletal_connections[i][0]]->data->updated_amplitudes[t] == this->config->amplitude_maximum &&
				this->all_nodes[dat.skeletal_connections[i][1]]->data->updated_frequencies[t] == this->config->frequency_maximum &&
				this->all_nodes[dat.skeletal_connections[i][1]]->data->updated_amplitudes[t] == this->config->amplitude_maximum) {
				// all of the conditions have been met.
				reached_max_wombo_combo = t;
				break;
			}
		}
		Epoch^ temp_epoch = gcnew Epoch(this->data->updated_timestamps[reached_max_wombo_combo], this->data->updated_timestamps[reached_max_wombo_combo+1],
			reached_max_wombo_combo, reached_max_wombo_combo+1, i);
		combinatorial_epochs.Add(temp_epoch);
	}
	// now we get the force for both our individual and combinatorial epochs.
	// initialize both lists of force capsules to the size of their corresponding epochs
	String^ message = "";
	List<MVCapsule^> individual_mvcapsule;
	List<MVCapsule^> combinatorial_mvcapsule;
	for (int i = 0; i < individual_epochs.Count; i++) {
		individual_mvcapsule.Add(this->extractMVCData(individual_epochs[i]->start_time, individual_epochs[i]->end_time));
		this->all_nodes[i]->node_force_constant = individual_mvcapsule[i]->average_data;
	}
	for (int i = 0; i < combinatorial_epochs.Count; i++) {
		combinatorial_mvcapsule.Add(this->extractMVCData(combinatorial_epochs[i]->start_time, combinatorial_epochs[i]->end_time));
		dat.union_force_constant.push_back(combinatorial_mvcapsule[i]->average_data);
		dat.redundancy_constant.push_back((this->all_nodes[dat.getNodeFromIndex(i, 0)]->node_force_constant + 
			this->all_nodes[dat.getNodeFromIndex(i, 1)]->node_force_constant - 
			combinatorial_mvcapsule[i]->average_data) / combinatorial_mvcapsule[i]->average_data);
		message += "For combination " + i.ToString() + "\n\tUnion Constant: " + dat.union_force_constant[i].ToString() +
			"\n\tRedundancy Constant: " + dat.redundancy_constant[i].ToString() + "\n";
	}

	MessageBox::Show(message, "Calibration complete!");
	// now we have all the force data, and we need to try and find the nonlinear and linear components.
	// now we need to move all the data we just got into the "calibration" section
	for (int i = 0; i < this->data->mvc_data.Count; i++) {
		this->data->calibration_mvc_data.Add(this->data->mvc_data[i]);
		this->data->calibration_mvc_timestamps.Add(this->data->mvc_timestamps[i]);
		this->data->calibration_predicted_mvc.Add(this->data->predicted_mvc[i]);
	}
	this->data->mvc_data.Clear();
	this->data->mvc_timestamps.Clear();
	this->data->predicted_mvc.Clear();
}

// predicts what the mvc will be if we use the nodes in node_vector.
// currently only uses a linear operator.
double NodeArray::predictForce(vector<int> node_vector) {
	double result = 0;
	// first we add all of the individual node_force_constants
	for (int i = 0; i < node_vector.size(); i++) {
		result += this->all_nodes[node_vector[i]]->node_force_constant;
	}

	// then we subtract all of the union_force_constants for all the skeletal connections we have
	vector<int> unions_to_subtract = dat.getUnionsContaining(node_vector);
	for (int i = 0; i < unions_to_subtract.size(); i++) {
		result -= dat.redundancy_constant[i] * dat.union_force_constant[i];
	}
	return result;
}

// this is one of the critical functions, increments time on all relevant variables.
void NodeArray::incrementTime(System::Diagnostics::Stopwatch^ w) {

#ifdef DEBUG_MAX
	OutputDebugMessage(L"Beginning incrementTime() function...\n");
#endif
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	this->time->new_time = System::Convert::ToDouble(w->Elapsed.TotalSeconds);

	this->time->recent_time_elapsed = this->time->new_time - this->time->old_time;
	this->time->old_time = this->time->new_time;
	
	this->time->running_time = this->time->new_time;
	this->time->time_since_last_amplitude += this->time->recent_time_elapsed;

	for (auto i : getNodes(ACTIVE)) { // increment the relevant time variables on all our active nodes.
		this->all_nodes[i]->addTime(SINCE_LAST_PULSE, this->time->recent_time_elapsed);
		this->all_nodes[i]->addTime(ACTIVE_TIME_BOTH, this->time->recent_time_elapsed);
	}
	// THE BELOW CODE TAKES > 50% OF OUR CPU TIME OF THE ENTIRE PROGRAM
	for (auto i : getNodes(DORMANT)) { // increment the relevant time variables on all our dormant nodes.
		this->all_nodes[i]->addTime(DORMANT_TIME_BOTH, this->time->recent_time_elapsed);
	}
	// WTF IS WRONG WITH THIS CODE

	for (auto i : getNodes(RAMPING_DOWN)) { // increment the relevant time variables on all our dormant nodes.
		this->all_nodes[i]->addTime(RAMPING_DOWN_CURRENT, this->time->recent_time_elapsed);
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"incrementTime() took ", , L"us to complete. \n");
#endif
}

// this is just a tiny bit important... Kinda does everything that timekeeping does not.
// recruitNodes handles the activation/deactivation/ramping up/down of new nodes,
// all this function has to do is set the variables on all nodes tapped for activation
// also, this function should set the status of all nodes finished ramping down to "DORMANT"
void NodeArray::setCurrentVariables() {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	// if the time since our last amplitude is greater than the period, send the new amplitudes.
	if (this->time->time_since_last_amplitude > this->config->amplitude_period) {
		this->send_amplitudes = true;
	}
	for (auto i : this->getNodes(RAMPING_UP)) { // sets the variables of all nodes ramping up

												// this ugly-looking piece of code calculates the current frequency based
												// on the time elapsed past the frequency ramp delay, ramping linearly.
												// we add the +1 so our integers do not get truncated down.
		if ((this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) <= (this->config->frequency_ramp_time + this->config->frequency_ramp_offset)) &&
			this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) > this->config->frequency_ramp_offset) {
			this->all_nodes[i]->setFrequency(
				this->config->frequency_nominal + 1.0 +
				(((this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) - this->config->frequency_ramp_offset) /
					(this->config->frequency_ramp_time)) *
					(this->config->frequency_maximum - this->config->frequency_nominal)));
			this->all_nodes[i]->setPeriod(1.0 / ((double)(this->all_nodes[i]->getFrequency())));

		}

		// ... and this takes care of the amplitude.
		// here we add the +1 but need to check 
		if ((this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) <= (this->config->amplitude_ramp_time + this->config->amplitude_ramp_offset)) &&
			this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) > this->config->amplitude_ramp_offset) {
			this->all_nodes[i]->setAmplitude(
				((this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) - this->config->amplitude_ramp_offset) /
					(this->config->amplitude_ramp_time)) *
				(this->config->amplitude_maximum - this->config->amplitude_nominal));
		}

		// here we check to see if we have exceeded the ramp times, and if so we set
		// the node in question to "active".
		if ((this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) > (this->config->amplitude_ramp_offset + this->config->amplitude_ramp_time)) &&
			this->all_nodes[i]->getTime(ACTIVE_TIME_CURRENT) > (this->config->frequency_ramp_offset + this->config->frequency_ramp_time)) { // here we check and see if we are finished ramping up
			this->all_nodes[i]->setStatus(ACTIVE);
			this->all_nodes[i]->setFrequency(this->config->frequency_maximum);
			this->all_nodes[i]->setAmplitude(this->config->amplitude_maximum);
		}

	}
	for (auto i : this->getNodes(RAMPING_DOWN)) { // stes the variables of all nodes ramping down
		 // something is wrong with this function. let's run some tests.
		
		if (this->all_nodes[i]->getTime(RAMPING_DOWN_CURRENT) <= (this->config->frequency_ramp_time)) {
			this->all_nodes[i]->setFrequency(
				((this->config->frequency_ramp_time - this->all_nodes[i]->getTime(RAMPING_DOWN_CURRENT)) /
				(this->config->frequency_ramp_time)) *
				(this->config->frequency_maximum));
			if (this->all_nodes[i]->getFrequency() > 0) {
				this->all_nodes[i]->setPeriod(1.0 / ((double)(this->all_nodes[i]->getFrequency())));
			}
		}

		// we bring down the frequency and amplitude at the same time.
		if ((this->all_nodes[i]->getTime(RAMPING_DOWN_CURRENT) <= (this->config->amplitude_ramp_time))) {
			this->all_nodes[i]->setAmplitude((int)
				(((this->config->amplitude_ramp_time - this->all_nodes[i]->getTime(RAMPING_DOWN_CURRENT)) / (this->config->amplitude_ramp_time)) *
				(this->config->amplitude_maximum - this->config->amplitude_nominal)));
		}
		if((this->all_nodes[i]->getTime(RAMPING_DOWN_CURRENT) > this->config->frequency_ramp_time) &&
			(this->all_nodes[i]->getTime(RAMPING_DOWN_CURRENT) > this->config->amplitude_ramp_time)) { // then set these nodes status to dormant
			this->all_nodes[i]->setTime(RAMPING_DOWN_CURRENT, 0);
			this->all_nodes[i]->setAmplitude(0);
			this->all_nodes[i]->setFrequency(0);
			this->all_nodes[i]->setTime(ACTIVE_TIME_CURRENT, 0);
			this->all_nodes[i]->setPeriod(9999999); // "infinity"
			this->all_nodes[i]->setStatus(DORMANT);
		}
	}
	// now we set the nodes finished ramping down to "dormant".
#ifdef DEBUG_TIME
	OutputDebugMessage(L"setCurrentVariables() took ", BeginDebugTimer() , L"us to complete. \n");
#endif
}

// if any of our nodes are ramping up or down we want to know.
bool NodeArray::checkRampingNodes() {

#ifdef DEBUG_MAX
	OutputDebugMessage(L"Entering the checkRampingNodes() function...\n");
#endif
	for (int i = 0; i < this->all_nodes.Count; i++) {
		if (this->all_nodes[i]->getStatus() == RAMPING_UP || this->all_nodes[i]->getStatus() == RAMPING_DOWN) {
			return true;
		}
	}
	return false;
}

// checks if the MVC is within the desired range, returns true if yes and false if no.
int NodeArray::checkMVC() {
#ifdef DEBUG_MAX
	OutputDebugMessage(L"Beginning checkMVC() function...\n");
#endif

	if ((this->current_mvc < (this->desired_mvc + this->tolerance_mvc)) && // if our desired MVC is within the "padding"
		(this->current_mvc >(this->desired_mvc - this->tolerance_mvc))) {  // of our tolerance, return true.
		return WITHIN_RANGE;
	}
	else if (this->current_mvc <= this->desired_mvc - this->tolerance_mvc) {
		return BELOW_RANGE;
	}
	else
		return ABOVE_RANGE;
}

// this function ramps down a node, the opposite of recruitment
// this is the non-overloaded version
void NodeArray::deactivateNode(vector<int> selected_nodes) {
	for (int i = 0; i < selected_nodes.size(); i++) {
		this->all_nodes[selected_nodes[i]]->setStatus(RAMPING_DOWN);
	}
}

// recruits a node within the specified force.
Custom::MotorUnitSignal NodeArray::deactivateNode(double force) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MIN
	OutputDebugMessage(L"Beginning deactivateNode() function...\n");
#endif
	Custom::MotorUnitSignal signal = NO_SIGNAL;
	double max_variable = 0;	// the variable we use to determine the "winner".
	double min_variable = 0;	// also used to determine the "winner", for different cases.
	double new_variable = 0;
	vector<int> chosen_node_indices;
	int counter = 0;
	vector<int> eligible_nodes = this->getNodes(ACTIVE); // gets all active nodes
		
	int too_high, too_low;
	for (auto n : eligible_nodes) {
		// if the node isn't within the range we want, remove it.
		if (!((this->data->node_force_constant[n] <= force + 0.5) &&
			(this->data->node_force_constant[n] >= (force - 0.5)))) {
			// now we need to figure out which condition it was that failed us.
			if (!(this->data->node_force_constant[n] <= force + 0.5)) {
				too_high++;
			}
			if (!(this->data->node_force_constant[n] >= force - 0.5)) {
				too_low++;
			}
			eligible_nodes.erase(eligible_nodes.begin()+ n);
		}
	}
	// if we have no eligible nodes left, set the return value to what we need to do next.
	if (eligible_nodes.size() == 0) {
		eligible_nodes = this->getNodes(ACTIVE);
		// if all the potential nodes we could deactivate had a force to high we need to recruit another node afterwards.
		if (too_low == 0 && too_high > 0) {
			signal = ANOTHER_RECRUITMENT;
		}
		// if all the potential nodes we could deactivate had a force too low we need to deactivate another node after.
		else if (too_low > 0 && too_high == 0) {
			signal = ANOTHER_DEACTIVATION;
		}
		// if all nodes were "too intermediate", that is just idiotic. Do nothing.
		// at some point we need to ensure that we then actually deactivate 
		else if (too_low > 0 && too_high > 0) {
			
		}
	}
	for (int i = 0; i < this->config->deactivation_priorities.Count; i++) {

		// after each iteration we should check the size of our chosen_node_indices and see if we need to proceed.
		switch (this->config->deactivation_priorities[i]) {
		case LONGEST_ACTIVE: {
			max_variable = 0;
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tEntering the Longest Active Condition...\n");
#endif
			for (auto n : eligible_nodes) { // this deactivates the longest-running active node
				new_variable = this->all_nodes[n]->getTime(ACTIVE_TIME_TOTAL);
				if ((new_variable >= max_variable) && (this->all_nodes[n]->getTime(ACTIVE_TIME_TOTAL) > 0)) {
					// this allows us to clear the entire array if we have found the best-yet variable
					if (new_variable > max_variable) {
#ifdef DEBUG_MIN
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						chosen_node_indices.clear();

					}
					max_variable = new_variable;
					chosen_node_indices.push_back(n);
#ifdef DEBUG_MIN
					OutputDebugMessage(L"\t\tPushing back index ", n, L"to chosen_node_indices.\n");
#endif
				}
			}
			break; // break out of the switch statement
		}

							 // this will be done by comparing adjacent ACTIVE and DORMANT nodes which are adjacent, and requires
							 // a vector to tell us which nodes are adjacent (or partially adjacent...?)
		case MAXIMIZE_CHARGE_ENTROPY: {
			max_variable = 0; // since we are trying to maximize entropy anything is higher than 0
			for (auto n : eligible_nodes) {
				new_variable = calculateChargeEntropy(n, DEACTIVATING_NODE); // gets the charge entropy based on currently active nodes

				if (new_variable >= max_variable) {
					if (new_variable > max_variable) {
#ifdef DEBUG_MIN
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						chosen_node_indices.clear();

					}
					max_variable = new_variable;
					chosen_node_indices.push_back(n);
				}
			}
			break;
		}

		case MAXIMIZE_CHARGE_DISTANCE: {
			max_variable = 99999999;
#ifdef DEBUG_IN
			OutputDebugMessage(L"\tEntering the Maximize Charge Distance Condition...\n");
#endif
			for (auto n : eligible_nodes) {
				new_variable = this->calculateChargeDistance(n, THIS_NODE_ONLY); // will keeping this node maximize distance?
				if (new_variable <= max_variable) {

					if (new_variable < max_variable) {
#ifdef DEBUG_MIN
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						chosen_node_indices.clear();

					}
					max_variable = new_variable;
#ifdef DEBUG_MIN
					OutputDebugMessage(L"\t\tPushing back index ", n, L" to chosen_node_indices.\n");
#endif
					chosen_node_indices.push_back(n);
				}
				break;

			}
		}
		case RANDOM_SELECT: {
			if (chosen_node_indices.size() > 0) {
				int temp_node = chosen_node_indices[0];
				chosen_node_indices.clear();
				chosen_node_indices.push_back(temp_node);
			}


			break;
		}
		}
		if (chosen_node_indices.size() == 1) {
			this->all_nodes[chosen_node_indices[0]]->setStatus(RAMPING_DOWN); // this deactivates our new node.
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tSuccessfully deactivated the node with index ", chosen_node_indices[0], L".\n");
#endif
			break; // if after one iteration we have only one node added then break out of this function.
		}
		// sets the nodes we have to choose from (eligible_nodes) to the nodes the last (more important) condition selected
		else if (chosen_node_indices.size() > 1) {
			eligible_nodes = chosen_node_indices;
		}
	}
	if (chosen_node_indices.size() == 0) {
#ifdef DEBUG_MIN
		OutputDebugMessage(L"\tThere are no eligible nodes to recruit.\n");
#endif
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"deactivateNode() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	return NO_SIGNAL;
}

// the overloaded version with no parameters
void NodeArray::deactivateNode() {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MIN
	OutputDebugMessage(L"Beginning deactivateNode() function...\n");
#endif
	double max_variable = 0;	// the variable we use to determine the "winner".
	double min_variable = 0;	// also used to determine the "winner", for different cases.
	double new_variable = 0;
	vector<int> node_indices_to_recruit;
	int counter = 0;
	vector<int> eligible_nodes = this->getNodes(ACTIVE); // gets all active nodes
													 // if we passed in a vector of indices, recruit all these indices
	for (int i = 0; i < this->config->deactivation_priorities.Count; i++) {

		// after each iteration we should check the size of our node_indices_to_recruit and see if we need to proceed.
		switch (this->config->deactivation_priorities[i]) {
		case LONGEST_ACTIVE: {
			max_variable = 0;
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tEntering the Longest Active Condition...\n");
#endif
			for (auto n : eligible_nodes) { // this deactivates the longest-running active node
				new_variable = this->all_nodes[n]->getTime(ACTIVE_TIME_TOTAL);
				if ((new_variable >= max_variable) && (this->all_nodes[n]->getTime(ACTIVE_TIME_TOTAL) > 0)) {
					// this allows us to clear the entire array if we have found the best-yet variable
					if (new_variable > max_variable) {
#ifdef DEBUG_MIN
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						node_indices_to_recruit.clear();

					}
					max_variable = new_variable;
					node_indices_to_recruit.push_back(n);
#ifdef DEBUG_MIN
					OutputDebugMessage(L"\t\tPushing back index ", n, L"to node_indices_to_recruit.\n");
#endif
				}
			}
			break; // break out of the switch statement
		}

							 // this will be done by comparing adjacent ACTIVE and DORMANT nodes which are adjacent, and requires
							 // a vector to tell us which nodes are adjacent (or partially adjacent...?)
		case MAXIMIZE_CHARGE_ENTROPY: {
			max_variable = 0; // since we are trying to maximize entropy anything is higher than 0
			for (auto n : eligible_nodes) {
				new_variable = calculateChargeEntropy(n, DEACTIVATING_NODE); // gets the charge entropy based on currently active nodes

				if (new_variable >= max_variable) {
					if (new_variable > max_variable) {
#ifdef DEBUG_MIN
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						node_indices_to_recruit.clear();

					}
					max_variable = new_variable;
					node_indices_to_recruit.push_back(n);
				}
			}
			break;
		}

		case MAXIMIZE_CHARGE_DISTANCE: {
			max_variable = 99999999;
#ifdef DEBUG_IN
			OutputDebugMessage(L"\tEntering the Maximize Charge Distance Condition...\n");
#endif
			for (auto n : eligible_nodes) {
				new_variable = this->calculateChargeDistance(n, THIS_NODE_ONLY); // will keeping this node maximize distance?
				if (new_variable <= max_variable) {

					if (new_variable < max_variable) {
#ifdef DEBUG_MIN
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						node_indices_to_recruit.clear();

					}
					max_variable = new_variable;
#ifdef DEBUG_MIN
					OutputDebugMessage(L"\t\tPushing back index ", n, L" to node_indices_to_recruit.\n");
#endif
					node_indices_to_recruit.push_back(n);
				}
				break;

			}
		}
		case RANDOM_SELECT: {
			if (node_indices_to_recruit.size() > 0) {
				int temp_node = node_indices_to_recruit[0];
				node_indices_to_recruit.clear();
				node_indices_to_recruit.push_back(temp_node);
			}


			break;
		}
		}
		if (node_indices_to_recruit.size() == 1) {
			this->all_nodes[node_indices_to_recruit[0]]->setStatus(RAMPING_DOWN); // this deactivates our new node.
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tSuccessfully deactivated the node with index ", node_indices_to_recruit[0], L".\n");
#endif
			break; // if after one iteration we have only one node added then break out of this function.
		}
		// sets the nodes we have to choose from (eligible_nodes) to the nodes the last (more important) condition selected
		else if (node_indices_to_recruit.size() > 1) {
			eligible_nodes = node_indices_to_recruit;
		}
	}
	if (node_indices_to_recruit.size() == 0) {
#ifdef DEBUG_MIN
		OutputDebugMessage(L"\tThere are no eligible nodes to recruit.\n");
#endif
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"deactivateNode() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}

// this determines which new node to recruit based on the protocol we have selected, theoretically finished.
// need to add in maximize charge entropy condition
void NodeArray::recruitNode() {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MIN
	OutputDebugMessage(L"Beginning recruitNode() function...\n");
#endif
	double max_variable = 0; // the variable we use to determine the "winner".
	double new_variable = 0;
	vector<int> node_indices_to_recruit;
	vector<int> eligible_nodes = this->getNodes(DORMANT); // only DORMANT nodes are eligible for recruitment
	for (int i = 0; i < this->config->recruitment_priorities.Count; i++) {
		switch (this->config->recruitment_priorities[i]) {
		case LONGEST_DORMANT: {
			max_variable = 0;
#ifdef DEBUG_MIN 
			OutputDebugMessage(L"\tEntering the Longest Dormant Condition...\n");
#endif
			for (auto n : eligible_nodes) {
				new_variable = this->all_nodes[n]->getTime(DORMANT_TIME_TOTAL);
				if (max_variable == 0) {
					max_variable = all_nodes[n]->getTime(DORMANT_TIME_TOTAL);
				}
				if ((new_variable >= max_variable) && (this->all_nodes[n]->getTime(DORMANT_TIME_TOTAL) > 0)) {
					if (new_variable > max_variable) {
#ifdef DEBUG_MAX
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						node_indices_to_recruit.clear();

					}
					max_variable = new_variable;
					node_indices_to_recruit.push_back(n);
#ifdef DEBUG_MAX
					OutputDebugMessage(L"\t\tPushing back index ", n, L"to node_indices_to_recruit.\n");
#endif
				}
			}
			break; // breaks out of case statement
		}
		case MAXIMIZE_CHARGE_ENTROPY: {
#ifdef DEBUG_MIN
			OutputDebugMessage(L"Entering the maximize charge entropy condition... \n");
#endif
			max_variable = 0; // since we are trying to maximize entropy anything is higher than 0
			for (auto n : eligible_nodes) {
				new_variable = calculateChargeEntropy(n, RECRUITING_NODE); // gets the charge entropy based on currently active nodes
#ifdef DEBUG_MIN
				OutputDebugMessage(L"Node ", n, L" had an entropy of: ");
				OutputDebugMessage(L"", new_variable, L".\n");
#endif
				if (new_variable >= max_variable) {
					if (new_variable > max_variable) {
						node_indices_to_recruit.clear();
					}
					max_variable = new_variable;
					node_indices_to_recruit.push_back(n);
				}
			}
			break;
		}
		case MAXIMIZE_CHARGE_DISTANCE: { // here we need to calculate the charge distance between all nodes.
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tEntering the Maximize Charge Distance Condition...\n");
#endif
			max_variable = 0;
			for (int n : eligible_nodes) {

				new_variable = this->calculateChargeDistance(n, TOTAL);
				if (new_variable >= max_variable) {

					if (new_variable > max_variable) {
#ifdef DEBUG_MAX
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						node_indices_to_recruit.clear();

					}
					max_variable = new_variable;
#ifdef DEBUG_MAX
					OutputDebugMessage(L"\t\tPushing back index ", eligible_nodes[n], L" to node_indices_to_recruit.\n");
#endif
					node_indices_to_recruit.push_back(n);
				}
			}
			break; // breaks out of switch statement
		}
									   // this is the catch-all clause that allows us to recruit a node when multiple may be a good choice.
		case RANDOM_SELECT: { // not really so random but w/e
			int temp_node = node_indices_to_recruit[0];
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tRecruiting a 'random' node with index ", temp_node, L".\n");
#endif
			node_indices_to_recruit.clear();
			node_indices_to_recruit.push_back(temp_node);
			break; // breaks out of switch statement
		}
		default:
#ifdef DEBUG_MIN
			OutputDebugMessage(L"Was not able to find requested priority.\n");
#endif
			break;
		}
		// if we only have 1 node in our vector after any round, we have the one we want.
		if (node_indices_to_recruit.size() == 1) {
			this->all_nodes[node_indices_to_recruit[0]]->setStatus(RAMPING_UP); // this deactivates our new node.
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tSuccessfully deactivated the node with index ", node_indices_to_recruit[0], L".\n");
#endif
			break; // if after one iteration we have only one node added then break out of this function.
		}
		// if our vector has greater than 1 node in it, we still need to narrow it down.
		else if (node_indices_to_recruit.size() > 1) {
			eligible_nodes = node_indices_to_recruit;
		}
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"recruitNode() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	if (node_indices_to_recruit.size() == 0) {
#ifdef DEBUG_MIN
		OutputDebugMessage(L"\tThere are no eligible nodes to recruit.\n");
#endif
	}
}

// this determines which new node to recruit based on the protocol we have selected, theoretically finished.
// need to add in maximize charge entropy condition
void NodeArray::recruitNode(vector<int> selected_nodes) {
	for (int i = 0; i < selected_nodes.size(); i++) {
		this->all_nodes[selected_nodes[i]]->setStatus(RAMPING_UP); // this deactivates our new node.
	}
}

// this determines which new node to recruit based on the protocol we have selected, AND
// taknig into account which nodes are active. May recruit multiple nodes if no nodes are 
// sufficiently close in node_force_constant
double NodeArray::recruitNode(bool) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MIN
	OutputDebugMessage(L"Beginning recruitNode() function...\n");
#endif
	double max_variable = 0; // the variable we use to determine the "winner".
	double new_variable = 0;
	vector<int> node_indices_to_recruit;
	int number_nodes_to_recruit = 1;
	bool exists_node = false;
	vector<int> eligible_nodes = this->getNodes(DORMANT); // only DORMANT nodes are eligible for recruitment
	// so first we need to check and see if we need to recruit more than one node by 
	// comparing the node_force_constant and seeing if any two are sufficiently close to each other.
	/*
	for (auto i : eligible_nodes) {
		for (auto n : getNodes(ACTIVE)) {
			if ((this->data->node_force_constant[i] <= (this->data->node_force_constant[n] + .05)) &&
				(this->data->node_force_constant[i] >= (this->data->node_force_constant[n] - .05))) {
				// then there exists a node we can bring down for this active node within the force range we want.
				exists_node = true;
			}
		}
		if (exists_node == false) {

		}
	}
	*/
	for (int i = 0; i < this->config->recruitment_priorities.Count; i++) {
		switch (this->config->recruitment_priorities[i]) {
		case LONGEST_DORMANT: {
			max_variable = 0;
#ifdef DEBUG_MIN 
			OutputDebugMessage(L"\tEntering the Longest Dormant Condition...\n");
#endif
			for (auto n : eligible_nodes) {
				new_variable = this->all_nodes[n]->getTime(DORMANT_TIME_TOTAL);
				if (max_variable == 0) {
					max_variable = all_nodes[n]->getTime(DORMANT_TIME_TOTAL);
				}
				if ((new_variable >= max_variable) && (this->all_nodes[n]->getTime(DORMANT_TIME_TOTAL) > 0)) {
					if (new_variable > max_variable) {
						node_indices_to_recruit.clear();

					}
					max_variable = new_variable;
					node_indices_to_recruit.push_back(n);
				}
			}
			break; // breaks out of case statement
		}
		case MAXIMIZE_CHARGE_ENTROPY: {
#ifdef DEBUG_MIN
			OutputDebugMessage(L"Entering the maximize charge entropy condition... \n");
#endif
			max_variable = 0; // since we are trying to maximize entropy anything is higher than 0
			for (auto n : eligible_nodes) {
				new_variable = calculateChargeEntropy(n, RECRUITING_NODE); // gets the charge entropy based on currently active nodes
#ifdef DEBUG_MIN
				OutputDebugMessage(L"Node ", n, L" had an entropy of: ");
				OutputDebugMessage(L"", new_variable, L".\n");
#endif
				if (new_variable >= max_variable) {
					if (new_variable > max_variable) {
						node_indices_to_recruit.clear();
					}
					max_variable = new_variable;
					node_indices_to_recruit.push_back(n);
				}
			}
			break;
		}
		case MAXIMIZE_CHARGE_DISTANCE: { // here we need to calculate the charge distance between all nodes.
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tEntering the Maximize Charge Distance Condition...\n");
#endif
			max_variable = 0;
			for (int n : eligible_nodes) {

				new_variable = this->calculateChargeDistance(n, TOTAL);
				if (new_variable >= max_variable) {

					if (new_variable > max_variable) {
#ifdef DEBUG_MAX
						OutputDebugMessage(L"\t\tClearing recruited nodes.\n");
#endif
						node_indices_to_recruit.clear();

					}
					max_variable = new_variable;
					node_indices_to_recruit.push_back(n);
				}
			}
			break; // breaks out of switch statement
		}
									   // this is the catch-all clause that allows us to recruit a node when multiple may be a good choice.
		case RANDOM_SELECT: { // not really so random but w/e
			int temp_node = node_indices_to_recruit[0];
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tRecruiting a 'random' node with index ", temp_node, L".\n");
#endif
			node_indices_to_recruit.clear();
			node_indices_to_recruit.push_back(temp_node);
			break; // breaks out of switch statement
		}
		default:
#ifdef DEBUG_MIN
			OutputDebugMessage(L"Was not able to find requested priority.\n");
#endif
			break;
		}
		// if we only have 1 node in our vector after any round, we have the one we want.
		if (node_indices_to_recruit.size() == number_nodes_to_recruit) {
			for (int i = 0; i < number_nodes_to_recruit; i++) {
				this->all_nodes[node_indices_to_recruit[i]]->setStatus(RAMPING_UP); // this recruits our new node.
				return this->data->node_force_constant[node_indices_to_recruit[i]];
			}
			
#ifdef DEBUG_MIN
			OutputDebugMessage(L"\tSuccessfully deactivated the node with index ", node_indices_to_recruit[0], L".\n");
#endif
			break; // if after one iteration we have only one node added then break out of this function.
		}
		// if our vector has greater than 1 node in it, we still need to narrow it down.
		else if (node_indices_to_recruit.size() > number_nodes_to_recruit) {
			eligible_nodes = node_indices_to_recruit;
		}
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"recruitNode() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	if (node_indices_to_recruit.size() == 0) {
#ifdef DEBUG_MIN
		OutputDebugMessage(L"\tThere are no eligible nodes to recruit.\n");
#endif
	}
	return 0.0; // our default return value. (should never happen)
}

// checks to see if we need to switch nodes based on the protocol, typically this will
// check if the active time on any node exceeds the threshold we want
bool NodeArray::checkUnitRotation() {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	for (int i = 0; i < this->all_nodes.Count; i++) {
		if (all_nodes[i]->getTime(ACTIVE_TIME_TOTAL) > this->config->time_threshold) {
			return true;
		}
	}
#ifdef DEBUG_MAX
	OutputDebugMessage(L"Entering the checkUnitRotation() function...\n");
#endif

#ifdef DEBUG_TIME
	OutputDebugMessage(L"checkUnitRotation() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	return false;
}

// This should be expanded to be able to select a certain number of nodes.
// This function should be improved at some point, and perhaps rewritten 
// so as not to contain recruitNode() or deactivateNode(), but as it's own stand-alone function.
void NodeArray::rotateMotorUnit() {
	for (int i = 0; i < this->all_nodes.Count; i++) {
		// "simultaneously" deactivates one node while recruiting another. May have to modify this function.
		if (all_nodes[i]->getTime(ACTIVE_TIME_TOTAL) > this->config->time_threshold) {
			double force_amount = recruitNode(true);
			Custom::MotorUnitSignal sig = deactivateNode(force_amount);
			switch (sig) {
			case NONE:
				// do nothing
				break;
			case ANOTHER_RECRUITMENT: {
				recruitNode();
				break;
			}
			case ANOTHER_DEACTIVATION: {
				deactivateNode();
				break;
			}
			}
			break;
		}
	}
}

// explicitly recruits X number of nodes
void NodeArray::rotateMotorUnit(int x) {
	// The number of nodes we want to switch is greater than zero. 
	if (x > 0) {
		for (int i = 0; i < x; i++) { // loop through however many nodes we want to switch
			for (int n = 0; n < this->all_nodes.Count; n++) { // loop through each node
				if (all_nodes[n]->getTime(ACTIVE_TIME_TOTAL) > this->config->time_threshold) {
					deactivateNode();
					recruitNode();
					break;
				}
			}
		}
	}
	// The case in which we switch all nodes
	else if (x == -1) {
		for (int i = 0; i < this->getNodes(ACTIVE).size(); i++) {
			deactivateNode();
			recruitNode();
		}
	}
}

// explicitly deactivates a set vector of nodes and recruits a different set vector
void NodeArray::rotateMotorUnit(vector<int> deactivate_nodes, vector<int> recruit_nodes) {
	// The number of nodes we want to switch is greater than zero. 
	vector<int> to_rotate;
	for (auto i : deactivate_nodes) {
		to_rotate.clear();
		to_rotate.push_back(i);
		this->deactivateNode(to_rotate);
	}
	for (auto i : recruit_nodes) {
		to_rotate.clear();
		to_rotate.push_back(i);
		this->recruitNode(to_rotate);
	}
}

// Connects our nodes for use by the maximizeEntropy function
void NodeArray::setConnectedNodes() {
	int skeletal_counter = 0;
	// first we need to construct our scaffold for the vectors we use
	for (int i = 0; i < this->all_nodes.Count; i++) {
		List<int>^ mylist = gcnew List<int>(); // an empty list
		this->config->connected_nodes.Add(mylist); // add the emty list to connected_nodes so we have as many as we have nodes.
		this->config->connected_nodes_skeletal.Add(mylist);
	}
	for (int i = 0; i < this->all_nodes.Count; i++) {
		for (int n = 0; n < this->all_nodes.Count; n++) {
			if (i == n) {
				// don't connect the node we are looking at to itself
			}
			else {
				/*
				if the x coordinate of node i is within WIDTH + 10 of the x coordinate of node n
				AND the y coordinate of node i is within HEIGHT + 10 of the y coordinate of node n
				then and only then the two nodes are adjacent
				*/
				bool already_connected = false;

				int x_distance_between = distanceFormula(this->all_nodes[i]->getProperty(X_COORDINATE), 0, this->all_nodes[n]->getProperty(X_COORDINATE), 0);
				int y_distance_between = distanceFormula(0, this->all_nodes[i]->getProperty(Y_COORDINATE), 0, this->all_nodes[n]->getProperty(Y_COORDINATE));
				int distance_between_nodes = distanceFormula(this->all_nodes[i]->getProperty(X_COORDINATE), this->all_nodes[i]->getProperty(Y_COORDINATE),
					this->all_nodes[n]->getProperty(X_COORDINATE), this->all_nodes[n]->getProperty(Y_COORDINATE));
				
				if ((x_distance_between <= (this->all_nodes[i]->getProperty(WIDTH) + 10)) &&
					(y_distance_between <= this->all_nodes[i]->getProperty(HEIGHT) + 10) &&
					(distance_between_nodes == x_distance_between || distance_between_nodes == y_distance_between)) {
					this->config->connected_nodes[i]->Add(n);

					already_connected = false;
					// check through all of our skeletal nodes we already have to see if a union between the 
					// two nodes has already been made. If not, add it. Each vector in skeletal_connections
					// is a vector of exactly TWO integers, which is a pair of nodes to be tested.
					for (int p = 0; p < dat.skeletal_connections.size(); p++) {
						// here we add the code that tests whether we already have the skeletal connection present.
						if ((dat.skeletal_connections[p][0] == i && dat.skeletal_connections[p][1] == n) ||
							(dat.skeletal_connections[p][0] == n && dat.skeletal_connections[p][1] == i)) {
							already_connected = true;
						}

					}
					if (already_connected == false) {
						vector<int> empty_vec;
						dat.skeletal_connections.push_back(empty_vec);
						dat.skeletal_connections[skeletal_counter].push_back(i);
						dat.skeletal_connections[skeletal_counter].push_back(n);
						skeletal_counter++;
					}
#ifdef DEBUG_MIN
					OutputDebugMessage(L"Connecting node ", i, L"to node ");
					OutputDebugMessage(L"", n, L".\n");
#endif
				}
			}
		}
	}
}

// encodes the elapsed pulses with the standard coding scheme we have.
// this encoding scheme looks like
// [31][1]
// where the first bit is marked "0" and the last 31 bits represents channels 0-30
// An example long is 00000000000000000000000000001110, which turns on channels 0, 1, and 2
unsigned long NodeArray::encodeElapsedPulses() {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MAX
	OutputDebugMessage(L"Entering the encodeElapsedPulses function...\n");
#endif
	vector<int> active_nodes_index = getNodes(ACTIVE);
	unsigned long base = 1;
	unsigned long data = 0;
	if (active_nodes_index.size() == (unsigned int)0) {
		return 0;
	}
	else {
		vector<int> elapsed_nodes_index = getElapsedNodes(active_nodes_index);
		for (int i = 0; i < elapsed_nodes_index.size(); i++) { // active_nodes represents the indices of the nodes we want to pulse

			// bit shifted over 1 because pulses have a "0" in the ones position always. Amplitudes have a "1".
			data += (base << (this->all_nodes[elapsed_nodes_index[i]]->getIndex() + 1)); // encodes 2^index of the node. Channel 0 is 1, 1 is
													// 2, 2 is 4, 3 is 8, etc.
		}
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"encodeElapsedPulses() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	return data;
}

// sends a string of characters over the serial port.
void NodeArray::sendData(SerialPort^ port, String^ data) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MAX
	
#endif
	try {
		port->Write(data + ",");
	}
	catch (System::IO::IOException^ ex) {
		MessageBox::Show("Please make sure the arduino is connected to the proper port.\nError Code: " + ex->ToString(), "Connection Error");
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"sendPulses() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}

String^ NodeArray::encodeAmplitudes() {
	// only encode amplitudes of nodes we know are changing.
	unsigned long raw_data = 0;
	String^ data = "";
	vector<int> all_ramping = this->getNodes(RAMPING_ALL);
	// now we need to figure out which of these nodes have amplitudes that need to be changed.
	for (auto i : all_ramping) {
		raw_data = (this->all_nodes[i]->getAmplitude() << 1); // set the first 8 bits to the amplitude bit-shifted by 1.
		raw_data += 1;	// this means "this is an amplitude signal"
		raw_data += (this->all_nodes[i]->getIndex() << (8 + 1)); // add i bit-shifted over 8 places + 1 place for the amplitude bit
		data += raw_data.ToString();
		// so long as we are not on the last item in the all_ramping vector
		data += ",";
	}
	return data;
}

String^ NodeArray::getLastAmplitude() {
	return this->last_amp_data;
}

void NodeArray::setMVC(double mv) {
#ifdef DEBUG_MAX
	OutputDebugMessage(L"Beginning setMVC() function...\n");
#endif
	this->current_mvc = mv;
}

double NodeArray::modifyMVC(int data, Custom::CalibrationCurves curve, bool first_run) {
	if (first_run) {
		switch (curve) {
		case NINE_VOLTS_FIVE_OHM: {
			return (-0.0089*data + 8.5675);
			break;
		}
		case NINE_VOLTS_TEN_OHM: {
			return (-0.0168*data + 12.62);
			break;
		}
		case NINE_VOLTS_TWENTY_OHM: {
			return (-0.0335*data + 21.209);
		}
		case FIVE_VOLTS_FIVE_OHM: {
			return (-0.0092*data + 8.4856);
			break;
		}
		case FIVE_VOLTS_TEN_OHM: {
			return (-0.017*data + 12.857);
		}
		case FIVE_VOLTS_TWENTY_OHM: {
			return (-0.0333*data + 21.25);
		}
		default:
			return data;
			break;
		}
	}
	else {
		switch (curve) {
		case NINE_VOLTS_FIVE_OHM: {
			return (-0.0089*data + 8.5675) / this->max_MVC;
			break;
		}
		case NINE_VOLTS_TEN_OHM: {
			return (-0.0168*data + 12.62) / this->max_MVC;
			break;
		}
		case NINE_VOLTS_TWENTY_OHM: {
			return (-0.0335*data + 21.209) / this->max_MVC;
		}
		case FIVE_VOLTS_FIVE_OHM: {
			return (-0.0092*data + 8.4856) / this->max_MVC;
			break;
		}
		case FIVE_VOLTS_TEN_OHM: {
			return (-0.017*data + 12.857) / this->max_MVC;
			break;
		}
		case FIVE_VOLTS_TWENTY_OHM: {
			return (-0.0333*data + 21.25) / this->max_MVC;
		}
		default:
			return data;
			break;
		}
	}

}

// extracts MVC data from our buffer
MVCapsule^ NodeArray::extractMVCData(double start_time, double end_time) {
	MVCapsule^ mv = gcnew MVCapsule();
	// first we loop through all of our MVC data collected thus far until we find the start time and end time.
	for (int i = 0; i < this->data->mvc_timestamps.Count; i++) {
		if (this->data->mvc_timestamps[i] >= start_time) {
			mv->start_index = i;
			break;
		}
	}
	for (int i = mv->start_index; i < this->data->mvc_timestamps.Count; i++) {
		if (this->data->mvc_timestamps[i] >= end_time) {
			mv->end_index = i;
			break; // we got the information we came for. Let's leave.
		}
	}
	// add to the data our mvc data for the range of our start and end times.
	mv->data.AddRange(this->data->mvc_data.GetRange(mv->start_index, mv->end_index-mv->start_index));
	mv->timestamps.AddRange(this->data->mvc_timestamps.GetRange(mv->start_index, mv->end_index - mv->start_index));
	for (int i = 0; i < mv->data.Count; i++) {
		mv->total_data += mv->data[i];
	}
	mv->average_data = mv->total_data / mv->data.Count; // sets the average MVC value 
	return mv;
}

void NodeArray::reset() {
	for (int i = 0; i < this->all_nodes.Count; i++) {
		this->all_nodes[i]->resetToZero();
	}
	this->time->new_time = 0;
	this->time->old_time = 0;
}

void NodeArray::pictureBoxToNode(PictureBox^ box, int ind) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_MAX
	OutputDebugMessage(L"Beginning pictureBoxToNode() function...\n");
#endif
	Point* p = new Point(this->all_nodes[ind]->getProperty(X_COORDINATE)+12, this->all_nodes[ind]->getProperty(Y_COORDINATE)+15); // the +15 is so it fits in the box.
	box->Size = System::Drawing::Size(this->all_nodes[ind]->getProperty(WIDTH), this->all_nodes[ind]->getProperty(HEIGHT));
	box->Location = *p;
	box->Visible = true;
	box->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
#ifdef DEBUG_TIME
	OutputDebugMessage(L"pictureBoxToNode() took ", EndDebugTimer(), L"us to complete. \n");
#endif
};

void NodeArray::updateNodeColors(List<PictureBox^>^ boxes) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	int step1 = (this->config->frequency_maximum - this->config->frequency_nominal)/4 + this->config->frequency_nominal;
	int step2 = step1 + (this->config->frequency_maximum - this->config->frequency_nominal) / 4;
	int step3 = step2 + (this->config->frequency_maximum - this->config->frequency_nominal) / 4;
	int step4 = step3 + (this->config->frequency_maximum - this->config->frequency_nominal) / 4;
	int astep1 = (this->config->amplitude_maximum - this->config->amplitude_nominal)/4 + this->config->amplitude_nominal;
	int astep2 = astep1 + (this->config->amplitude_maximum - this->config->amplitude_nominal) / 4;
	int astep3 = astep2 + (this->config->amplitude_maximum - this->config->amplitude_nominal) / 4;
	int astep4 = astep3 + (this->config->amplitude_maximum - this->config->amplitude_nominal) / 4;
	for (int i = 0; i < this->all_nodes.Count; i++) {
		if (this->all_nodes[i]->getFrequency() == 0) {
			boxes[i]->BackColor = System::Drawing::Color::Transparent;
		}
		if ((this->all_nodes[i]->getFrequency() > this->config->frequency_nominal) && (this->all_nodes[i]->getFrequency() <= step1)) {
			boxes[i]->BackColor = System::Drawing::Color::White;
		}
		else if ((this->all_nodes[i]->getFrequency() > step1) && (this->all_nodes[i]->getFrequency() <= step2)) {
			boxes[i]->BackColor = System::Drawing::Color::LightGray;
		}
		else if ((this->all_nodes[i]->getFrequency() > step2) && (this->all_nodes[i]->getFrequency() <= step3)) {
			boxes[i]->BackColor = System::Drawing::Color::DarkGray;
		}
		else if ((this->all_nodes[i]->getFrequency() > step3) && (this->all_nodes[i]->getFrequency() <= step4)) {
			boxes[i]->BackColor = System::Drawing::Color::DarkSlateGray;
		}
		else if (this->all_nodes[i]->getFrequency() == this->config->frequency_maximum) {
			boxes[i]->BackColor = System::Drawing::Color::Black;
		}
		if ((this->all_nodes[i]->getAmplitude() > this->config->amplitude_nominal) && (this->all_nodes[i]->getAmplitude() <= astep1)) {
			boxes[i]->Image = Image::FromFile(IMAGE_PATH + "white_box.png");
		}
		else if ((this->all_nodes[i]->getAmplitude() > astep1) && (this->all_nodes[i]->getAmplitude() <= astep2)) {
			boxes[i]->Image = Image::FromFile(IMAGE_PATH + "grey_box_1.png");
		}
		else if ((this->all_nodes[i]->getAmplitude() > astep2) && (this->all_nodes[i]->getAmplitude() <= astep3)) {
			boxes[i]->Image = Image::FromFile(IMAGE_PATH + "grey_box_2.png");
		}
		else if ((this->all_nodes[i]->getAmplitude() > astep3) && (this->all_nodes[i]->getAmplitude() <= astep4)) {
			boxes[i]->Image = Image::FromFile(IMAGE_PATH + "grey_box_3.png");
		}
		else if ((this->all_nodes[i]->getAmplitude() > astep4) && (this->all_nodes[i]->getAmplitude() <= this->config->amplitude_maximum)) {
			boxes[i]->Image = Image::FromFile(IMAGE_PATH + "black_box.png");
		}
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"updateNodeColors() took ", EndDebugTimer(), L"us to complete. \n");
#endif
};

void NodeArray::updateNodeInformation(TextBox^ box, short node) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	if (node >= 0 && node < this->all_nodes.Count) {
		box->Text = "";
		box->AppendText(("Node: " + node.ToString()) + System::Environment::NewLine);
		box->AppendText("Index: " + this->all_nodes[node]->getIndex().ToString() + System::Environment::NewLine);
		box->AppendText("Status: " + ((int)this->all_nodes[node]->getStatus()).ToString() + System::Environment::NewLine);
		box->AppendText("Coordinates: " + this->all_nodes[node]->getProperty(X_COORDINATE).ToString() + ", " +
			this->all_nodes[node]->getProperty(Y_COORDINATE).ToString() + System::Environment::NewLine);
		box->AppendText("Size: " + this->all_nodes[node]->getProperty(SHAPE).ToString() + ", " +
			this->all_nodes[node]->getProperty(WIDTH).ToString() + "x" + this->all_nodes[node]->getProperty(HEIGHT).ToString() + System::Environment::NewLine);
		box->AppendText("Current Frequency: " + this->all_nodes[node]->getFrequency().ToString() + System::Environment::NewLine);
		box->AppendText("Current Period: " + this->all_nodes[node]->getPeriod().ToString() + System::Environment::NewLine);
		box->AppendText("Current Amplitude: " + this->all_nodes[node]->getAmplitude().ToString() + System::Environment::NewLine);
		box->AppendText("Status: " + ((int)this->all_nodes[node]->getStatus()).ToString() + System::Environment::NewLine);
		box->AppendText("Total Active Time: " + this->all_nodes[node]->getTime(ACTIVE_TIME_TOTAL).ToString() + System::Environment::NewLine);
		box->AppendText("Total Dormant Time: " + this->all_nodes[node]->getTime(DORMANT_TIME_TOTAL).ToString() + System::Environment::NewLine);
		box->AppendText("Current Active Time: " + this->all_nodes[node]->getTime(ACTIVE_TIME_CURRENT).ToString() + System::Environment::NewLine);
		box->AppendText("Current Dormant Time: " + this->all_nodes[node]->getTime(DORMANT_TIME_CURRENT).ToString() + System::Environment::NewLine);
		box->AppendText("Time Since Last Pulse: " + this->all_nodes[node]->getTime(SINCE_LAST_PULSE).ToString() + System::Environment::NewLine);

		//box->Text = box_text;
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"updateNodeInformation() took ", EndDebugTimer(), L"us to complete. \n");
#endif
}

double NodeArray::getRunningTime() {
	return this->time->running_time;
}

void NodeArray::setRunningTime(double time) {
	this->time->running_time = time;
}

float NodeArray::getMVC(Custom::MVCVariable) {
	return this->current_mvc;
}

// returns all of the nodes that are currently active, and thus the nodes eligible for pulse checking

vector<int> NodeArray::getNodes(Custom::NodeStatus type) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
#ifdef DEBUG_ARE_YOU_FUCKING_INSANE
	OutputDebugMessage(L"Entering the getNodes() function...\n");
#endif
	vector<int> desired_nodes;
	switch (type) {
	case ACTIVE: {
		for (int i = 0; i < this->all_nodes.Count; i++) {
			if (this->all_nodes[i]->getStatus() != DORMANT) { // then we know the node needs to be checked
				desired_nodes.push_back(i);
			}
		}
		break;
	}
	case ACTIVE_ONLY: {
		for (int i = 0; i < this->all_nodes.Count; i++) {
			if (this->all_nodes[i]->getStatus() == ACTIVE) {
				desired_nodes.push_back(i);
			}
		}
		break;
	}
	case RAMPING_ALL: {
		for (int i = 0; i < this->all_nodes.Count; i++) {
			if ((this->all_nodes[i]->getStatus() == RAMPING_UP) || (this->all_nodes[i]->getStatus() == RAMPING_DOWN)) {
				desired_nodes.push_back(i);
			}
		}
	}
	default:
		for (int i = 0; i < this->all_nodes.Count; i++) {
			if (this->all_nodes[i]->getStatus() == type) { // then we know the node needs to be checked
				desired_nodes.push_back(i);
			}
		}
		break;
	}
		
#ifdef DEBUG_TIME
	OutputDebugMessage(L"getNodes() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	return desired_nodes;
}

vector<int> NodeArray::getElapsedNodes(vector<int> act_nodes) {
#ifdef DEBUG_TIME
	StartDebugTimer();
#endif
#ifdef DEBUG_MAX
	OutputDebugMessage(L"Entering the getElapsedNodes function...\n");
#endif
	// first we need to add the time recently elapsed to all active nodes
	vector<int> elapsed_nodes;
	if (act_nodes.size() == 0) {
		return elapsed_nodes;
	}
	else {
		for (int i = 0; i < act_nodes.size(); i++) {
			// SOMETHING IS WRONG WITH THE BELOW CODE. NOT SURE WHAT.
			if (this->all_nodes[act_nodes[i]]->getTime(SINCE_LAST_PULSE) > this->all_nodes[act_nodes[i]]->getPeriod()) {
				elapsed_nodes.push_back(act_nodes[i]); // push back the index of the elapsed node.
				this->all_nodes[act_nodes[i]]->setTime(SINCE_LAST_PULSE, 0);
			}
		}
	}
#ifdef DEBUG_TIME
	OutputDebugMessage(L"getElapsedNodes() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	return elapsed_nodes;
}

float NodeArray::calculateChargeDistance(int test_node_index, Custom::ChargeVariable var) {
#ifdef DEBUG_TIME
	BeginDebugTimer();
#endif
	float distance = 0;
#ifdef DEBUG_MAX
	OutputDebugMessage(L"\t\tBeginning calculateChargeDistance() function on node ", test_node_index, L"...\n");
#endif
	switch (var) {
	case TOTAL:
		for (auto i : this->getNodes(ACTIVE)) {
			for (auto n : this->getNodes(ACTIVE)) {
				distance += distanceFormula(this->all_nodes[i]->getProperty(X_COORDINATE), this->all_nodes[i]->getProperty(Y_COORDINATE),
					this->all_nodes[n]->getProperty(X_COORDINATE), this->all_nodes[n]->getProperty(Y_COORDINATE));
			}
			distance += distanceFormula(this->all_nodes[test_node_index]->getProperty(X_COORDINATE),
				this->all_nodes[test_node_index]->getProperty(Y_COORDINATE),
				this->all_nodes[i]->getProperty(X_COORDINATE), this->all_nodes[i]->getProperty(Y_COORDINATE));
		}
		break;
	case THIS_NODE_ONLY:
		for (auto i : this->getNodes(ACTIVE)) {
			distance += distanceFormula(this->all_nodes[test_node_index]->getProperty(X_COORDINATE),
				this->all_nodes[test_node_index]->getProperty(Y_COORDINATE),
				this->all_nodes[i]->getProperty(X_COORDINATE), this->all_nodes[i]->getProperty(Y_COORDINATE));
		}
		break;
	}
	
#ifdef DEBUG_MAX
	OutputDebugMessage(L"\t\t\tReturning a value of roughly ", (long)distance, L" as the total charge distance.\n");
#endif
#ifdef DEBUG_TIME
	OutputDebugMessage(L"calculateChargeDistance() took ", EndDebugTimer(), L"us to complete. \n");
#endif
	return distance;

}

// calculates the charge entropy of our system.
int NodeArray::calculateChargeEntropy(int node_to_test, Custom::EntropyCondition test_condition) {
	int entropy = 0;
	vector<int> active_nodes = getNodes(ACTIVE_ONLY); // might want to change this to ACTIVE_ONLY. For now this works.
	vector<int> dormant_nodes = getNodes(DORMANT);
	if (test_condition == RECRUITING_NODE) {
		// theoretically the node_to_test should never already be in here. If it is that could be a problem
		active_nodes.push_back(node_to_test); // add the node to test to the active nodes
		for (int i = 0; i < dormant_nodes.size(); i++) {// remove the node to test from the dormant nodes
			if (node_to_test == dormant_nodes[i]) {
				dormant_nodes.erase(dormant_nodes.begin() + i);
				break;
			}
		}
	}
	else {
		dormant_nodes.push_back(node_to_test);
		for (int i = 0; i < active_nodes.size(); i++) {// remove the node to test from the dormant nodes
			if (node_to_test == active_nodes[i]) {
				active_nodes.erase(active_nodes.begin() + i);
				break;
			}
		}
	}
	// first we add all the entropy contributed by our active nodes
	for (int i : active_nodes) {
		for (int n : dormant_nodes) {
			if (this->config->connected_nodes[i]->Contains(n)) { // in other words, if n is connected to i
				entropy += 1;
#ifdef DEBUG_MIN
				OutputDebugMessage(L"Adding 1 to entropy from the connection between nodes ", i, L" and ");
				OutputDebugMessage(L"", n, L".\n");
#endif
			}
		}
	}
	// then we add all the entropy contributed by our dormant nodes
	for (int i : dormant_nodes) { // these are indices we are dealing with here.
		for (int n : active_nodes) {
			if (this->config->connected_nodes[i]->Contains(n)) {
				entropy += 1;
#ifdef DEBUG_MIN
				OutputDebugMessage(L"Adding 1 to entropy from the connection between nodes ", i, L" and ");
				OutputDebugMessage(L"", n, L".\n");
#endif
			}
		}
	}
	// together, they account for all but the ramping nodes.

	return entropy;
}

// this clears the time-variant data from our NodeArray and from all nodes.
// does NOT clear the last_mvc file or the last_stim_file, this could cause issues. 
// Function used for the purpose of calibration
void NodeArray::clearData() {
	this->data->mvc_data.Clear();
	this->data->mvc_timestamps.Clear();
	this->data->updated_timestamps.Clear();
	//this->data->node_force_constant.Clear();
	for (int i = 0; i < this->all_nodes.Count; i++) {
		this->all_nodes[i]->data->updated_amplitudes.Clear();
		this->all_nodes[i]->data->updated_frequencies.Clear();
	}
}

void NodeArray::InitializeThreading() {
	// sets up our arduino_port object
	this->arduino_port = (gcnew System::IO::Ports::SerialPort());
	this->arduino_port->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &NodeArray::arduino_port_DataReceived);
	this->arduino_port->BaudRate = 19200;
	this->arduino_port->PortName = "COM4"; // the default port for the arduino, can use others.
	// end arduino_port object
	// sets up our stopwatch
	this->time->watch = (gcnew System::Diagnostics::Stopwatch());
	// end watch object
};

// cleans up all our resources
void NodeArray::DestroyThreading() {
#ifdef DEBUG_TIME // this is our report about how much CPU time each of our functions used
	unsigned long long total_time = get_nodes_func + increment_time_func + calculate_charge_distance_func + recruit_node_func +
		set_time_func + get_elapsed_nodes_func + encode_elapsed_pulses_func + set_current_variables_func;
	OutputDebugMessage(L"--------FUNCTION RELATIVE TIME USE REPORT--------\n");
	OutputDebugMessage(L"getNodes():\n\t", get_nodes_func / 1000, L"ms\n");
	OutputDebugMessage(L"\t", get_nodes_func * 100 / total_time, L"%\n");
	OutputDebugMessage(L"incrementTime():\n\t", increment_time_func / 1000, L"ms total to run.\n");
	OutputDebugMessage(L"\t", increment_time_func * 100 / total_time, L"%\n");
	OutputDebugMessage(L"calculateChargeDistance():\n\t", calculate_charge_distance_func/ 1000, L"ms total to run.\n");
	OutputDebugMessage(L"\t", calculate_charge_distance_func * 100 / total_time, L"%\n");
	OutputDebugMessage(L"recruitNode():\n\t", recruit_node_func/ 1000, L"ms total to run.\n");
	OutputDebugMessage(L"\t", recruit_node_func * 100 / total_time, L"%\n");
	OutputDebugMessage(L"setTime():\n\t", set_time_func/ 1000, L"ms total to run.\n");
	OutputDebugMessage(L"\t", set_time_func * 100 / total_time, L"%\n");
	OutputDebugMessage(L"getElapsedNodes():\n\t", get_elapsed_nodes_func/ 1000, L"ms total to run.\n");
	OutputDebugMessage(L"\t", get_elapsed_nodes_func * 100/ total_time, L"%\n");
	OutputDebugMessage(L"encodeElapsedPulses():\n\t", encode_elapsed_pulses_func/ 1000, L"ms total to run.\n");
	OutputDebugMessage(L"\t", encode_elapsed_pulses_func * 100/ total_time, L"%\n");
	OutputDebugMessage(L"setCurrentVariables():\n\t", set_current_variables_func/ 1000, L"ms total to run.\n");
	OutputDebugMessage(L"\t", set_current_variables_func * 100 / total_time, L"%\n");
	OutputDebugMessage(L"--------END FUNCTION RELATIVE TIME USE REPORT--------\n");
#endif
	if (this->time->watch) {
		delete this->time->watch;
	}
	if (this->arduino_port) {
		delete arduino_port;
	}
}

// this is the method that will run once the "Stimulate" button is clicked, and will be responsible for
// sending and recieving data to and from the arduino, checking time, and everything else fun.
// this will replace our timer currently in use if we can get it to work.
void NodeArray::updateThread(System::Object^ stateInfo) {
	if (this->initialized_MC == false) {
		this->InitializeThreading(); // we only want this to happen once in the entire program.
		this->initialized_MC = true;
	}
	if (this->time->watch->IsRunning == false) {
		this->time->watch->Reset();
		this->setRunningTime(0);
		this->reset();
		this->time->watch->Start();
	}
	ThreadInfo^ ti = (ThreadInfo^)stateInfo;
	vector<int> current_node = { 0 };
	// after we have clicked the "STIMULATE" button, but stops if we click the "ABORT" button
	try {
		if (ti->getType() == GET_MAX_MVC) {
			MessageBox::Show("Please flex as hard as you can against the force meter until instructed otherwise. Close this window to begin.", "Calibrate MVC");
			Sleep(300);
			this->arduino_port->Open();
			Sleep(2000);
			this->arduino_port->Close();

			double total_data = 0;
			for (int i = 0; i < this->data->mvc_data.Count; i++) {
				total_data += this->modifyMVC(this->data->mvc_data[i], CALIBRATION_CURVE, true);
			}
			this->max_MVC = total_data / this->data->mvc_data.Count;
			this->data->mvc_data.Clear();
			this->data->mvc_timestamps.Clear();
			MessageBox::Show("Stop flexing. Your Maximum Voluntary Contraction was " + max_MVC.ToString() + "kg.", "Calibrate Force");
		}
	}
	catch (System::IO::IOException^ ex) {
		OutputDebugMessage(L"You shuold try and connecting the arduino.\n");
		MessageBox::Show("Check that your arduino is connected.\nError Code: " + ex->ToString(), "Connection Error");
	}
	catch (System::UnauthorizedAccessException^ ex1) {
		OutputDebugMessage(L"Ensure no other program is using COM1");
		MessageBox::Show("Please ensure no other program is using COM1.\nError Code: " + ex1->ToString(), "Connection Error");
	}
	while (this->connected_to_MC == true) {
#ifdef DEBUG_MAX
		OutputDebugMessage(L"Looping updateThread... \n");
#endif

		if (this->checkRampingNodes() == false) { // only recruit additional nodes if our current node(s) is(are) done changing
			// the below code runs as our "main" thrust of the program, and autonomously recruits nodes.
			if (ti->getType() == MAIN) {
				if (this->checkMVC() == ABOVE_RANGE) {
					this->deactivateNode(); // if we are above our MVC range, we need to deactivate a node
				}
				else if (this->checkMVC() == BELOW_RANGE) { // if we are below our MVC range, we need to recruit a new node.@#ifdef DEBUG_MAX

					// we only want to recruit a new node if there are any left to recruit
					if (this->getNodes(ACTIVE).size() < this->all_nodes.Count) {
						this->recruitNode();
					}
				}
				else if (this->checkMVC() == WITHIN_RANGE) {	// then check and see if we need to rotate our motor units
					if (this->checkUnitRotation() == true) {	// checks if we need to rotate to a new node.
						this->rotateMotorUnit();				// deactivates one node and recruits a new one
					}
				}
			}
			else if (ti->getType() == CALIBRATION) {
				// First we recruit all the nodes individually
				if (current_node[0] < this->all_nodes.Count) {
					// if all of our nodes are dormant, bring up a new one. 
					if (this->getNodes(DORMANT).size() == this->all_nodes.Count) {
						recruitNode(current_node);
					}
					// if any of our nodes are active (only one should ever be), bring it down after
					// a slight delay so our calibration can gather some data.
					else {
						// this ASSUMES that ramping of amplitude always directly follows frequency.
						// MUST BE FIXED OTHERWISE
						if (this->all_nodes[current_node[0]]->getTime(ACTIVE_TIME_CURRENT) >
							(this->config->frequency_ramp_offset + this->config->frequency_ramp_time + this->config->amplitude_ramp_time + 0.5)) {
							deactivateNode(current_node);
							current_node[0]++;
						}
					}
				}
				else { // this only runs after the last node has been activated individually. Now we
					// activate them in conjunction with each other.
					int ramping_counter = 0;
					while (this->connected_to_MC == true) {
						// I apologize for this hopeless convolution. What we are trying to do is extract all the nodes
						// connected to each other and activate them.
						if (checkRampingNodes() == false) {
							if (ramping_counter < dat.skeletal_connections.size()) {
								if (ramping_counter > 0) {
									Sleep(500);
									deactivateNode(dat.skeletal_connections[ramping_counter-1]);
								}
								recruitNode(dat.skeletal_connections[ramping_counter]);
								ramping_counter++;
							}
							else {
								deactivateNode(dat.skeletal_connections[ramping_counter-1]);
								this->connected_to_MC = false;
							}
						}
						Sleep(2);
						
					}
					calibrateForce();
				}
			}
		}
		Sleep(2);
	}
}

// this thread is responsible for incrementing the time on each of the nodes and the 
// NodeArray as well as checking to see if we need to send any pulses and then sending them.
// this thread is also responsible for storing our time-variant data, and starting and stopping our watch.
void NodeArray::pulseThread(System::Object^ stateInfo) {
	if (this->time->watch->IsRunning == false) {
		this->time->watch->Reset();
		this->setRunningTime(0);
		this->reset();
		this->time->watch->Start();
	}
	ThreadInfo^ ti = (ThreadInfo^)stateInfo;
	try {
		// this resets everything so we are ready to begin.
		if (this->arduino_port->IsOpen == false) {
			this->arduino_port->Open();
		}
		while (this->connected_to_MC == true) {
			this->incrementTime(this->time->watch);
			// this is the main thrust of our program, and this is the code we will run on the "main" thread.

			if (this->checkRampingNodes() == true) {
				this->setCurrentVariables(); // updates the current variables of the ramping nodes.
			}
			unsigned long pulses = this->encodeElapsedPulses();
			if (pulses > 0) {// this checks to see if we need to pulse anything, if so then we send the data.
				this->sendData(arduino_port, (pulses.ToString() + ","));
#ifdef DEBUG_MAX
				OutputDebugMessage(L"\tSending pulses: ", pulses, L".\n");
#endif
				if (this->send_amplitudes == true) { // this WILL run every 20ms only if we sent pulses also
					this->send_amplitudes = false;		// ensure we wait another (at least) 20ms
					this->time->time_since_last_amplitude = 0;

					String^ amp_data = this->encodeAmplitudes();
					if (amp_data != this->last_amp_data) {
						this->sendData(arduino_port, amp_data);
						this->last_amp_data = amp_data; // reset the last_amp_data if it happens to be different this time.
					}
						
					// here we write whatever we just updated to our variables
					for (int i = 0; i < this->all_nodes.Count; i++) {
						this->all_nodes[i]->data->updated_amplitudes.Add(this->all_nodes[i]->getAmplitude());
						this->all_nodes[i]->data->updated_frequencies.Add(this->all_nodes[i]->getFrequency());
					}
					if (this->all_nodes.Count > 0) { // same condition as the for loop above
						this->data->updated_timestamps.Add(this->time->running_time);
					}
				}
			}
			Sleep(2);
		}
		this->arduino_port->Close();
		this->time->watch->Stop();
	}
	catch (System::IO::IOException^ ex) {
		OutputDebugMessage(L"You shuold try and connecting the arduino.\n");
		MessageBox::Show("Check that your arduino is connected.\nError Code: " + ex->ToString() , "Connection Error");
	}
	catch (System::UnauthorizedAccessException^ ex1) {
		OutputDebugMessage(L"Ensure no other program is using COM1");
		MessageBox::Show("Please ensure no other program is using COM1.\nError Code: " + ex1->ToString(), "Connection Error");
	}
	this->DestroyThreading();
}