#include "include/cef_render_handler.h"

class OSRHandler : public CefRenderHandler {
    bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {\
        // In cefclient/.../browser_window_osr_gtk.cc (what osrdelegate calls) it is always false.
        // this maybe due to well, us not having a window.
        // since it is false, per the doxygen documentation, we will have to get our rectangle
        // from GetViewRect
        return false;
    }

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
        rect.x = rect.y = 0;

        // The simulated screen and view rectangle are the same. This is necessary
        // for popup menus to be located and sized inside the view.
        // This time, I will use a dummy resolution just as a placeholder.
        // never return an empty rectangle.
        rect.width = 1000;
        rect.height = 1000;
    }

    bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info) {
        CefRect view_rect;

        GetViewRect(browser, view_rect);

        float device_scale_factor = 1;

        screen_info.device_scale_factor = device_scale_factor;

        // The screen info rectangles are used by the renderer to create and position
        // popups. Keep popups inside the view rectangle.
        screen_info.rect = view_rect;
        screen_info.available_rect = view_rect;
        return true;
    }


};