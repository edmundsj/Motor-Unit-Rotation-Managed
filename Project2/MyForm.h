//Arduino GUI COntrol COde
//I have no idea what to do with C++ but this is the Myform.h(the design)
#include <stdio.h>
#include "motorUnitImplementation.h"
#using <system.dll>
#include <iostream>
#include "Graphing.h"

#pragma once

// this is our timer code, and I think it is working. Let's check
// THE BELOW LINE OF CODE JUST PLACE ON LINE 158. 
// this->pads = (gcnew List<PictureBox^>()); // THIS LINE SHOULD BE COMMENTED OUT WHEN USING THE DESIGNER

/*Some of our important vim marks
	'b - bottom of page
	't - top of page
	'm - middle of page
	*/
namespace UCIBCITeamGUI {

	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace std;
	using namespace System::Xml;
	using namespace System::Collections::Generic;

	/// <summary>
	/// Summary for MyForm
	/// </summary>

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public: System::IO::Ports::SerialPort^ arduino;
	private: System::IO::Ports::SerialPort^  serialPort1;





	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label9;






	private: System::Windows::Forms::Label^  label2;





	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  protocolToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  electrodeArrayToolStripMenuItem;


	



	private: System::Windows::Forms::GroupBox^  groupBox1;




	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Timer^  slow_timer;
	private: System::Windows::Forms::Label^  data_label;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  pulses_label;

	private: System::Windows::Forms::Label^  label14;

	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label15;


	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  time_threshold_label;
	private: System::Windows::Forms::Label^  charge_threshold_label;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  deactivation_priorities_label;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::Label^  recruitment_priorities_label;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::TextBox^  node_textbox;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::ComboBox^  node_combobox;

	public:
		ThreadPool^ tp;

	private: System::Windows::Forms::Button^  graph_button;
	private: System::Windows::Forms::ToolStripMenuItem^  stimulationRunToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveLastRunToolStripMenuItem;




	private: System::Windows::Forms::ToolStripMenuItem^  mVCFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  stimFileToolStripMenuItem;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::TrackBar^  nominal_frequency_slider;
	private: System::Windows::Forms::TrackBar^  maximum_frequency_slider;
	private: System::Windows::Forms::TrackBar^  frequency_ramp_time_slider;
	private: System::Windows::Forms::TrackBar^  frequency_ramp_offset_slider;

	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::Label^  frequency_ramp_offset_label;
	private: System::Windows::Forms::Label^  frequency_ramp_time_label;
	private: System::Windows::Forms::Label^  maximum_frequency_label;
	private: System::Windows::Forms::Label^  nominal_frequency_label;
	private: System::Windows::Forms::TrackBar^  nominal_amplitude_slider;
	private: System::Windows::Forms::TrackBar^  amplitude_ramp_time_slider;
	private: System::Windows::Forms::TrackBar^  amplitude_ramp_offset_slider;

			 


	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::Label^  amplitude_ramp_offset_label;
	private: System::Windows::Forms::Label^  amplitude_ramp_time_label;
	private: System::Windows::Forms::Label^  maximum_amplitude_label;
	private: System::Windows::Forms::Label^  nominal_amplitude_label;
private: System::Windows::Forms::TrackBar^  maximum_amplitude_slider;
private: System::Windows::Forms::Button^  button3;
private: System::Windows::Forms::ToolStripMenuItem^  configurationToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  voltageLevelToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  vToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  vToolStripMenuItem1;
private: System::Windows::Forms::ToolStripMenuItem^  resistanceToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ohmToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ohmToolStripMenuItem1;
private: System::Windows::Forms::ToolStripMenuItem^  ohmToolStripMenuItem2;
private: System::Windows::Forms::ToolStripMenuItem^  microcontrollerToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  portToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  baudRateToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem3;
private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem4;
private: System::Windows::Forms::ToolStripMenuItem^  cOMXToolStripMenuItem;





	public:
		//Thread^ t2 = gcnew Thread(gcnew ParameterizedThreadStart(ThreadY::StartUp));
		//Thread^ newThread = gcnew
		//	Thread(gcnew ParameterizedThreadStart(Work::DoWork));
		// THIS IS THE KEY. THE BELOW ACTUALLY WORKS (both of them)

		NodeArray^ nodes = gcnew NodeArray();
		
		//Thread^ newThread = gcnew Thread(
		//	gcnew ThreadStart(w, &Work::DoWork));

