#include "ApplicationModule.h"

#include "../platform/VPPlatform.h"

namespace Quartz
{
    ApplicationManager::ApplicationManager()
        : Module({ L"Application System" })
    {
        // Nothing
    }

    Application* ApplicationManager::CreateManagedApplication(const ApplicationInfo& info)
    {
        Application* pApplication = Platform::GetInstance()->CreateApplication(info);

        if (!pApplication)
        {
            return nullptr;
        }

        mApps.PushBack(pApplication);

        return pApplication;
    }

    void ApplicationManager::Tick()
    {
        for (Application* pApp : mApps)
        {
            pApp->PollMessages();
        }
    }
}

