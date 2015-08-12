#include "V8ScriptComponent.hpp"

WheelsOfWarEngine::V8ScriptComponent::V8ScriptComponent(const std::string& script) {

}

WheelsOfWarEngine::V8ScriptComponent::V8ScriptComponent(const V8ScriptComponent&) {

}

WheelsOfWarEngine::V8ScriptComponent::Platform::Platform() {
	// Initialize V8.
	v8::V8::InitializeICU();
	std::unique_ptr<v8::Platform> platformPtr(v8::platform::CreateDefaultPlatform());
	v8::V8::InitializePlatform(platformPtr.get());
	v8::V8::Initialize();

	this->platformPtr = std::move(platformPtr);
}

WheelsOfWarEngine::V8ScriptComponent::Platform::~Platform() {
	V8::Dispose();
	V8::ShutdownPlatform();
}

WheelsOfWarEngine::V8ScriptComponent::Context::Context() {
	this->allocatorPtr = std::make_unique<ArrayBufferAllocator>();

	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = this->allocatorPtr.get();

	this->isolatePtr = std::unique_ptr<v8::Isolate>(v8::Isolate::New(create_params));
}

template<class R>
R WheelsOfWarEngine::V8ScriptComponent::Context::run(const std::string& script) const {
	v8::Isolate::Scope isolate_scope(this->isolatePtr);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(this->isolatePtr);

    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate);

}

WheelsOfWarEngine::V8ScriptComponent::Factory::Factory(const shared_ptr<ResourceManager>& resources) 
: resourcesPtr(resources), ctxPtr(std::make_unique<Context>()) {

}

V8ScriptComponent WheelsOfWarEngine::V8ScriptComponent::Factory::create(const std::string& scriptFile) {

}