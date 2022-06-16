#pragma once
#include <string>

namespace gf3d
{
	enum class resource_type
	{
		eDefault,
		eTexture
	};

	class Resource
	{
	public:

		void setResourceInfo(const char* resourceName, std::string_view path, resource_type type)
		{
			this->resourceName = resourceName;
			resourceFilepath = path;
			resourceType = type;
		}

		const char* name() const noexcept { return resourceName; }
		std::string_view filepath() const noexcept { return resourceFilepath; }
		resource_type type() const noexcept { return resourceType; }

	protected:
		resource_type resourceType = resource_type::eDefault;
		const char* resourceName;
		std::string resourceFilepath;
	};

} // namespace gf3d
