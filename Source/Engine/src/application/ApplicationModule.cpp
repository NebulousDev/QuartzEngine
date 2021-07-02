#include "ApplicationModule.h"

#include "../Engine.h"

namespace Quartz
{
    ApplicationManager::ApplicationManager()
        : Module({ L"Application System" })
    {
        // Nothing
    }

    Application* ApplicationManager::CreateManagedApplication(const ApplicationInfo& info)
    {
        Application* pApplication = Engine::GetInstance()->GetPlatform()->CreateApplication(info);

        if (!pApplication)
        {
            return nullptr;
        }

        mApps.PushBack(pApplication);

        return pApplication;
    }

    void ApplicationManager::Update(Float32 delta)
    {
        for (Application* pApp : mApps)
        {
            pApp->PollMessages();
        }
    }
}

