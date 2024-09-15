#include "Application.h"
#include "events/ApplicationEvent.h"
#include "Log.h"

namespace AE
{
    AE::Application::Application()
    {
    }

    AE::Application::~Application()
    {
    }

    void AE::Application::Run()
    {
        WindowResizeEvent e(960, 540); 

        if(e.IsInCategory(EventCategoryApplication)){
            AE_INFO(e);
        }
        
        Window window;
    }
} // namespace AE
