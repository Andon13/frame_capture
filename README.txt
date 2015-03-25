To use this, you would do something like:

#include "capture.h"

void main (void)
{
  eTB_Capture capture;

  // Main Loop
  {
    // Begin Frame

    capture.captureFrame (window_width, window_height);

    // End Frame (Swap Buffers, etc.)

    std::auto_ptr <eTB_PixelBuffer> last_frame = capture.getLastFrame ();

    // If nullptr, the frame is not ready yet...
    if (last_frame.get () != nullptr) {
      GLubyte* pixels = last_frame.get ()->getData ().get ();
      // Encode these pixels
    }
  }

  // Write all remaining frames (probably about 2 or 3 of them at program exit)
  while ((last_frame = capture.getLastFrame ()).get () != nullptr)
    GLubyte* pixels = last_frame.get ()->getData ().get ();
    // Encode these pixels
  }
}