// Copyright (c) 2013, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef VM_ISOLATE_H_
#define VM_ISOLATE_H_

#include "vm/allocation.h"
#include "vm/globals.h"
#include "vm/port.h"
#include "vm/random.h"

namespace psoup {

class Heap;
class Interpreter;
class MessageLoop;
class Monitor;
class Object;
class ThreadPool;

class Isolate {
 public:
  Isolate(void* snapshot, size_t snapshot_length, uint64_t seed);
  ~Isolate();

  MessageLoop* loop() const { return loop_; }
  uintptr_t salt() const { return salt_; }
  Random& random() { return random_; }

  void ActivateMessage(IsolateMessage* message);
  void ActivateWakeup();
  void ActivateSignal(intptr_t handle,
                      intptr_t status,
                      intptr_t signals,
                      intptr_t count);

  void Interpret();

  void Spawn(IsolateMessage* initial_message);

  static void Startup();
  static void Shutdown();

  static void InterruptAll();
  void Interrupt();
  void PrintStack();

 private:
  void Activate(Object* message, Object* port);

  Heap* heap_;
  Interpreter* interpreter_;
  MessageLoop* loop_;
  void* snapshot_;
  size_t snapshot_length_;
  uintptr_t salt_;
  Random random_;
  Isolate* next_;

  void AddIsolateToList(Isolate* isolate);
  void RemoveIsolateFromList(Isolate* isolate);

  static Monitor* isolates_list_monitor_;
  static Isolate* isolates_list_head_;
  static ThreadPool* thread_pool_;

  DISALLOW_COPY_AND_ASSIGN(Isolate);
};

}  // namespace psoup

#endif  // VM_ISOLATE_H_
