#pragma once

#include <memory>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"
#include "ScriptComponent.hpp"
#include "../Factory.hpp"
#include "../ResourceManager.h"

namespace WheelsOfWarEngine {

class V8ScriptComponent : public ScriptComponent {
private:
  explicit V8ScriptComponent(const std::string& script);
  V8ScriptComponent(const V8ScriptComponent&);
  V8ScriptComponent(V8ScriptComponent&&) = default;
  ~V8ScriptComponent() = default;
  
private:
  class Platform {
  private:
    Platform();
  public:
    Platform(const Platform&) = delete;
    Platform(Platform&&) = delete;
    ~Platform();

    static const Platform& getInstance() {
      static Platform platform;
      return platform;
    }
  private:
    std::unique_ptr<v8::Platform> platformPtr;
};

  class Context {
  private:
    class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
     public:
      virtual void* Allocate(size_t length) {
        void* data = AllocateUninitialized(length);
        return data == NULL ? data : memset(data, 0, length);
      }
      virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
      virtual void Free(void* data, size_t) { free(data); }
    };

  public:
    explicit Context(const Platform& platform);
    Context(const Context&) = delete;
    Context(Context&&) = default;
    ~Context() = default;

    template<class R>
    R run(const std::string& script) const;

  private:
    std::unique_ptr<ArrayBufferAllocator> allocatorPtr;
    std::unique_ptr<v8::Isolate> isolatePtr;
  };

public:
  class Factory : public WheelsOfWarEngine::Factory<V8ScriptComponent> {
  public:
    explicit Factory(const shared_ptr<ResourceManager>& resources);
    virtual ~Factory() = default;
    virtual V8ScriptComponent create(const std::string& scriptFile);
  private:
    shared_ptr<ResourceManager> resourcesPtr;
    unique_ptr<Context> ctxPtr;
  };
};

}


int main(int argc, char* argv[]) {
  // Initialize V8.
  V8::InitializeICU();
  v8::Platform* platform = v8::platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();

  // Create a new Isolate and make it the current one.
  ArrayBufferAllocator allocator;
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = &allocator;
  v8::Isolate* isolate = v8::Isolate::New(create_params);
  {
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate);

    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);

    // Create a string containing the JavaScript source code.
    v8::Local<v8::String> source =
        v8::String::NewFromUtf8(isolate, "'Hello' + ', World!'",
                            v8::NewStringType::kNormal).ToLocalChecked();

    // Compile the source code.
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

    // Run the script to get the result.
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    // Convert the result to an UTF8 string and print it.
    v8::String::Utf8Value utf8(result);
    printf("%s\n", *utf8);
  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  return 0;
}