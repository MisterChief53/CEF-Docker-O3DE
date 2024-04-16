#include "include/cef_render_handler.h"
#include "iostream"
#include "cstddef"
//#include <png.h>
#include <unistd.h>
#include <png++/png.hpp>

//struct Pixel_struct {
//    unsigned char red;
//    unsigned char green;
//    unsigned char blue;
//    unsigned char alpha;
//};

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
        width = 1024;
        height = 768;

        //const std::byte* byteBuffer = static_cast<const std::byte*>(buffer);
        const unsigned char* charBuffer = (unsigned char*) buffer;
        //std::size_t channelSize = width * height;

        //sleep(1);

//        std::cout << "Trying to write image..." << std::endl;
//        saveRgbaToPng(byteBuffer, width, height, "pic.png");

        // create and write image to file
        png::image<png::rgba_pixel> image(width, height);
        size_t count = 0;
        for (png::uint_32 y = 0; y < image.get_height(); ++y)
        {
            for (png::uint_32 x = 0; x < image.get_width(); ++x) {
                //image[y][x] = png::rgb_pixel(x, y, x);
                //image[y][x] = png::rgba_pixel(x, y, x + y, y);
                // non-checking equivalent of image.set_pixel(x, y, ...);
                image.set_pixel(x, y, png::basic_rgba_pixel((unsigned char) *(charBuffer + count), (unsigned char) *(charBuffer + count + 1), (unsigned char) *(charBuffer + count + 2), (unsigned char) *(charBuffer + count + 3)));
                //image.set_pixel(x, y, png::basic_rgba_pixel((unsigned char) *(charBuffer + count), (unsigned char) *(charBuffer + count + 1), (unsigned char) *(charBuffer + count + 2), (unsigned char) *(charBuffer + count + 3)));
                count += 4;
            }
        }
        image.write("img.png");
        std::cout << "image written" << std::endl;

//        for (std::size_t i = 0; i < channelSize; ++i) {
//            // Calculate the starting index in the buffer for the current pixel
//            std::size_t startIndex = 4 * i;
//
//            // Access and print the values of BGRA channels for the current pixel
//            std::cout << "Pixel " << i << ": ";
//            for (std::size_t channel = 0; channel < 4; ++channel) {
//                std::cout << static_cast<int>(byteBuffer[startIndex + channel]) << " ";
//            }
//            std::cout << "\n";
//        }
//        std::cout << std::endl;
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
private:
//    void saveRgbaToPng(const std::byte* pixels, int width, int height, const char* filename) {
//        // Open output file
//        FILE* fp = fopen(filename, "wb");
//        if (!fp) {
//            // Handle file opening error
//            return;
//        }
//
//        // Create PNG writing structures
//        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//        if (!png_ptr) {
//            fclose(fp);
//            // Handle PNG structure creation error
//            return;
//        }
//
//        png_infop info_ptr = png_create_info_struct(png_ptr);
//        if (!info_ptr) {
//            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
//            fclose(fp);
//            // Handle PNG info creation error
//            return;
//        }
//
//        // Set error handling
//        if (setjmp(png_jmpbuf(png_ptr))) {
//            png_destroy_write_struct(&png_ptr, &info_ptr);
//            fclose(fp);
//            // Handle PNG error during writing
//            return;
//        }
//
//        // Set PNG info
//        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA,
//                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
//
//        std::cout << "now building raw pointers" << std::endl;
//        png_bytepp bytePointers = createRowPointers(pixels, width, height);
//        png_set_rows(png_ptr, info_ptr, bytePointers);
//
//        std::cout << "built raw pointers and set png rows" << std::endl;
//
//        // Write PNG header
//        png_write_info(png_ptr, info_ptr);
//
//        // Write image data
//        png_write_image(png_ptr, bytePointers);
//
//        std::cout << "wrote image data" << std::endl;
//
//        // Write PNG end information
//        png_write_end(png_ptr, info_ptr);
//
//        // Clean up resources
//        png_destroy_write_struct(&png_ptr, &info_ptr);
//        fclose(fp);
//    }
//
//    png_bytepp createRowPointers(const std::byte* pixels, int width, int height) {
//        png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * height);
//        for (int y = 0; y < height; y++) {
//            row_pointers[y] = (png_byte*)pixels + y * width * 4;
//        }
//        return row_pointers;
//    }


};