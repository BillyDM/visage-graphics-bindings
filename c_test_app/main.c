#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>

#include "visage_graphics_c.h"

int main() {
    printf("Hello, world!\n");

    Display* main_display = XOpenDisplay(nullptr);

    if (main_display == nullptr) {
        printf("Failed to open display\n");
        return 1;
    }

    XSync(main_display, false);

    Window root_window = XDefaultRootWindow(main_display);

    Window main_window = XCreateSimpleWindow(main_display, root_window, 0, 0, 800, 600, 0, 0, 0x00000000);
    XMapWindow(main_display, main_window);
    XFlush(main_display);

    VisageRenderer_checkInitialization((void*)main_window, (void*)main_display);

    if (VisageRenderer_supported()) {
        printf("supported\n");
    } else {
        printf("not supported\n");
    }
    if (VisageRenderer_swapChainSupported()) {
        printf("swap chain supported\n");
    } else {
        printf("swap chain not supported\n");
    }
    if (VisageRenderer_initialized()) {
        printf("initialized\n");
    } else {
        printf("not initialized\n");
    }

    VisageCanvas* canvas = VisageCanvas_new();

    VisageCanvas_pairToWindow(canvas, (void*)main_window, 800, 600);

    VisageCanvas_setColor(canvas, VisageColor_fromARGB(0xff000066));
    VisageCanvas_fill(canvas, 0, 0, 800, 600);
    VisageCanvas_setColor(canvas, VisageColor_fromARGB(0xff00ffff));
    VisageCanvas_circle(canvas, 300.0, 200.0, 100.0);

    VisageCanvas_submit(canvas, 0);

    for(;;) {
        if (getchar() == 'q') {
            break;
        }

        sleep(1);
    }

    VisageCanvas_destroy(canvas);

    return 0;
}