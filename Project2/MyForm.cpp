#include "MyForm.h"
#include <iostream>

using namespace std;
using namespace System;
using namespace System::Windows::Forms;

[STAThread]

void Main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// FormsTutorial::MyForm form; what is this equivalent to
	
	UCIBCITeamGUI::MyForm form;
	Application::Run(%form);
}
