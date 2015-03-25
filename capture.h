#ifndef __EPSILON3D__CAPTURE_H__
#define __EPSILON3D__CAPTURE_H__

// This won't work for _you_, replace with includes to GLEW and GL.H ;)
#include "../../Epsilon/Epsilon3D/OpenGL.h"

#include <memory>
#include <queue>

class eTB_PixelBuffer {
public:
   eTB_PixelBuffer (GLuint width,
                    GLuint height,
                    GLenum format = GL_RGB,
                    GLenum type   = GL_UNSIGNED_BYTE);
  ~eTB_PixelBuffer (void);

  void                    beginCopy   (void);

  std::auto_ptr <GLubyte> getData     (void);
  bool                    isDataReady (void);

protected:
private:
  GLenum format_;
  GLenum type_;

  GLuint width_;
  GLuint height_;

  GLuint handle_;
  GLsync sync_;
};

class eTB_Capture {
public:
  void captureFrame (int width, int height);

  std::auto_ptr <eTB_PixelBuffer> getLastFrame (void);

protected:
private:
  std::queue <eTB_PixelBuffer *> frames_;
};

#endif /* __EPSILON3D__CAPTURE_H__ */
