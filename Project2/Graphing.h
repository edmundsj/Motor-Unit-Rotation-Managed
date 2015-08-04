#pragma once

namespace Graph {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	using namespace System::Collections::Generic;

	/// <summary>
	/// Summary for Graphing
	/// </summary>
	public ref class Graphing : public System::Windows::Forms::Form
	{
	private:
		/*
		List<System::Windows::Forms::DataVisualization::Charting::DataPoint^>^ stim_amp_points; // one set of these for each node
		List<System::Windows::Forms::DataVisualization::Charting::DataPoint^>^ stim_freq_points;
		List<System::Windows::Forms::DataVisualization::Charting::DataPoint^>^ mvc_points;
		*/
		List <System::Windows::Forms::DataVisualization::Charting::Series^>^ frequency_series;
		List <System::Windows::Forms::DataVisualization::Charting::Series^>^ amplitude_series;
		
	private: System::Windows::Forms::ComboBox^  comboBox1;

		System::Windows::Forms::DataVisualization::Charting::Chart^  chartX;
		
	public:
		// check out this sexy secondary constructor function I wrote :D. Deal with it VS.
		void InitializeCoolStuff(String^ mvc_file, String^ stim_file) {
			
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartAreaF = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartAreaA = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legendX = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  seriesX = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());

			List<System::Windows::Forms::DataVisualization::Charting::Series^>^ frequency_series = gcnew List<System::Windows::Forms::DataVisualization::Charting::Series^>();
			List<System::Windows::Forms::DataVisualization::Charting::Series^>^ amplitude_series = gcnew List<System::Windows::Forms::DataVisualization::Charting::Series^>();

			this->chartX = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());

			StreamReader^ mvc_reader = File::OpenText(mvc_file);

			List<double>^ x_coordinates = gcnew List<double>();
			List<short>^ y_coordinates = gcnew List<short>();

			String^ str;
			// this allows us to read our CSV files line-by-line
			// for our MVC file, we need to create points
			while ((str = mvc_reader->ReadLine()) != nullptr)
			{
				String^ temp_num = "";
				int column = 0;
				for (int i = 0; i < str->Length; i++) {
					if (str[i] == ',') { // we are in the next column
						if (column == 0) {
							x_coordinates->Add(System::Convert::ToDouble(temp_num));
							temp_num = "";
							column = 1;
						}

					}
					else {
						temp_num += str[i];
					}
					if (column == 1 && i == (str->Length - 1)) {
						y_coordinates->Add(System::Convert::ToDouble(temp_num));
						temp_num = "";
						column = 0;
					}
				}

			}
			mvc_reader->Close();

			StreamReader^ stim_reader = File::OpenText(stim_file);
			List<List<int>^>^ amplitudes = gcnew List<List<int>^>();	// amplitudes[node][time_index]
			List<List<int>^>^ frequencies = gcnew List<List<int>^>();	// frequency[node][time_index]
			List<double>^ stim_times = gcnew List<double>();			// stim_times[time_index]

			List<int>^ all_nodes = gcnew List<int>();

			int total_counter = 0;
			int active_time_counter = 0;
			int active_node = -1;
			int time_index = 0;

			// this allows us to read our CSV files line-by-line
			// for our MVC file, we need to create points
			// THIS IS CHEATING BUT ITS FOR DEBUGGING I SWEAR
			
			while ((str = stim_reader->ReadLine()) != nullptr)
			{
				String^ temp_num = "";
				int column = 0;
				for (int i = 0; i < str->Length; i++) {
					if (str[i] == ',') { // we are in the next column
						// this is the column that contains our time
						if (column == 0) {
							//The time is the same as the last time, move on. THIS COULD BE DANGEROUS!
							if (total_counter == 0) {
								stim_times->Add(System::Convert::ToDouble(temp_num));
							}
							else {
								if (System::Convert::ToDouble(temp_num) == stim_times[active_time_counter]) {
									// do nothing, it's the same time.
								}
								else {
									double active_time = System::Convert::ToDouble(temp_num);
									stim_times->Add(active_time);
									active_time_counter++;
								}
							}
						}
						// this is the column that contains our nodes
						else if (column == 1) {
							// no node was found, move on
							if (temp_num == "") {
								// an issue, but w/e
							}
							// we found a new node! Let's have a party.
							else {
								active_node = System::Convert::ToInt16(temp_num); // set the active node
								
								if (all_nodes->Contains(active_node)) {

								}
								// only initialize more lists if we haven't encountered that node yet.
								else {
									all_nodes->Add(active_node);
									
									List<int>^ temp_list = gcnew List<int>();
									amplitudes->Add(temp_list);
									frequencies->Add(temp_list);
									
								}
							}
						}
						else if (column == 2) {
							// no data, just move on.
							if (temp_num == "") {

							}
							// extract the frequency data.
							else {
								frequencies[active_node]->Insert(active_time_counter, System::Convert::ToInt16(temp_num));
							}
						}
						temp_num = ""; // reset the temp_num;
						column++;
					}
					else {
						temp_num += str[i]; 
					}
					if (column == 3 && i == (str->Length - 1)) { // this is the last column Extract amplitude info.
						amplitudes[active_node]->Insert(active_time_counter, System::Convert::ToInt16(temp_num));
						temp_num = "";
						column = 0;
					}
				}
				total_counter++;
			}

