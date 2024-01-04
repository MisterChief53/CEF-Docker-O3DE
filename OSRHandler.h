#include "include/cef_render_handler.h"
#include "iostream"
#include "cstddef"

// This class is an implementation of the CefRenderHandler interface.
class OSRHandler : public CefRenderHandler {
    CefRect popup_rect_;
    CefRect original_popup_rect_;
    int view_width_ = 1000;
    int view_height_ = 1000;
    mutable base::AtomicRefCount ref_count_{0};

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

    // Once a popup executes, what do do?
    void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) {
        if (!show) {
            // Clear the popup rectangle.
            popup_rect_.Set(0, 0, 0, 0);
            original_popup_rect_.Set(0, 0, 0, 0);
        }
    }

    // Utility function to get a CefRect data structure that is within the
    // Dimensions we need.
    CefRect GetPopupRectInWebView(const CefRect& original_rect) {
        CefRect rc(original_rect);
        // if x or y are negative, move them to 0.
        if (rc.x < 0) {
            rc.x = 0;
        }
        if (rc.y < 0) {
            rc.y = 0;
        }
        // if popup goes outside the view, try to reposition origin
        if (rc.x + rc.width > view_width_) {
            rc.x = view_width_ - rc.width;
        }
        if (rc.y + rc.height > view_height_) {
            rc.y = view_height_ - rc.height;
        }
        // if x or y became negative, move them to 0 again.
        if (rc.x < 0) {
            rc.x = 0;
        }
        if (rc.y < 0) {
            rc.y = 0;
        }
        return rc;
    }

    // Called when the browser wants to move or resize the popup widget.
    void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) {
        // float device_scale_factor = 1;

        if (rect.width <= 0 || rect.height <= 0) {
            return;
        }

        original_popup_rect_ = rect;
        popup_rect_ = GetPopupRectInWebView(original_popup_rect_);
    }

//    Called when an element should be painted.
//
//    Pixel values passed to this method are scaled relative to view coordinates based on the value of CefScreenInfo.device_scale_factor
//    returned from GetScreenInfo. | type | indicates whether the element is the view or the popup widget. | buffer | contains the pixel
//    data for the whole image. | dirtyRects | contains the set of rectangles in pixel coordinates that need to be repainted. | buffer |
//    will be | width | *| height | *4 bytes in size and represents a BGRA image with an upper - left origin.This method is only called
//    when CefWindowInfo::shared_texture_enabled is set to false.
//
//    This is where the pixel buffer is going to be manipulatd. This is an action performed so that you *do something* each time it is
//    called.
//

    void OnPaint(CefRefPtr<CefBrowser>, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) {
//        std::size_t width = 1000;
//        std::size_t height = 1000;
        width = 1000;
        height = 1000;

        const std::byte* byteBuffer = static_cast<const std::byte*>(buffer);
        std::size_t channelSize = width * height;

        for (std::size_t i = 0; i < channelSize; ++i) {
            // Calculate the starting index in the buffer for the current pixel
            std::size_t startIndex = 4 * i;

            // Access and print the values of BGRA channels for the current pixel
            std::cout << "Pixel " << i << ": ";
            for (std::size_t channel = 0; channel < 4; ++channel) {
                std::cout << static_cast<int>(byteBuffer[startIndex + channel]) << " ";
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }

    bool HasOneRef() const {
        return ref_count_.IsOne();
    }

    virtual bool HasAtLeastOneRef() const {
        return !ref_count_.IsZero();
    };
public:
///
/// Called to increment the reference count for the object. Should be called
/// for every new copy of a pointer to a given object.
///
void AddRef() const{
    ref_count_.Increment();
}

///
/// Decrement the reference count. Returns true if the reference count is 0.
///
bool Release() const {
    return !ref_count_.Decrement();
}
};