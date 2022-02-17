#include <string.h>

#include <Carbon/Carbon.h>

int main(int argc, char** argv) {
	AXValueRef temp;
	CGSize windowSize;
	CGPoint windowPosition;
	AXUIElementRef frontMostApp;
	AXUIElementRef frontMostWindow;
	pid_t pid;
	ProcessSerialNumber psn;
	CGDirectDisplayID display;

	/* get currently active window */
	GetFrontProcess(&psn);
	GetProcessPID(&psn, &pid);
	frontMostApp = AXUIElementCreateApplication(pid);
	AXUIElementCopyAttributeValue(frontMostApp, kAXFocusedWindowAttribute, (CFTypeRef *)&frontMostWindow);

	/* get active window size */
	AXUIElementCopyAttributeValue(frontMostWindow, kAXSizeAttribute, (CFTypeRef *)&temp);
	AXValueGetValue(temp, kAXValueCGSizeType, &windowSize);
	CFRelease(temp);

	/* get active window position */
	AXUIElementCopyAttributeValue(frontMostWindow, kAXPositionAttribute, (CFTypeRef *)&temp);
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
		windowPosition.x = 0;
		windowPosition.y = 0;
		windowSize.width = displayWidth/2;
		windowSize.height = displayHeight;
	} else if (strcmp(position, "right") == 0) {
		windowPosition.x = displayWidth/2;
		windowPosition.y = 0;
		windowSize.width = displayWidth/2;
		windowSize.height = displayHeight;
	} else if (strcmp(position, "top") == 0) {
		windowPosition.x = 0;
		windowPosition.y = 0;
		windowSize.width = displayWidth;
		windowSize.height = displayHeight/2;
	} else if (strcmp(position, "bottom") == 0) {
		windowPosition.x = 0;
		windowPosition.y = displayHeight/2;
		windowSize.width = displayWidth;
		windowSize.height = displayHeight/2;
	} else if (strcmp(position, "top-left") == 0) {
		windowPosition.x = 0;
		windowPosition.y = 0;
		windowSize.width = displayWidth/2;
		windowSize.height = displayHeight/2;
	} else if (strcmp(position, "top-right") == 0) {
		windowPosition.x = displayWidth/2;
		windowPosition.y = 0;
		windowSize.width = displayWidth/2;
		windowSize.height = displayHeight/2;
	} else if (strcmp(position, "bottom-left") == 0) {
		windowPosition.x = 0;
		windowPosition.y = displayHeight/2;
		windowSize.width = displayWidth/2;
		windowSize.height = displayHeight/2;
	} else if (strcmp(position, "bottom-right") == 0) {
		windowPosition.x = displayWidth/2;
		windowPosition.y = displayHeight/2;
		windowSize.width = displayWidth/2;
		windowSize.height = displayHeight/2;
	} else if (strcmp(position, "center") == 0) {
		windowPosition.x = (displayWidth/2)-(windowSize.width/2);
		windowPosition.y = (displayHeight/2)-(windowSize.height/2);
	} else if (strcmp(position, "full") == 0) {
		windowPosition.x = 0;
		windowPosition.y = 0;
		windowSize.width = displayWidth;
		windowSize.height = displayHeight;
	} else {
		return 1;
	}

	/* reposition active window */
	temp = AXValueCreate(kAXValueCGPointType, &windowPosition);
	AXUIElementSetAttributeValue(frontMostWindow, kAXPositionAttribute, temp);
	CFRelease(temp);

	/* resize active window */
	temp = AXValueCreate(kAXValueCGSizeType, &windowSize);
	AXUIElementSetAttributeValue(frontMostWindow, kAXSizeAttribute, temp);
	CFRelease(temp);

	CFRelease(frontMostWindow);
	CFRelease(frontMostApp);

	return 0;
}