		// THIS IS THE CODE RUN AT STARTUP
		MyForm(void)
		{
			InitializeComponent();
			InitializeCoolerStuff();
			//
			//TODO: Add the constructor code here			
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
		// this is a test
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label6;



	private: System::Windows::Forms::Label^  label7;
	List<System::Windows::Forms::PictureBox^>^ pads;
	private: System::Windows::Forms::Label^  label4;

private: System::ComponentModel::IContainer^  components;
	
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		 // BEGIN COMPONENT INITIALIZATION - WHICH I WILL MODIFY BITCHES
		void InitializeCoolerStuff(void) {
			this->pads = (gcnew List<PictureBox^>()); // THIS LINE SHOULD BE COMMENTED OUT WHEN USING THE DESIGNER
			this->pictureBox2->Image = Image::FromFile(IMAGE_PATH + "warning.png");
			this->pictureBox2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		}
		void InitializeComponent(void)
		{ 
			this->components = (gcnew System::ComponentModel::Container());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->arduino = (gcnew System::IO::Ports::SerialPort(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->protocolToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->electrodeArrayToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->stimulationRunToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mVCFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->stimFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveLastRunToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->configurationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->voltageLevelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->vToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->vToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resistanceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ohmToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ohmToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ohmToolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->microcontrollerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->portToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cOMXToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->baudRateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->slow_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->data_label = (gcnew System::Windows::Forms::Label());
			this->pulses_label = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->charge_threshold_label = (gcnew System::Windows::Forms::Label());
			this->time_threshold_label = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->recruitment_priorities_label = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->deactivation_priorities_label = (gcnew System::Windows::Forms::Label());
			this->node_textbox = (gcnew System::Windows::Forms::TextBox());
			this->node_combobox = (gcnew System::Windows::Forms::ComboBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->graph_button = (gcnew System::Windows::Forms::Button());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->nominal_frequency_slider = (gcnew System::Windows::Forms::TrackBar());
			this->maximum_frequency_slider = (gcnew System::Windows::Forms::TrackBar());
			this->frequency_ramp_time_slider = (gcnew System::Windows::Forms::TrackBar());
			this->frequency_ramp_offset_slider = (gcnew System::Windows::Forms::TrackBar());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->frequency_ramp_offset_label = (gcnew System::Windows::Forms::Label());
			this->frequency_ramp_time_label = (gcnew System::Windows::Forms::Label());
			this->maximum_frequency_label = (gcnew System::Windows::Forms::Label());
			this->nominal_frequency_label = (gcnew System::Windows::Forms::Label());
			this->nominal_amplitude_slider = (gcnew System::Windows::Forms::TrackBar());
			this->maximum_amplitude_slider = (gcnew System::Windows::Forms::TrackBar());
			this->amplitude_ramp_time_slider = (gcnew System::Windows::Forms::TrackBar());
			this->amplitude_ramp_offset_slider = (gcnew System::Windows::Forms::TrackBar());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->amplitude_ramp_offset_label = (gcnew System::Windows::Forms::Label());
			this->amplitude_ramp_time_label = (gcnew System::Windows::Forms::Label());
			this->maximum_amplitude_label = (gcnew System::Windows::Forms::Label());
			this->nominal_amplitude_label = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nominal_frequency_slider))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->maximum_frequency_slider))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frequency_ramp_time_slider))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frequency_ramp_offset_slider))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nominal_amplitude_slider))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->maximum_amplitude_slider))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->amplitude_ramp_time_slider))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->amplitude_ramp_offset_slider))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(871, 972);
			this->button1->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(416, 124);
			this->button1->TabIndex = 2;
			this->button1->Text = L"STIMULATE";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(91, 186);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(204, 25);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Nominal Frequency:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(72, 299);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(235, 25);
			this->label6->TabIndex = 26;
			this->label6->Text = L"Frequency Ramp Time:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(1088, 771);
			this->label7->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(62, 25);
			this->label7->TabIndex = 30;
			this->label7->Text = L"TIME";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(631, 238);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(38, 25);
			this->label4->TabIndex = 74;
			this->label4->Text = L"Hz";
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 19200;
			// 
			// arduino
			// 
			this->arduino->BaudRate = 19200;
			this->arduino->ReadTimeout = 15;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(108, 371);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(183, 25);
			this->label1->TabIndex = 131;
			this->label1->Text = L"Frequency Offset:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(64, 565);
			this->label9->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(228, 25);
			this->label9->TabIndex = 132;
			this->label9->Text = L"Amplitude Ramp Time:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(51, 621);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(238, 25);
			this->label2->TabIndex = 163;
			this->label2->Text = L"Amplitude Ramp Offset:";
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(24, 24);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->fileToolStripMenuItem,
					this->configurationToolStripMenuItem, this->microcontrollerToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(2104, 42);
			this->menuStrip1->TabIndex = 166;
			this->menuStrip1->Text = L"menuStrip1";
			this->menuStrip1->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &MyForm::menuStrip1_ItemClicked);
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->loadToolStripMenuItem,
					this->saveLastRunToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(64, 38);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadToolStripMenuItem
			// 
			this->loadToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->protocolToolStripMenuItem,
					this->electrodeArrayToolStripMenuItem, this->stimulationRunToolStripMenuItem
			});
			this->loadToolStripMenuItem->Name = L"loadToolStripMenuItem";
			this->loadToolStripMenuItem->Size = System::Drawing::Size(262, 38);
			this->loadToolStripMenuItem->Text = L"Load";
			// 
			// protocolToolStripMenuItem
			// 
			this->protocolToolStripMenuItem->Name = L"protocolToolStripMenuItem";
			this->protocolToolStripMenuItem->Size = System::Drawing::Size(285, 38);
			this->protocolToolStripMenuItem->Text = L"Protocol";
			this->protocolToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::protocolToolStripMenuItem_Click);
			// 
			// electrodeArrayToolStripMenuItem
			// 
			this->electrodeArrayToolStripMenuItem->Name = L"electrodeArrayToolStripMenuItem";
			this->electrodeArrayToolStripMenuItem->Size = System::Drawing::Size(285, 38);
			this->electrodeArrayToolStripMenuItem->Text = L"Electrode Array";
			this->electrodeArrayToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::electrodeArrayToolStripMenuItem_Click);
			// 
			// stimulationRunToolStripMenuItem
			// 
			this->stimulationRunToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->mVCFileToolStripMenuItem,
					this->stimFileToolStripMenuItem
			});
			this->stimulationRunToolStripMenuItem->Name = L"stimulationRunToolStripMenuItem";
			this->stimulationRunToolStripMenuItem->Size = System::Drawing::Size(285, 38);
			this->stimulationRunToolStripMenuItem->Text = L"Stimulation Run";
			this->stimulationRunToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::stimulationRunToolStripMenuItem_Click);
			// 
			// mVCFileToolStripMenuItem
			// 
			this->mVCFileToolStripMenuItem->Name = L"mVCFileToolStripMenuItem";
			this->mVCFileToolStripMenuItem->Size = System::Drawing::Size(210, 38);
			this->mVCFileToolStripMenuItem->Text = L"MVC File";
			this->mVCFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::mVCFileToolStripMenuItem_Click);
			// 
			// stimFileToolStripMenuItem
			// 
			this->stimFileToolStripMenuItem->Name = L"stimFileToolStripMenuItem";
			this->stimFileToolStripMenuItem->Size = System::Drawing::Size(210, 38);
			this->stimFileToolStripMenuItem->Text = L"Stim File";
			this->stimFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::stimFileToolStripMenuItem_Click);
			// 
			// saveLastRunToolStripMenuItem
			// 
			this->saveLastRunToolStripMenuItem->Name = L"saveLastRunToolStripMenuItem";
			this->saveLastRunToolStripMenuItem->Size = System::Drawing::Size(262, 38);
			this->saveLastRunToolStripMenuItem->Text = L"Save Last Run";
			this->saveLastRunToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveLastRunToolStripMenuItem_Click);
			// 
			// configurationToolStripMenuItem
			// 
			this->configurationToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->voltageLevelToolStripMenuItem,
					this->resistanceToolStripMenuItem
			});
			this->configurationToolStripMenuItem->Name = L"configurationToolStripMenuItem";
			this->configurationToolStripMenuItem->Size = System::Drawing::Size(125, 38);
			this->configurationToolStripMenuItem->Text = L"Load Cell";
			// 
			// voltageLevelToolStripMenuItem
			// 
			this->voltageLevelToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->vToolStripMenuItem,
					this->vToolStripMenuItem1
			});
			this->voltageLevelToolStripMenuItem->Name = L"voltageLevelToolStripMenuItem";
			this->voltageLevelToolStripMenuItem->Size = System::Drawing::Size(257, 38);
			this->voltageLevelToolStripMenuItem->Text = L"Voltage Level";
			// 
			// vToolStripMenuItem
			// 
			this->vToolStripMenuItem->Name = L"vToolStripMenuItem";
			this->vToolStripMenuItem->Size = System::Drawing::Size(143, 38);
			this->vToolStripMenuItem->Text = L"5V";
			// 
			// vToolStripMenuItem1
			// 
			this->vToolStripMenuItem1->Name = L"vToolStripMenuItem1";
			this->vToolStripMenuItem1->Size = System::Drawing::Size(143, 38);
			this->vToolStripMenuItem1->Text = L"9V";
			// 
			// resistanceToolStripMenuItem
			// 
			this->resistanceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->ohmToolStripMenuItem,
					this->ohmToolStripMenuItem1, this->ohmToolStripMenuItem2
			});
			this->resistanceToolStripMenuItem->Name = L"resistanceToolStripMenuItem";
			this->resistanceToolStripMenuItem->Size = System::Drawing::Size(257, 38);
			this->resistanceToolStripMenuItem->Text = L"Resistance";
			// 
			// ohmToolStripMenuItem
			// 
			this->ohmToolStripMenuItem->Name = L"ohmToolStripMenuItem";
			this->ohmToolStripMenuItem->Size = System::Drawing::Size(197, 38);
			this->ohmToolStripMenuItem->Text = L"5 ohm";
			// 
			// ohmToolStripMenuItem1
			// 
			this->ohmToolStripMenuItem1->Name = L"ohmToolStripMenuItem1";
			this->ohmToolStripMenuItem1->Size = System::Drawing::Size(197, 38);
			this->ohmToolStripMenuItem1->Text = L"10 ohm";
			// 
			// ohmToolStripMenuItem2
			// 
			this->ohmToolStripMenuItem2->Name = L"ohmToolStripMenuItem2";
			this->ohmToolStripMenuItem2->Size = System::Drawing::Size(197, 38);
			this->ohmToolStripMenuItem2->Text = L"20 ohm";
			// 
			// microcontrollerToolStripMenuItem
			// 
			this->microcontrollerToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->portToolStripMenuItem,
					this->baudRateToolStripMenuItem
			});
			this->microcontrollerToolStripMenuItem->Name = L"microcontrollerToolStripMenuItem";
			this->microcontrollerToolStripMenuItem->Size = System::Drawing::Size(190, 38);
			this->microcontrollerToolStripMenuItem->Text = L"Microcontroller";
			// 
			// portToolStripMenuItem
			// 
			this->portToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->cOMXToolStripMenuItem });
			this->portToolStripMenuItem->Name = L"portToolStripMenuItem";
			this->portToolStripMenuItem->Size = System::Drawing::Size(269, 38);
			this->portToolStripMenuItem->Text = L"Port";
			// 
			// cOMXToolStripMenuItem
			// 
			this->cOMXToolStripMenuItem->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->cOMXToolStripMenuItem->Name = L"cOMXToolStripMenuItem";
			this->cOMXToolStripMenuItem->Size = System::Drawing::Size(269, 38);
			this->cOMXToolStripMenuItem->Text = L"COMX";
			this->cOMXToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::cOMXToolStripMenuItem_Click);
			// 
			// baudRateToolStripMenuItem
			// 
			this->baudRateToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->toolStripMenuItem2,
					this->toolStripMenuItem3, this->toolStripMenuItem4
			});
			this->baudRateToolStripMenuItem->Name = L"baudRateToolStripMenuItem";
			this->baudRateToolStripMenuItem->Size = System::Drawing::Size(269, 38);
			this->baudRateToolStripMenuItem->Text = L"BaudRate";
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(180, 38);
			this->toolStripMenuItem2->Text = L"9600";
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(180, 38);
			this->toolStripMenuItem3->Text = L"19200";
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			this->toolStripMenuItem4->Size = System::Drawing::Size(180, 38);
			this->toolStripMenuItem4->Text = L"38400";
			// 
			// groupBox1
			// 
			this->groupBox1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->groupBox1->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->groupBox1->Location = System::Drawing::Point(755, 114);
			this->groupBox1->Margin = System::Windows::Forms::Padding(7, 6, 7, 6);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(4);
			this->groupBox1->Size = System::Drawing::Size(501, 558);
			this->groupBox1->TabIndex = 189;
			this->groupBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->Dock = System::Windows::Forms::DockStyle::Top;
			this->pictureBox2->Location = System::Drawing::Point(0, 42);
			this->pictureBox2->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(2104, 62);
			this->pictureBox2->TabIndex = 1;
			this->pictureBox2->TabStop = false;
			this->pictureBox2->Visible = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(865, 771);
			this->label8->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(151, 25);
			this->label8->TabIndex = 190;
			this->label8->Text = L"Running Time:";
			// 
			// slow_timer
			// 
			this->slow_timer->Enabled = true;
			this->slow_timer->Tick += gcnew System::EventHandler(this, &MyForm::slow_timer_Tick);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(865, 852);
			this->label12->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(116, 25);
			this->label12->TabIndex = 193;
			this->label12->Text = L"MVC Data:";
			// 
			// data_label
			// 
			this->data_label->AutoSize = true;
			this->data_label->Location = System::Drawing::Point(1088, 852);
			this->data_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->data_label->Name = L"data_label";
			this->data_label->Size = System::Drawing::Size(68, 25);
			this->data_label->TabIndex = 194;
			this->data_label->Text = L"DATA";
			// 
			// pulses_label
			// 
			this->pulses_label->AutoSize = true;
			this->pulses_label->Location = System::Drawing::Point(1088, 888);
			this->pulses_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->pulses_label->Name = L"pulses_label";
			this->pulses_label->Size = System::Drawing::Size(95, 25);
			this->pulses_label->TabIndex = 196;
			this->pulses_label->Text = L"PULSES";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(240, 148);
			this->label14->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(91, 25);
			this->label14->TabIndex = 197;
			this->label14->Text = L"Protocol";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(91, 238);
			this->label15->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(218, 25);
			this->label15->TabIndex = 198;
			this->label15->Text = L"Maximum Frequency:";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(631, 182);
			this->label16->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(38, 25);
			this->label16->TabIndex = 199;
			this->label16->Text = L"Hz";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(96, 445);
			this->label17->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(197, 25);
			this->label17->TabIndex = 201;
			this->label17->Text = L"Nominal Amplitude:";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(91, 506);
			this->label18->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(211, 25);
			this->label18->TabIndex = 202;
			this->label18->Text = L"Maximum Amplitude:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(104, 789);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(173, 25);
			this->label5->TabIndex = 205;
			this->label5->Text = L"Time Threshold: ";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(104, 828);
			this->label10->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(190, 25);
			this->label10->TabIndex = 206;
			this->label10->Text = L"Charge Threshold:";
			// 
			// charge_threshold_label
			// 
			this->charge_threshold_label->AutoSize = true;
			this->charge_threshold_label->Location = System::Drawing::Point(364, 828);
			this->charge_threshold_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->charge_threshold_label->Name = L"charge_threshold_label";
			this->charge_threshold_label->Size = System::Drawing::Size(42, 25);
			this->charge_threshold_label->TabIndex = 207;
			this->charge_threshold_label->Text = L"CH";
			// 
			// time_threshold_label
			// 
			this->time_threshold_label->AutoSize = true;
			this->time_threshold_label->Location = System::Drawing::Point(361, 789);
			this->time_threshold_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->time_threshold_label->Name = L"time_threshold_label";
			this->time_threshold_label->Size = System::Drawing::Size(62, 25);
			this->time_threshold_label->TabIndex = 208;
			this->time_threshold_label->Text = L"TIME";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(104, 902);
			this->label19->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(223, 25);
			this->label19->TabIndex = 209;
			this->label19->Text = L"Recruitment Priorities:";
			// 
			// recruitment_priorities_label
			// 
			this->recruitment_priorities_label->AutoSize = true;
			this->recruitment_priorities_label->Location = System::Drawing::Point(364, 902);
			this->recruitment_priorities_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->recruitment_priorities_label->Name = L"recruitment_priorities_label";
			this->recruitment_priorities_label->Size = System::Drawing::Size(41, 25);
			this->recruitment_priorities_label->TabIndex = 210;
			this->recruitment_priorities_label->Text = L"PR";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(104, 940);
			this->label21->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(227, 25);
			this->label21->TabIndex = 211;
			this->label21->Text = L"Deactivation Priorities:";
			// 
			// deactivation_priorities_label
			// 
			this->deactivation_priorities_label->AutoSize = true;
			this->deactivation_priorities_label->Location = System::Drawing::Point(365, 940);
			this->deactivation_priorities_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->deactivation_priorities_label->Name = L"deactivation_priorities_label";
			this->deactivation_priorities_label->Size = System::Drawing::Size(41, 25);
			this->deactivation_priorities_label->TabIndex = 212;
			this->deactivation_priorities_label->Text = L"PR";
			// 
			// node_textbox
			// 
			this->node_textbox->Location = System::Drawing::Point(1508, 182);
			this->node_textbox->Margin = System::Windows::Forms::Padding(4);
			this->node_textbox->Multiline = true;
			this->node_textbox->Name = L"node_textbox";
			this->node_textbox->Size = System::Drawing::Size(476, 348);
			this->node_textbox->TabIndex = 213;
			// 
			// node_combobox
			// 
			this->node_combobox->FormattingEnabled = true;
			this->node_combobox->Location = System::Drawing::Point(1381, 325);
			this->node_combobox->Margin = System::Windows::Forms::Padding(4);
			this->node_combobox->Name = L"node_combobox";
			this->node_combobox->Size = System::Drawing::Size(96, 33);
			this->node_combobox->TabIndex = 214;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(1396, 281);
			this->label20->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(69, 25);
			this->label20->TabIndex = 215;
			this->label20->Text = L"Node:";
			// 
			// graph_button
			// 
			this->graph_button->Location = System::Drawing::Point(1611, 559);
			this->graph_button->Margin = System::Windows::Forms::Padding(4);
			this->graph_button->Name = L"graph_button";
			this->graph_button->Size = System::Drawing::Size(225, 112);
			this->graph_button->TabIndex = 216;
			this->graph_button->Text = L"Visualize    Stimulation";
			this->graph_button->UseVisualStyleBackColor = true;
			this->graph_button->Click += gcnew System::EventHandler(this, &MyForm::graph_button_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(865, 888);
			this->label11->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(133, 25);
			this->label11->TabIndex = 221;
			this->label11->Text = L"Pulses Sent:";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(1611, 844);
			this->button2->Margin = System::Windows::Forms::Padding(4);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(225, 112);
			this->button2->TabIndex = 222;
			this->button2->Text = L"Calibrate FES";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// nominal_frequency_slider
			// 
			this->nominal_frequency_slider->Location = System::Drawing::Point(297, 176);
			this->nominal_frequency_slider->Margin = System::Windows::Forms::Padding(4);
			this->nominal_frequency_slider->Maximum = 60;
			this->nominal_frequency_slider->Name = L"nominal_frequency_slider";
			this->nominal_frequency_slider->Size = System::Drawing::Size(268, 90);
			this->nominal_frequency_slider->TabIndex = 223;
			this->nominal_frequency_slider->TickFrequency = 3;
			this->nominal_frequency_slider->Scroll += gcnew System::EventHandler(this, &MyForm::nominal_frequency_slider_Scroll);
			// 
			// maximum_frequency_slider
			// 
			this->maximum_frequency_slider->Location = System::Drawing::Point(297, 238);
			this->maximum_frequency_slider->Margin = System::Windows::Forms::Padding(4);
			this->maximum_frequency_slider->Maximum = 60;
			this->maximum_frequency_slider->Name = L"maximum_frequency_slider";
			this->maximum_frequency_slider->Size = System::Drawing::Size(268, 90);
			this->maximum_frequency_slider->TabIndex = 224;
			this->maximum_frequency_slider->TickFrequency = 3;
			this->maximum_frequency_slider->Scroll += gcnew System::EventHandler(this, &MyForm::maximum_frequency_slider_Scroll);
			// 
			// frequency_ramp_time_slider
			// 
			this->frequency_ramp_time_slider->Location = System::Drawing::Point(297, 299);
			this->frequency_ramp_time_slider->Margin = System::Windows::Forms::Padding(4);
			this->frequency_ramp_time_slider->Maximum = 100;
			this->frequency_ramp_time_slider->Name = L"frequency_ramp_time_slider";
			this->frequency_ramp_time_slider->Size = System::Drawing::Size(268, 90);
			this->frequency_ramp_time_slider->TabIndex = 225;
			this->frequency_ramp_time_slider->TickFrequency = 5;
			this->frequency_ramp_time_slider->Scroll += gcnew System::EventHandler(this, &MyForm::frequency_ramp_time_slider_Scroll);
			// 
			// frequency_ramp_offset_slider
			// 
			this->frequency_ramp_offset_slider->Location = System::Drawing::Point(297, 358);
			this->frequency_ramp_offset_slider->Margin = System::Windows::Forms::Padding(4);
			this->frequency_ramp_offset_slider->Maximum = 100;
			this->frequency_ramp_offset_slider->Name = L"frequency_ramp_offset_slider";
			this->frequency_ramp_offset_slider->Size = System::Drawing::Size(268, 90);
			this->frequency_ramp_offset_slider->TabIndex = 226;
			this->frequency_ramp_offset_slider->TickFrequency = 5;
			this->frequency_ramp_offset_slider->Scroll += gcnew System::EventHandler(this, &MyForm::frequency_offset_slider_Scroll);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(631, 358);
			this->label13->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(23, 25);
			this->label13->TabIndex = 227;
			this->label13->Text = L"s";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(631, 299);
			this->label22->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(23, 25);
			this->label22->TabIndex = 228;
			this->label22->Text = L"s";
			// 
			// frequency_ramp_offset_label
			// 
			this->frequency_ramp_offset_label->AutoSize = true;
			this->frequency_ramp_offset_label->Location = System::Drawing::Point(585, 358);
			this->frequency_ramp_offset_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->frequency_ramp_offset_label->Name = L"frequency_ramp_offset_label";
			this->frequency_ramp_offset_label->Size = System::Drawing::Size(24, 25);
			this->frequency_ramp_offset_label->TabIndex = 229;
			this->frequency_ramp_offset_label->Text = L"0";
			// 
			// frequency_ramp_time_label
			// 
			this->frequency_ramp_time_label->AutoSize = true;
			this->frequency_ramp_time_label->Location = System::Drawing::Point(585, 299);
			this->frequency_ramp_time_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->frequency_ramp_time_label->Name = L"frequency_ramp_time_label";
			this->frequency_ramp_time_label->Size = System::Drawing::Size(24, 25);
			this->frequency_ramp_time_label->TabIndex = 230;
			this->frequency_ramp_time_label->Text = L"0";
			// 
			// maximum_frequency_label
			// 
			this->maximum_frequency_label->AutoSize = true;
			this->maximum_frequency_label->Location = System::Drawing::Point(585, 238);
			this->maximum_frequency_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->maximum_frequency_label->Name = L"maximum_frequency_label";
			this->maximum_frequency_label->Size = System::Drawing::Size(24, 25);
			this->maximum_frequency_label->TabIndex = 231;
			this->maximum_frequency_label->Text = L"0";
			this->maximum_frequency_label->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// nominal_frequency_label
			// 
			this->nominal_frequency_label->AutoSize = true;
			this->nominal_frequency_label->Location = System::Drawing::Point(585, 186);
			this->nominal_frequency_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->nominal_frequency_label->Name = L"nominal_frequency_label";
			this->nominal_frequency_label->Size = System::Drawing::Size(24, 25);
			this->nominal_frequency_label->TabIndex = 232;
			this->nominal_frequency_label->Text = L"0";
			// 
			// nominal_amplitude_slider
			// 
			this->nominal_amplitude_slider->Location = System::Drawing::Point(297, 438);
			this->nominal_amplitude_slider->Margin = System::Windows::Forms::Padding(4);
			this->nominal_amplitude_slider->Maximum = 255;
			this->nominal_amplitude_slider->Name = L"nominal_amplitude_slider";
			this->nominal_amplitude_slider->Size = System::Drawing::Size(268, 90);
			this->nominal_amplitude_slider->TabIndex = 233;
			this->nominal_amplitude_slider->TickFrequency = 15;
			this->nominal_amplitude_slider->Scroll += gcnew System::EventHandler(this, &MyForm::nominal_amplitude_slider_Scroll);
			// 
			// maximum_amplitude_slider
			// 
			this->maximum_amplitude_slider->Location = System::Drawing::Point(297, 504);
			this->maximum_amplitude_slider->Margin = System::Windows::Forms::Padding(4);
			this->maximum_amplitude_slider->Maximum = 255;
			this->maximum_amplitude_slider->Name = L"maximum_amplitude_slider";
			this->maximum_amplitude_slider->Size = System::Drawing::Size(268, 90);
			this->maximum_amplitude_slider->TabIndex = 234;
			this->maximum_amplitude_slider->TickFrequency = 15;
			this->maximum_amplitude_slider->Scroll += gcnew System::EventHandler(this, &MyForm::maximum_amplitude_slider_Scroll);
			// 
			// amplitude_ramp_time_slider
			// 
			this->amplitude_ramp_time_slider->Location = System::Drawing::Point(297, 564);
			this->amplitude_ramp_time_slider->Margin = System::Windows::Forms::Padding(4);
			this->amplitude_ramp_time_slider->Maximum = 100;
			this->amplitude_ramp_time_slider->Name = L"amplitude_ramp_time_slider";
			this->amplitude_ramp_time_slider->Size = System::Drawing::Size(268, 90);
			this->amplitude_ramp_time_slider->TabIndex = 235;
			this->amplitude_ramp_time_slider->TickFrequency = 5;
			this->amplitude_ramp_time_slider->Scroll += gcnew System::EventHandler(this, &MyForm::amplitude_ramp_time_slider_Scroll);
			// 
			// amplitude_ramp_offset_slider
			// 
			this->amplitude_ramp_offset_slider->Location = System::Drawing::Point(297, 621);
			this->amplitude_ramp_offset_slider->Margin = System::Windows::Forms::Padding(4);
			this->amplitude_ramp_offset_slider->Maximum = 100;
			this->amplitude_ramp_offset_slider->Name = L"amplitude_ramp_offset_slider";
			this->amplitude_ramp_offset_slider->Size = System::Drawing::Size(268, 90);
			this->amplitude_ramp_offset_slider->TabIndex = 236;
			this->amplitude_ramp_offset_slider->TickFrequency = 5;
			this->amplitude_ramp_offset_slider->Scroll += gcnew System::EventHandler(this, &MyForm::amplitude_ramp_offset_slider_Scroll);
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(631, 565);
			this->label23->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(23, 25);
			this->label23->TabIndex = 237;
			this->label23->Text = L"s";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(631, 621);
			this->label24->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(23, 25);
			this->label24->TabIndex = 238;
			this->label24->Text = L"s";
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(631, 506);
			this->label25->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(31, 25);
			this->label25->TabIndex = 239;
			this->label25->Text = L"%";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(631, 445);
			this->label26->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(31, 25);
			this->label26->TabIndex = 240;
			this->label26->Text = L"%";
			// 
			// amplitude_ramp_offset_label
			// 
			this->amplitude_ramp_offset_label->AutoSize = true;
			this->amplitude_ramp_offset_label->Location = System::Drawing::Point(585, 621);
			this->amplitude_ramp_offset_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->amplitude_ramp_offset_label->Name = L"amplitude_ramp_offset_label";
			this->amplitude_ramp_offset_label->Size = System::Drawing::Size(24, 25);
			this->amplitude_ramp_offset_label->TabIndex = 241;
			this->amplitude_ramp_offset_label->Text = L"0";
			// 
			// amplitude_ramp_time_label
			// 
			this->amplitude_ramp_time_label->AutoSize = true;
			this->amplitude_ramp_time_label->Location = System::Drawing::Point(585, 565);
			this->amplitude_ramp_time_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->amplitude_ramp_time_label->Name = L"amplitude_ramp_time_label";
			this->amplitude_ramp_time_label->Size = System::Drawing::Size(24, 25);
			this->amplitude_ramp_time_label->TabIndex = 242;
			this->amplitude_ramp_time_label->Text = L"0";
			// 
			// maximum_amplitude_label
			// 
			this->maximum_amplitude_label->AutoSize = true;
			this->maximum_amplitude_label->Location = System::Drawing::Point(585, 506);
			this->maximum_amplitude_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->maximum_amplitude_label->Name = L"maximum_amplitude_label";
			this->maximum_amplitude_label->Size = System::Drawing::Size(24, 25);
			this->maximum_amplitude_label->TabIndex = 243;
			this->maximum_amplitude_label->Text = L"0";
			// 
			// nominal_amplitude_label
			// 
			this->nominal_amplitude_label->AutoSize = true;
			this->nominal_amplitude_label->Location = System::Drawing::Point(585, 445);
			this->nominal_amplitude_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->nominal_amplitude_label->Name = L"nominal_amplitude_label";
			this->nominal_amplitude_label->Size = System::Drawing::Size(24, 25);
			this->nominal_amplitude_label->TabIndex = 244;
			this->nominal_amplitude_label->Text = L"0";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(1611, 701);
			this->button3->Margin = System::Windows::Forms::Padding(4);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(225, 112);
			this->button3->TabIndex = 245;
			this->button3->Text = L"Set Max Force";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(2104, 1056);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->nominal_amplitude_label);
			this->Controls->Add(this->maximum_amplitude_label);
			this->Controls->Add(this->amplitude_ramp_time_label);
			this->Controls->Add(this->amplitude_ramp_offset_label);
			this->Controls->Add(this->label26);
			this->Controls->Add(this->label25);
			this->Controls->Add(this->label24);
			this->Controls->Add(this->label23);
			this->Controls->Add(this->amplitude_ramp_offset_slider);
			this->Controls->Add(this->amplitude_ramp_time_slider);
			this->Controls->Add(this->maximum_amplitude_slider);
			this->Controls->Add(this->nominal_amplitude_slider);
			this->Controls->Add(this->nominal_frequency_label);
			this->Controls->Add(this->maximum_frequency_label);
			this->Controls->Add(this->frequency_ramp_time_label);
			this->Controls->Add(this->frequency_ramp_offset_label);
			this->Controls->Add(this->label22);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->frequency_ramp_offset_slider);
			this->Controls->Add(this->frequency_ramp_time_slider);
			this->Controls->Add(this->maximum_frequency_slider);
			this->Controls->Add(this->nominal_frequency_slider);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->graph_button);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->node_combobox);
			this->Controls->Add(this->node_textbox);
			this->Controls->Add(this->deactivation_priorities_label);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->recruitment_priorities_label);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->time_threshold_label);
			this->Controls->Add(this->charge_threshold_label);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->pulses_label);
			this->Controls->Add(this->data_label);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->menuStrip1);
			this->ForeColor = System::Drawing::Color::Black;
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FES UI";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nominal_frequency_slider))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->maximum_frequency_slider))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frequency_ramp_time_slider))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frequency_ramp_offset_slider))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nominal_amplitude_slider))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->maximum_amplitude_slider))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->amplitude_ramp_time_slider))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->amplitude_ramp_offset_slider))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
// END COMPONENT INITIALIZATION

		private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
		}
			
		private: System::Void menuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
		}

		 // this gets the XML data from our electrode array and parses it into our NodeArray class.
		private: System::Void electrodeArrayToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			try {

				OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
				int active_node = 0;
				int current_attribute;
				int temp_x_coor = -1;
				int temp_y_coor = -1;
				int temp_shape = -1;
				int temp_width = -1;
				int temp_height = -1;
				int temp_index;

				// opens a dialog box so the user may select a file. This loads our node array.
				try {
					if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
#ifdef DEBUG_MAX
						OutputDebugMessage(L"Attempting to load Electrode Array XML file...\n");
#endif
						// first we erase all the previous nodes that may have been loaded
						nodes->destroyAllNodes();
						pads->Clear();
						node_combobox->Items->Clear();
						// then we add some new ones
						XmlTextReader^ xreader = gcnew XmlTextReader(openFileDialog1->FileName);
						while (xreader->Read()) {
							if (xreader->NodeType == XmlNodeType::Element) {
								if (xreader->Name == "Node") { // we have begun reading data from a node
									// do nothing
								}
								else if (xreader->Name == "node_id") {
									current_attribute = INDEX;
								}
								else if (xreader->Name == "x_coordinate") {
									current_attribute = X_COORDINATE;
								}
								else if (xreader->Name == "y_coordinate") {
									current_attribute = Y_COORDINATE;
								}
								else if (xreader->Name == "shape") {
									current_attribute = SHAPE;
								}
								else if (xreader->Name == "width") {
									current_attribute = WIDTH;
								}
								else if (xreader->Name == "height") {
									current_attribute = HEIGHT;
								}

							}
							else if (xreader->NodeType == XmlNodeType::Text) {
								switch (current_attribute) {
								case INDEX: {
									temp_index = Convert::ToInt16(xreader->Value);
								}
								case X_COORDINATE: {
									temp_x_coor = Convert::ToInt16(xreader->Value);
									break;
								}
								case Y_COORDINATE: {
									temp_y_coor = Convert::ToInt16(xreader->Value);
									break;
								}
								case SHAPE: {
									if (xreader->Value == "rectangle") {
										temp_shape = RECTANGLE;
									}
									else if (xreader->Value == "circle") {
										temp_shape = CIRCLE;
									}
									break;
								}
								case WIDTH: {
									temp_width = Convert::ToInt16(xreader->Value);
									break;
								}
								case HEIGHT: {
									temp_height = Convert::ToInt16(xreader->Value);
									break;
								}

								}
							}
							else if (xreader->NodeType == XmlNodeType::EndElement) { // we have reached the end of a node.
								if (xreader->Name == "Node") {
									nodes->addNode(temp_index, temp_x_coor, temp_y_coor, temp_shape, temp_width, temp_height);
									active_node++;
								}
							}
						}
						xreader->Close();
					}
				}
				catch (System::FormatException^ ex) {
					MessageBox::Show("Please ensure you loaded the proper XML File.\n Error Code: " + ex->ToString(), "XML Error");
				}
				// this part adds the new picture boxes to the form and makes them visible.
				for (int i = 0; i < nodes->all_nodes.Count; i++) { // for each node we set up a new pad
					PictureBox^ newbox;
					newbox = (gcnew System::Windows::Forms::PictureBox());
					pads->Add(newbox);
					this->groupBox1->Controls->Add(pads[i]);
					nodes->pictureBoxToNode(pads[i], i);
					node_combobox->Items->Add(i.ToString());
				}
				nodes->setConnectedNodes(); // connects all adjacent nodes
			}
			catch (System::Xml::XmlException^ xml_ex) {
				MessageBox::Show("Error loading the XML file. Please ensure you loaded the right file and the file was properly formatted. \n" + 
					xml_ex->ToString(), "XML Error");
			}
		}

		// this gets the XML data from our stimulation protocol and parses it into our NodeArray class
		private: System::Void protocolToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
			try {
				if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
	#ifdef DEBUG_MAX
					OutputDebugMessage(L"Attempting to load protocol XML file...\n");
	#endif
					int current_attribute = 0;
					bool in_recruitment, in_deactivation;
					XmlTextReader^ xreader = gcnew XmlTextReader(openFileDialog1->FileName);
					while (xreader->Read()) { // the xreader proceeds one node at a time.
						if (xreader->NodeType == XmlNodeType::Element) {

							if (xreader->Name == "nominal_frequency") {
								current_attribute = NOMINAL_FREQUENCY;
							}
							else if (xreader->Name == "maximum_frequency") {
								current_attribute = MAXIMUM_FREQUENCY;
							}
							else if (xreader->Name == "frequency_ramp_time") {
								current_attribute = FREQUENCY_RAMP_TIME;
							}
							else if (xreader->Name == "frequency_ramp_offset") {
								current_attribute = FREQUENCY_RAMP_DELAY;
							}
							else if (xreader->Name == "nominal_amplitude") {
								current_attribute = NOMINAL_AMPLITUDE;
							}
							else if (xreader->Name == "maximum_amplitude") {
								current_attribute = MAXIMUM_AMPLITUDE;
							}
							else if (xreader->Name == "amplitude_ramp_time") {
								current_attribute = AMPLITUDE_RAMP_TIME;
							}
							else if (xreader->Name == "amplitude_ramp_offset") {
								current_attribute = AMPLITUDE_RAMP_DELAY;
							}
							else if (xreader->Name == "time_threshold") {
								current_attribute = TIME_THRESHOLD;
							}
							else if (xreader->Name == "charge_threshold") {
								current_attribute = CHARGE_THRESHOLD;
							}
							else if (xreader->Name == "recruitment_priorities") {
								in_recruitment = true;
								in_deactivation = false;
							}
							else if (xreader->Name == "deactivation_priorities") {
								in_deactivation = true;
								in_recruitment = false;
							}
							else if (xreader->Name == "priority") {
								current_attribute = PRIORITY;
							}

						}
						else if (xreader->NodeType == XmlNodeType::Text) {
							switch (current_attribute) {
							case NOMINAL_FREQUENCY: {
								nodes->config->frequency_nominal = System::Convert::ToInt16(xreader->Value);
								break;
							}
							case MAXIMUM_FREQUENCY: {
								nodes->config->frequency_maximum = System::Convert::ToInt16(xreader->Value);
								break;
							}
							case FREQUENCY_RAMP_TIME: {
								nodes->config->frequency_ramp_time = System::Convert::ToDouble(xreader->Value);
								break;
							}
							case FREQUENCY_RAMP_DELAY: {
								nodes->config->frequency_ramp_offset = System::Convert::ToDouble(xreader->Value);
								break;
							}
							case NOMINAL_AMPLITUDE: {
								nodes->config->amplitude_nominal = System::Convert::ToInt16(xreader->Value);
								break;
							}

							case MAXIMUM_AMPLITUDE: {
								nodes->config->amplitude_maximum = System::Convert::ToInt16(xreader->Value);
								break;
							}

							case AMPLITUDE_RAMP_TIME: {
								nodes->config->amplitude_ramp_time = System::Convert::ToDouble(xreader->Value);
								break;
							}

							case AMPLITUDE_RAMP_DELAY: {
								nodes->config->amplitude_ramp_offset = System::Convert::ToDouble(xreader->Value);
								break;
							}

							case TIME_THRESHOLD: {
								nodes->config->time_threshold = System::Convert::ToInt16(xreader->Value);
								break;
							}
							case CHARGE_THRESHOLD: {
								nodes->config->charge_threshold = System::Convert::ToInt16(xreader->Value);
								break;
							}
							case PRIORITY: {
								int prior = 0;
								if (xreader->Value == "longest_dormant") {
									prior = LONGEST_DORMANT;
								}
								else if (xreader->Value == "longest_active") {
									prior = LONGEST_ACTIVE;
								}
								else if (xreader->Value == "maximize_charge_distance") {
									prior = MAXIMIZE_CHARGE_DISTANCE;
								}
								else if (xreader->Value == "maximize_charge_entropy") {
									prior = MAXIMIZE_CHARGE_ENTROPY;
								}
								else if (xreader->Value == "random_select") {
									prior = RANDOM_SELECT;
								}
								if (in_recruitment == true) {
									nodes->config->recruitment_priorities.Add(prior);
								}
								if (in_deactivation == true) {
									nodes->config->deactivation_priorities.Add(prior);
								}
								break;
							}
							}
						}
						else if (xreader->NodeType == XmlNodeType::EndElement) { // we have reached the end of a node.
							if (xreader->Name == "Node") {

							}
						}
					}
				}
		}
		catch (System::FormatException^ ex) {
			MessageBox::Show("Please ensure you are loading the proper XML file.\nError Code: " + ex->ToString() , "XML Error");
		}
				// here we set all of the protocol variables in our myNodes array and write them to the GUI
				nominal_frequency_label->Text = nodes->config->frequency_nominal.ToString();
				maximum_frequency_label->Text = nodes->config->frequency_maximum.ToString();
				frequency_ramp_time_label->Text = nodes->config->frequency_ramp_time.ToString();
				frequency_ramp_offset_label->Text = nodes->config->frequency_ramp_offset.ToString();

				nominal_amplitude_label->Text = nodes->config->amplitude_nominal.ToString();
				maximum_amplitude_label->Text = nodes->config->amplitude_maximum.ToString();
				amplitude_ramp_time_label->Text = nodes->config->amplitude_ramp_time.ToString();
				amplitude_ramp_offset_label->Text = nodes->config->amplitude_ramp_offset.ToString();

				time_threshold_label->Text = nodes->config->time_threshold.ToString();
				charge_threshold_label->Text = nodes->config->charge_threshold.ToString();

				recruitment_priorities_label->Text = "";
				deactivation_priorities_label->Text = "";
				for (int i = 0; i < nodes->config->recruitment_priorities.Count; i++) {
					recruitment_priorities_label->Text += i.ToString();
					if (i != nodes->config->recruitment_priorities[nodes->config->recruitment_priorities.Count - 1]) {
						recruitment_priorities_label->Text += ", ";
					}
				
				}
				for (Int32 i = 0; i < nodes->config->deactivation_priorities.Count; i++) {
					deactivation_priorities_label->Text += nodes->config->deactivation_priorities[i].ToString();
					if (nodes->config->deactivation_priorities[i] != nodes->config->deactivation_priorities[nodes->config->deactivation_priorities.Count - 1]) {
						deactivation_priorities_label->Text += ", ";
					}
				}
			}

			private: System::Void connectToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				//newThread->Start(arduino);
				//arduino->Open();
			}

					 // this method happens upon data being recieved from the arduino
					 /*
			private: System::Void arduino_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) { 
				try {
					nodes->setMVC(System::Convert::ToInt16(arduino->ReadLine())); // this is the raw MVC data, we will need to modify it at a later time.
				}
				catch (Exception^ ex) {

				}
			
			}
			*/

		private: System::Void slow_timer_Tick(System::Object^  sender, System::EventArgs^  e) {
			try {
				if (nodes->connected_to_MC == true) { // give the big warning sign.
					pictureBox2->Visible = true;
				}
				else {
					pictureBox2->Visible = false;
				}
				label7->Text = nodes->getRunningTime().ToString();
				//clock_rate->Text = 
				data_label->Text = nodes->getMVC(CURRENT).ToString();
				data_label->Text += " / " + nodes->desired_mvc.ToString();

				nodes->updateNodeColors(pads);
				if (node_combobox->Text != "") {
					nodes->updateNodeInformation(node_textbox, Convert::ToInt16(node_combobox->Text));
				}
				nominal_frequency_label->Text = nodes->config->frequency_nominal.ToString();
				nominal_frequency_slider->Value = nodes->config->frequency_nominal;
				maximum_frequency_label->Text = nodes->config->frequency_maximum.ToString();
				maximum_frequency_slider->Value = nodes->config->frequency_maximum;
				frequency_ramp_time_label->Text = (nodes->config->frequency_ramp_time).ToString();
				frequency_ramp_time_slider->Value = (int)(nodes->config->frequency_ramp_time * 10);
				frequency_ramp_offset_label->Text = (nodes->config->frequency_ramp_offset).ToString();
				frequency_ramp_offset_slider->Value = (int)(nodes->config->frequency_ramp_offset * 10);

				nominal_amplitude_label->Text = nodes->config->amplitude_nominal.ToString();
				nominal_amplitude_slider->Value = nodes->config->amplitude_nominal;
				maximum_amplitude_label->Text = nodes->config->amplitude_maximum.ToString();
				maximum_amplitude_slider->Value = nodes->config->amplitude_maximum;
				amplitude_ramp_time_label->Text = (nodes->config->amplitude_ramp_time).ToString();
				amplitude_ramp_time_slider->Value = (int)(nodes->config->amplitude_ramp_time * 10);
				amplitude_ramp_offset_label->Text = (nodes->config->amplitude_ramp_offset).ToString();
				amplitude_ramp_offset_slider->Value = (int)(nodes->config->amplitude_ramp_offset * 10);

				String^ yolo = System::Convert::ToString(arduino->GetPortNames()[0]);
				cOMXToolStripMenuItem->Text = yolo;
 
			}
			catch (Exception^ ex) {
				OutputDebugMessage(L"An exception occured in the slow ticking timer.\n");
			}
		}
				 // opens up a graph of the current run 
		private: System::Void graph_button_Click(System::Object^  sender, System::EventArgs^  e) {
			using namespace Graph;
			Graph::Graphing^ form2 = gcnew Graph::Graphing();
			form2->InitializeCoolStuff(nodes->data->last_mvc_file, nodes->data->last_stim_file);
			form2->Show();
		}
				 // this will save the last aborted run of our FES Stimulation, including the 
				 // MVC (force/fatigue) profile and the amplitude/frequency per node profile.
		private: System::Void saveLastRunToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			if (nodes->data->mvc_data.Count > 0) {

				// this writes our MVC profile to a CSV file
				String^ mvc_d = "";
				node_textbox->Text += ("Runtime: " + nodes->time->running_time.ToString() + System::Environment::NewLine);
				node_textbox->Text += ("Number Updates: " + nodes->data->mvc_data.Count.ToString() + System::Environment::NewLine);
				node_textbox->Text += ("Update frequency: " + (nodes->data->mvc_data.Count / nodes->time->running_time).ToString() + System::Environment::NewLine);
				//mvc_data += ("MVC, Time (ms)" + "\n");
				for (int i = 0; i < nodes->data->mvc_data.Count; i++) {
					mvc_d += ((nodes->data->mvc_timestamps[i]).ToString() + "," + nodes->data->mvc_data[i].ToString() +
						nodes->data->predicted_mvc[i].ToString() + 
						"\n");
				}

				String^ stim_data = "";
				//stim_data += ("Runtime: " + nodes->running_time.ToString() + "\n");
				//stim_data += ("Node, Frequency, Amplitude, Time(ms)" + "\n");
				for (int t = 0; t < nodes->data->updated_timestamps.Count; t++) {
					for (int n = 0; n < nodes->all_nodes.Count; n++) {
							stim_data += nodes->data->updated_timestamps[t].ToString() + ","; // add the timestamp 
							stim_data += n.ToString() + ","; // add the node number we are dealing with.
							stim_data += nodes->all_nodes[n]->data->updated_frequencies[t].ToString() + ","; // add the frequency
							stim_data += nodes->all_nodes[n]->data->updated_amplitudes[t].ToString() + "\n";
					}
				}

				// this actually writes the file.
				try {
					DateTime^ t = DateTime::Now;
					String^ my_path = System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments) + "\\";
					String^ mvc_filename = my_path + "mvc_profile";
					String^ stim_filename = my_path + "stim_profile";
					String^ filename_ending = "";
					filename_ending += "_";
					filename_ending += t->Month.ToString() + "-";
					filename_ending += (t->Day.ToString()) + "_";
					filename_ending += t->Hour.ToString() + "_";
					filename_ending += t->Minute.ToString() + ".csv";

					mvc_filename += filename_ending;
					stim_filename += filename_ending;

					nodes->data->last_stim_file = stim_filename;
					nodes->data->last_mvc_file = mvc_filename;
					System::IO::StreamWriter^ mw = gcnew System::IO::StreamWriter(mvc_filename);
					mw->Write(mvc_d);
					mw->Close();

					System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(stim_filename);
					sw->Write(stim_data);
					sw->Close();

				}
				catch (System::IO::IOException^ ex) {
					MessageBox::Show("The file we tried to write to appears to be in use. ", "File Writing Error");
				}
			}
	
		}
				 // this will let us load previous stimulation runs, need to add a file opening request
		private: System::Void stimulationRunToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	
		}
		private: System::Void mVCFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			OpenFileDialog^ openFileDialog2 = gcnew OpenFileDialog;
			if (openFileDialog2->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				nodes->data->last_mvc_file = openFileDialog2->FileName;
			}
		}
		private: System::Void stimFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			OpenFileDialog^ openFileDialog3 = gcnew OpenFileDialog;
			if (openFileDialog3->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				nodes->data->last_stim_file = openFileDialog3->FileName;
			}
		}

		// this calibrates the patient's MVC to whatever we get in the next few seconds over the serial port
				 // we may want to add some stuff that lets us re-calibrate.
		private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			if (nodes->initialized_MC == false) { 
				nodes->InitializeThreading(); // we only want this to happen once in the entire program.
				nodes->initialized_MC = true;
			}

			if (nodes->connected_to_MC == false) {
				nodes->connected_to_MC = true;

				ThreadInfo^ ti = gcnew ThreadInfo(CALIBRATION); // declares the thread we are calling as calibration
				ThreadPool::QueueUserWorkItem(gcnew WaitCallback(nodes, &NodeArray::pulseThread));
				ThreadPool::QueueUserWorkItem(gcnew WaitCallback(nodes, &NodeArray::updateThread), ti);
				button2->Text = "ABORT CALIBRATION";
			}
			else {
				nodes->connected_to_MC = false;
				// do nothing. The thread should have terminated on its own.
			}
			
		}

		private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			try {

				//nodes->all_nodes[3].setStatus(ACTIVE); // this line is going to cause issues...
				if (nodes->connected_to_MC == true) { // we have Stimulated. Need to abort
													// myTimer1->Enabled = false;
													// timeEndPeriod(1);
					nodes->updateNodeColors(pads);
					button1->Text = "STIMULATE";
					nodes->connected_to_MC = false;

				}
				else { // we have not stimulated. Need to Stimulate.
					if (nodes->initialized_MC == false) {
						nodes->InitializeThreading();
						nodes->initialized_MC = true;
					}
					button1->Text = "ABORT";
					nodes->connected_to_MC = true;

					ThreadInfo^ ti = gcnew ThreadInfo(MAIN); // declares the thread we are calling as main
					ThreadPool::SetMaxThreads(10, 10);
					ThreadPool::QueueUserWorkItem(gcnew WaitCallback(nodes, &NodeArray::pulseThread), ti);
					ThreadPool::QueueUserWorkItem(gcnew WaitCallback(nodes, &NodeArray::updateThread), ti);

				}
			}
			catch (Exception^ ex) {
				OutputDebugMessage(L"An exception occured after pressing the button1.\n");
			}
		}
				 // changes the nominal frequency
