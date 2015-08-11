#pragma once

#include <future>
#include <fstream>
#include <memory>
#include <vector>
#include "EventBus.h"

namespace WheelsOfWarEngine {

	class ResourceManager {
	public:
		typedef std::vector<unsigned char> Buffer;
		typedef std::shared_ptr<Buffer> BufferPtr;
		typedef std::shared_ptr<const Buffer> ConstBufferPtr;

		struct Progress {
			friend class ResourceManager;

			struct ProgressEvent {
				const std::string& path;
				const ConstBufferPtr& buffer;
				const unsigned int count;
				const unsigned int total;
			};

			template<typename H>
			on(const H* handlerPtr) {
				events.on<ProgressEvent>(handlerPtr);
			}

		private:
			EventBus events;
		};

		typedef std::shared_ptr<Progress> ProgressPtr;

		ResourceManager() = default;
		ResourceManager(const ResourceManager&) = default;
		ResourceManager(ResourceManager&&) = default;
		~ResourceManager() = default;

		ResourceManager& operator=(const ResourceManager&) = default;
		ResourceManager& operator=(ResourceManager&&) = default;

		template<class CollectionT>
		ProgressPtr load(const CollectionT& filePaths) {
			auto progressPtr = std::make_shared<Progress>();

			auto future = std::async(std::launch::async, [&]() {
				// Get total count
				const unsigned int total = std::distance(filePaths.begin(), filePaths.end());
				unsigned int count = 0;
				// Load some resources and raise events
				for (const std::string& path : filePaths) {
					ifstream in(path, ios_base::in | ios_base::binary);

					auto buffer = std::make_shared<Buffer>(
						std::istreambuf_iterator<unsigned char>(in),
						std::istreambuf_iterator<unsigned char>());

					this->resources.insert_or_assign(path, buffer);

					progressPtr->events.send(Progress::ProgressEvent{ path, buffer, ++count, total } );
				}
			});

			return progressPtr;
		}

		Resource get(const PathString& path) const {
			auto it = this->resources.find(path);
			if (it == this->resources.end()) {
				return Resource{path}; // empty ResourcePtr
			} else {
				return Resource{path, it->second};
			}
		}

	private:
		std::unordered_map<std::string, BufferPtr> resources;
	};

}