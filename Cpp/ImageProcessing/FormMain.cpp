#include "FormMain.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Drawing::Imaging;
using namespace System::Diagnostics;
using namespace System::Threading::Tasks;
using namespace System::Windows::Forms;

using namespace ImageProcessing;

void FormMain::SetToolTip()
{
	toolTipBtnFileSelect->InitialDelay = 1000;
	toolTipBtnFileSelect->ReshowDelay = 1000;
	toolTipBtnFileSelect->AutoPopDelay = 10000;
	toolTipBtnFileSelect->ShowAlways = false;
	toolTipBtnFileSelect->SetToolTip(btnFileSelect, "Please select a file to open.\nDisplay the image on the original image.");

	toolTipBtnAllClear->InitialDelay = 1000;
	toolTipBtnAllClear->ReshowDelay = 1000;
	toolTipBtnAllClear->AutoPopDelay = 10000;
	toolTipBtnAllClear->ShowAlways = false;
	toolTipBtnAllClear->SetToolTip(btnAllClear, "Clear the display.");

	toolTipBtnStart->InitialDelay = 1000;
	toolTipBtnStart->ReshowDelay = 1000;
	toolTipBtnStart->AutoPopDelay = 10000;
	toolTipBtnStart->ShowAlways = false;
	toolTipBtnStart->SetToolTip(btnStart, "Perform unstable filter processing.");

	toolTipBtnStop->InitialDelay = 1000;
	toolTipBtnStop->ReshowDelay = 1000;
	toolTipBtnStop->AutoPopDelay = 10000;
	toolTipBtnStop->ShowAlways = false;
	toolTipBtnStop->SetToolTip(btnStop, "Processing stop.");

	return;
}

void FormMain::SetButtonEnable()
{
	btnFileSelect->Enabled = true;
	btnAllClear->Enabled = true;
	btnStart->Enabled = true;
	btnStop->Enabled = false;
}

void FormMain::SetTextTime(long long _lTime)
{
	textBoxTime->Text = _lTime.ToString();

	return;
}

void FormMain::SetPictureBoxStatus()
{
	pictureBoxStatus->Visible = false;

	return;
}

void FormMain::ExecTask()
{
	Stopwatch^ stopwatch = gcnew Stopwatch();
	stopwatch->Start();

	m_tokenSource = gcnew CancellationTokenSource();
	CancellationToken token = m_tokenSource->Token;
	bool bRst = m_edgeDetection->GoEdgeDetection(token);
	if (bRst)
	{
		pictureBoxOriginal->ImageLocation = m_strOpenFileName;
		pictureBoxAfter->Image = m_edgeDetection->GetBitmap();

		stopwatch->Stop();

		Invoke(gcnew Action<long long>(this, &FormMain::SetTextTime), stopwatch->ElapsedMilliseconds);
	}
	Invoke(gcnew Action(this, &FormMain::SetPictureBoxStatus));
	Invoke(gcnew Action(this, &FormMain::SetButtonEnable));

	stopwatch = nullptr;
	m_tokenSource = nullptr;
	m_bitmap = nullptr;

	return;
}

void FormMain::TaskWorkImageProcessing()
{
	Task::Run(gcnew Action(this, &FormMain::ExecTask));

	return;
}

void FormMain::LoadImage(void)
{
	m_bitmap = gcnew Bitmap(m_strOpenFileName);
	m_edgeDetection = gcnew EdgeDetection(m_bitmap);

	return;
}
void FormMain::OnFormClosingFormMain(Object^ sender, FormClosingEventArgs^ e)
{
	if (m_tokenSource != nullptr)
	{
		e->Cancel = true;
	}
	return;
}

void FormMain::OnClickBtnFileSelect(Object^ sender, EventArgs^ e)
{
	OpenFileDialog^ openFileDlg = gcnew OpenFileDialog();

	openFileDlg->FileName = "default.jpg";
	openFileDlg->InitialDirectory = "C:\\";
	openFileDlg->Filter = "JPG|*.jpg|PNG|*.png";
	openFileDlg->FilterIndex = 1;
	openFileDlg->Title = "Please select a file to open";
	openFileDlg->RestoreDirectory = true;
	openFileDlg->CheckFileExists = true;
	openFileDlg->CheckPathExists = true;

	if (openFileDlg->ShowDialog() == ::DialogResult::OK)
	{
		pictureBoxOriginal->Image = nullptr;
		pictureBoxAfter->Image = nullptr;
		m_strOpenFileName = openFileDlg->FileName;
		try
		{
			LoadImage();
		}
		catch (Exception^)
		{
			MessageBox::Show(this, "Open File Error", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		pictureBoxOriginal->ImageLocation = m_strOpenFileName;
		btnStart->Enabled = true;
		textBoxTime->Text = "";
	}
	return;
}

void FormMain::OnClickBtnAllClear(Object^ sender, EventArgs^ e)
{
	pictureBoxOriginal->ImageLocation = nullptr;
	pictureBoxAfter->Image = nullptr;

	m_bitmap = nullptr;
	m_strOpenFileName = "";

	textBoxTime->Text = "";

	btnFileSelect->Enabled = true;
	btnAllClear->Enabled = true;
	btnStart->Enabled = false;

	return;
}

void FormMain::OnClickBtnStart(Object^ sender, EventArgs^ e)
{
	pictureBoxAfter->Image = nullptr;

	btnFileSelect->Enabled = false;
	btnAllClear->Enabled = false;
	btnStart->Enabled = false;

	textBoxTime->Text = "";

	pictureBoxStatus->Visible = true;

	LoadImage();

	btnStop->Enabled = true;

	TaskWorkImageProcessing();

	return;
}

void FormMain::OnClickBtnStop(Object^ sender, EventArgs^ e)
{
	if (m_tokenSource != nullptr)
	{
		m_tokenSource->Cancel();
	}

	return;
}