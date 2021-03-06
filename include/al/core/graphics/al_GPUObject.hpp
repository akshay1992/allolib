#ifndef INCLUDE_AL_GRAPHICS_GPUOBJECT_H
#define INCLUDE_AL_GRAPHICS_GPUOBJECT_H

/*  Allocore --
  Multimedia / virtual environment application class library

  Copyright (C) 2009. AlloSphere Research Group, Media Arts & Technology, UCSB.
  Copyright (C) 2012. The Regents of the University of California.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

    Neither the name of the University of California nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.


  File description:
  Ensures that GPU resources are valid even when a rendering context is
  rebuilt.

  File author(s):
  Graham Wakefield, 2010, grrrwaaa@gmail.com
  Lance Putnam, 2010, putnam.lance@gmail.com
  Keehong Youn, 2017, younkeehong@gmail.com
*/

/**
  Ensures that GPU resources are valid even when a rendering context is
  rebuilt.

  GPUContext represents an object with a corollary rendering context on a GPU.
  GPUObject is a shared base class for all objects that have a corrollary
  object on the GPU, and which must be re-submitted when the rendering context
  is rebuilt.
  An object that inherits GPUContext must call contextDestroy() when the
  context has been invalidated. For example, the al::Window object does this
  automatically when the window is closed or enters/leaves fullscreen. Doing
  so will mark all associated GPUObjects as invalidated (calling their
  onDestroy() handlers).
  Each GPUObject should call validate() before attempting to render; this will
  re-submit the resources (by calling its onCreate() handler).

  GPUContexts are identified by an integer ID, which increments for each
  context created.
  The first created context has ID=0. GPUObjects will register with this
  context by default.
*/

namespace al{

/// Base class for allocated resources on the GPU
///
/// @ingroup allocore
class GPUObject{
public:
  GPUObject();

  // disable copy/move construction
  GPUObject(GPUObject const&) = delete;
  GPUObject(GPUObject&&) = delete;

  // disable copy/move assignment
  GPUObject& operator=(GPUObject const&) = delete;
  GPUObject& operator=(GPUObject&&) = delete;

  virtual ~GPUObject();

  /// Returns whether object has been created
  bool created() const;

  /// Creates object on GPU
  void create();

  /// Destroys object on GPU
  void destroy();

  /// Returns the assigned object id
  unsigned long id() const { return mID; }

  void id(unsigned long v){ mID = v; }

  /// Ensure that the GPUObject is ready to use

  /// This is typically placed before any rendering implementation.
  /// If the object has been invalidated, the object will be destroyed and
  /// then created again. Otherwise, the object will simply be created if not
  /// already created.
  void validate();

  /// Triggers re-creation of object safely
  void invalidate();

protected:
  unsigned int mID;
  bool mResubmit;

  /// Called when currently assigned context is created
  virtual void onCreate() = 0;

  /// Called when currently assigned context is destroyed
  virtual void onDestroy() = 0;
};

} // ::al

#endif
