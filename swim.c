#include <string.h>

#include <Carbon/Carbon.h>

#define setWindow(newX, newY, newWidth, newHeight) {\
	windowPosition.x = newX;\
	windowPosition.y = newY;\
	windowSize.width = newWidth;\
	windowSize.height = newHeight;\
}

int main(int argc, char** argv) {
	AXValueRef temp;
	CGSize windowSize;
	CGPoint windowPosition;
	AXUIElementRef focusedApp;
	AXUIElementRef focusedWindow;
	pid_t pid;
	ProcessSerialNumber psn;
	CGDirectDisplayID display;

	/* get currently active window */
	GetFrontProcess(&psn);
	GetProcessPID(&psn, &pid);
	focusedApp = AXUIElementCreateApplication(pid);
	AXUIElementCopyAttributeValue(focusedApp, kAXFocusedWindowAttribute,
			(CFTypeRef *)&focusedWindow);

	/* get active window size */
	AXUIElementCopyAttributeValue(focusedWindow, kAXSizeAttribute,
			(CFTypeRef *)&temp);
	AXValueGetValue(temp, kAXValueCGSizeType, &windowSize);
	CFRelease(temp);

	/* get active window position */
	AXUIElementCopyAttributeValue(focusedWindow, kAXPositionAttribute,
			(CFTypeRef *)&temp);
	AXValueGetValue(temp, kAXValueCGPointType, &windowPosition);
	CFRelease(temp);

	/* get display size */
	display = CGMainDisplayID();
	int displayWidth = CGDisplayPixelsWide(display);
	int displayHeight = CGDisplayPixelsHigh(display);

	if (argc < 2) {
		return 1;
	}

	char* position = argv[1];

	/* check options and update the active window position and size */
	if (strcmp(position, "left") == 0) {
		setWindow(0, 0, displayWidth/2, displayHeight);
	} else if (strcmp(position, "right") == 0) {
		setWindow(displayWidth/2, 0, displayWidth/2, displayHeight);
	} else if (strcmp(position, "top") == 0) {
		setWindow(0, 0, displayWidth, displayHeight/2);
	} else if (strcmp(position, "bottom") == 0) {
		setWindow(0, displayHeight/2, displayWidth, displayHeight/2);
	} else if (strcmp(position, "top-left") == 0) {
		setWindow(0, 0, displayWidth/2, displayHeight/2);
	} else if (strcmp(position, "top-right") == 0) {
		setWindow(displayWidth/2, 0, displayWidth/2, displayHeight/2);
	} else if (strcmp(position, "bottom-left") == 0) {
		setWindow(0, displayHeight/2, displayWidth/2, displayHeight/2);
	} else if (strcmp(position, "bottom-right") == 0) {
		setWindow(displayWidth/2, displayHeight/2, displayWidth/2,
				displayHeight/2);
	} else if (strcmp(position, "center") == 0) {
		setWindow((displayWidth/2)-(windowSize.width/2),
				(displayHeight/2)-(windowSize.height/2),
				windowSize.width, windowSize.height);
	} else if (strcmp(position, "full") == 0) {
		setWindow(0, 0, displayWidth, displayHeight);
	} else {
		return 1;
	}

	/* reposition active window */
	temp = AXValueCreate(kAXValueCGPointType, &windowPosition);
	AXUIElementSetAttributeValue(focusedWindow, kAXPositionAttribute, temp);
	CFRelease(temp);

	/* resize active window */
	temp = AXValueCreate(kAXValueCGSizeType, &windowSize);
	AXUIElementSetAttributeValue(focusedWindow, kAXSizeAttribute, temp);
	CFRelease(temp);

	CFRelease(focusedWindow);
	CFRelease(focusedApp);

	return 0;
}
