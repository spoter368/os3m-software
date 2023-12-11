#include "Os3mApp.h"
#include "Os3mFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(Os3mApp);

bool Os3mApp::OnInit()
{
    Os3mFrame* os3mFrame = new Os3mFrame("OS3M Mouse Desktop App");
    os3mFrame->Show();
    return true;
}