private: System::Void nominal_frequency_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->frequency_nominal = nominal_frequency_slider->Value;
}
private: System::Void maximum_frequency_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->frequency_maximum = maximum_frequency_slider->Value;
}
private: System::Void frequency_ramp_time_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->frequency_ramp_time = ((double)frequency_ramp_time_slider->Value / 10);
}
private: System::Void frequency_offset_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->frequency_ramp_offset = ((double)frequency_ramp_offset_slider->Value / 10);
}
private: System::Void nominal_amplitude_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->amplitude_nominal = nominal_amplitude_slider->Value;
}
private: System::Void maximum_amplitude_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->amplitude_maximum = maximum_amplitude_slider->Value;
}
private: System::Void amplitude_ramp_time_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->amplitude_ramp_time = ((double)amplitude_ramp_time_slider->Value) / 10;
}
private: System::Void amplitude_ramp_offset_slider_Scroll(System::Object^  sender, System::EventArgs^  e) {
	nodes->config->amplitude_ramp_offset = ((double) amplitude_ramp_offset_slider->Value) / 10;
}

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

	ThreadInfo^ ti = gcnew ThreadInfo(GET_MAX_MVC);
	ThreadPool::QueueUserWorkItem(gcnew WaitCallback(nodes, &NodeArray::updateThread), ti);
	
}
private: System::Void cOMXToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (nodes->initialized_MC == false) {
		nodes->InitializeThreading();
		nodes->initialized_MC = true;
	}
	nodes->arduino_port->PortName = cOMXToolStripMenuItem->Text;
	cOMXToolStripMenuItem->BackgroundImage = Image::FromFile(IMAGE_PATH + "checkmark.png");
}
};
}


