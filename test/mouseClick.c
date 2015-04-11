#include <X11/extensions/XTest.h>
#include <unistd.h>

int main() {
    Display *dpy = NULL;
    XEvent event;
    dpy = XOpenDisplay(NULL);
    XQueryPointer(dpy, RootWindow (dpy, 0), &event.xbutton.root,
            &event.xbutton.window, &event.xbutton.x_root,
            &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
            &event.xbutton.state);

    XTestFakeMotionEvent (dpy, 0, event.xbutton.x + 100,
            event.xbutton.y + 50, CurrentTime);
    XSync(dpy, 0);
    sleep(3);

    XTestFakeMotionEvent (dpy, 0, 250, 250, CurrentTime);
    sleep(3);

    XTestFakeButtonEvent( dpy, 3, True, CurrentTime);
    XTestFakeButtonEvent (dpy, 3, False, CurrentTime);
    XCloseDisplay(dpy);
    return 0;
}
