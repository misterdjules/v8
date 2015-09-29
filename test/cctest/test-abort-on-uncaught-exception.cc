// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/v8.h"

#include "test/cctest/cctest.h"

const char* BOOM_FUNC = "function boom() { throw new Error(\"boom\") }";

TEST(AbortOnUncaughtExceptionDefault) {
  v8::Isolate* isolate = CcTest::isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Handle<v8::ObjectTemplate> global_template =
      v8::ObjectTemplate::New(isolate);
  LocalContext env(NULL, global_template);

  i::FLAG_abort_on_uncaught_exception = true;
  CompileRun(BOOM_FUNC);
  
  v8::Local<v8::Object> global_object = env->Global();
  v8::Local<v8::Function> foo =
      v8::Local<v8::Function>::Cast(global_object->Get(v8_str("boom")));

  foo->Call(global_object, 0, NULL);
  /*
  EXPECT_EXIT(foo->Call(global_object, 0, NULL);,
    ::testing::KilledBySignal(SIGKILL),
    "throwing and using --abort-on-uncaught-exception should cause process to abort");
  */
}

bool NoAbortOnUncaughtException(v8::Isolate* isolate) {
  return false;
}

TEST(AbortOnUncaughtExceptionUncaughtExceptionCallbackNoAbort) {
  v8::Isolate* isolate = CcTest::isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Handle<v8::ObjectTemplate> global_template =
      v8::ObjectTemplate::New(isolate);
  LocalContext env(NULL, global_template);

  i::FLAG_abort_on_uncaught_exception = true;
  isolate->SetAbortOnUncaughtExceptionCallback(NoAbortOnUncaughtException);
  
  CompileRun(BOOM_FUNC);
  
  v8::Local<v8::Object> global_object = env->Global();
  v8::Local<v8::Function> foo =
      v8::Local<v8::Function>::Cast(global_object->Get(v8_str("boom")));

  foo->Call(global_object, 0, NULL);
  /*
  EXPECT_EXIT(foo->Call(global_object, 0, NULL);,
    ::testing::KilledBySignal(SIGKILL),
    "throwing, using --abort-on-uncaught-exception and overriding uncaught callback should NOT cause process to abort");
  */
}