			stim_reader->Close();


			for (int i = 0; i < x_coordinates->Count; i++) { // this code block adds our data points
				System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPointX =
					(gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(x_coordinates[i], y_coordinates[i]));
				seriesX->Points->Add(dataPointX);
			}
			for (int n = 0; n < frequencies->Count; n++) {
				
				// first we create the data series we are going to use
				System::Windows::Forms::DataVisualization::Charting::Series^ temp_series_f = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				System::Windows::Forms::DataVisualization::Charting::Series^ temp_series_a = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				
				// these are needed to initialize a series. We need to create two series per iteration.
				temp_series_f->ChartArea = L"ChartAreaF";
				temp_series_f->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
				temp_series_f->Legend = L"LegendX";
				temp_series_f->Name = L"Node " + n.ToString() + " Frequency";
				
				// these are needed to initialize a series. We need to create two series per iteration.
				temp_series_a->ChartArea = L"ChartAreaA";
				temp_series_a->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
				temp_series_a->Legend = L"LegendX";
				temp_series_a->Name = L"Node " + n.ToString() + " Amplitude";
				
				// then we add our data points to the data series.
				for (int t = 0; t < stim_times->Count; t++) {
					// adds our frequency data point
					System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPointF =
						(gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(stim_times[t], frequencies[n][t]));
					temp_series_f->Points->Add(dataPointF);

					System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPointA =
						(gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(stim_times[t], amplitudes[n][t]));
					temp_series_a->Points->Add(dataPointA);
				}
				frequency_series->Add(temp_series_f);
				amplitude_series->Add(temp_series_a);
			}
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartX))->BeginInit();
			chartAreaF->Name = L"ChartAreaF";
			chartAreaF->AxisX->Title = L"Time (milliseconds)";
			chartAreaF->AxisY->Title = L"Frequency";

			chartAreaA->Name = L"ChartAreaA";
			chartAreaA->AxisX->Title = L"Time (milliseconds)";
			chartAreaA->AxisY->Title = L"Amplitude";

			seriesX->ChartArea = L"ChartAreaF";
			seriesX->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			seriesX->Legend = L"LegendX";
			seriesX->Name = L"SeriesX";

			this->chartX->Series->Add(seriesX);

			this->chartX->ChartAreas->Add(chartAreaF);
			this->chartX->ChartAreas->Add(chartAreaA);

			for (int i = 0; i < frequency_series->Count; i++) {
				this->chartX->Series->Add(frequency_series[i]);

				this->chartX->Series->Add(amplitude_series[i]);
			}
			
			legendX->Name = L"LegendX";
			this->chartX->Legends->Add(legendX);
			this->chartX->Location = System::Drawing::Point(0, 0);
			this->chartX->Name = L"chartX";
			this->chartX->Size = System::Drawing::Size(877, 547);
			this->chartX->TabIndex = 247;
			this->chartX->Text = L"chartX";


			this->Controls->Add(this->chartX);

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartX))->EndInit();
		}
		Graphing(void)
		{
			
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Graphing()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;


		

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint1 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0,
				0));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint2 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(2,
				2));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint3 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(5,
				5));
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			chartArea1->AxisX->Title = L"Title 1";
			chartArea1->AxisY->Title = L"Title 2";
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(364, 173);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			series1->Points->Add(dataPoint1);
			series1->Points->Add(dataPoint2);
			series1->Points->Add(dataPoint3);
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(513, 374);
			this->chart1->TabIndex = 217;
			this->chart1->Text = L"chart1";
			this->chart1->Visible = false;
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(13, 59);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(48, 28);
			this->comboBox1->TabIndex = 218;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Graphing::comboBox1_SelectedIndexChanged);
			// 
			// Graphing
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(878, 545);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->chart1);
			this->Name = L"Graphing";
			this->Text = L"Graphing";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		// this is where we will update the graph based on which node the user requests
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

	}
};
}
