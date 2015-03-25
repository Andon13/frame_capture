#include "capture.h"

eTB_PixelBuffer::eTB_PixelBuffer (GLuint width, GLuint height, GLenum format, GLenum type)
{
  glGenBuffers (1, &handle_);
  glBindBuffer (GL_PIXEL_PACK_BUFFER, handle_);
  glBufferData (GL_PIXEL_PACK_BUFFER, 3 * width * height, NULL, GL_DYNAMIC_READ);
  glBindBuffer (GL_PIXEL_PACK_BUFFER, 0);

  width_  = width;
  height_ = height;
  format_ = format;
  type_   = type;
}

eTB_PixelBuffer::~eTB_PixelBuffer (void)
{
  glDeleteBuffers (1, &handle_);
  handle_ = 0;

  glDeleteSync (sync_);
  sync_   = 0;
}

void
eTB_PixelBuffer::beginCopy (void)
{
  glBindBuffer   (GL_PIXEL_PACK_BUFFER, handle_);
  glPixelStorei  (GL_PACK_ALIGNMENT, 1);
  glReadPixels   (0, 0, width_, height_, format_, type_, NULL);
  glBindBuffer   (GL_PIXEL_PACK_BUFFER, 0);

  sync_ = 
    glFenceSync  (GL_SYNC_GPU_COMMANDS_COMPLETE, 0x00);
}

std::auto_ptr <GLubyte>
eTB_PixelBuffer::getData (void)
{
  //assert (isDataReady ()); // Poor performance if not ready!

  // Assumes that we're always going to use GL_RGB and GL_UNSIGNED_BYTE for now
  GLubyte* data = new GLubyte [3 * width_ * height_];

  glBindBuffer       (GL_PIXEL_PACK_BUFFER, handle_);
  glGetBufferSubData (GL_PIXEL_PACK_BUFFER, 0, 3 * width_ * height_, data);
  glBindBuffer       (GL_PIXEL_PACK_BUFFER, 0);

  return std::auto_ptr <GLubyte> (data);
}

bool
eTB_PixelBuffer::isDataReady (void)
{
  GLenum state = glClientWaitSync (sync_, GL_SYNC_FLUSH_COMMANDS_BIT, 0UL);

  if (state == GL_CONDITION_SATISFIED || state == GL_ALREADY_SIGNALED)
    return true;

  return false;
}

void
eTB_Capture::captureFrame (int width, int height)
{
  eTB_PixelBuffer* frame = new eTB_PixelBuffer (width, height);
  frame->beginCopy ();
  frames_.push (frame);
}

std::auto_ptr <eTB_PixelBuffer>
eTB_Capture::getLastFrame (void)
{
  eTB_PixelBuffer* front = frames_.size () > 0 ? frames_.front () : nullptr;

  if (front != nullptr && front->isDataReady ()) {
    frames_.pop ();
    return std::auto_ptr <eTB_PixelBuffer> (front);
  }

  return std::auto_ptr <eTB_PixelBuffer> (nullptr);
}