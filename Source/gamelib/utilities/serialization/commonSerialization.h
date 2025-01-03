#pragma once
// #include "../../Entity.hpp"
#define YAML_CPP_STATIC_DEFINE  //if not included yaml will look for a .dll
#include <yaml-cpp/yaml.h>
#include <raylib/raylib.h>

namespace YAML
{
	template<>
	struct convert<Vector2>
	{
		static Node encode(const Vector2& rhs) { Node node; node.push_back(rhs.x); node.push_back(rhs.y); return node; }
		static bool decode(const Node& node, Vector2& rhs) {
			if (!node.IsSequence() || node.size() < 2) return false;
			rhs.x = node[0].as<float>(); rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<Vector3>
	{
		static Node encode(const Vector3& rhs) { Node node; node.push_back(rhs.x); node.push_back(rhs.y); node.push_back(rhs.z); return node; }
		static bool decode(const Node& node, Vector3& rhs) {
			if (!node.IsSequence() || node.size() < 3) return false;
			rhs.x = node[0].as<float>(); rhs.y = node[1].as<float>(); rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<Vector4>
	{
		static Node encode(const Vector4& rhs) { Node node; node.push_back(rhs.x); node.push_back(rhs.y); node.push_back(rhs.z); node.push_back(rhs.w); return node; }
		static bool decode(const Node& node, Vector4& rhs) {
			if (!node.IsSequence() || node.size() < 4) return false;
			rhs.x = node[0].as<float>(); rhs.y = node[1].as<float>(); rhs.z = node[2].as<float>(); rhs.w = node[3].as<float>();
			return true;
		}
	};
	template<>
	struct convert<Rectangle>
	{
		static Node encode(const Rectangle& rhs) { Node node; node.push_back(rhs.x); node.push_back(rhs.y); node.push_back(rhs.width); node.push_back(rhs.height); return node; }
		static bool decode(const Node& node, Rectangle& rhs) {
			if (!node.IsSequence() || node.size() < 4) return false;
			rhs.x = node[0].as<float>(); rhs.y = node[1].as<float>(); rhs.width = node[2].as<float>(); rhs.height = node[3].as<float>();
			return true;
		}
	};
	template<>
	struct convert<Color>
	{
		static Node encode(const Color& rhs) {
			Node node;
			std::stringstream hexValue;
			hexValue.imbue(std::locale("C"));
			hexValue << std::uppercase << std::hex << ColorToInt(rhs);

			node.push_back(hexValue.str());

			return node;
		}
		static bool decode(const Node& node, Color& rhs) {
			if (!node.IsScalar())
				return false;
			unsigned int hexValue;
			std::string s = node.as<std::string>();
			std::istringstream converter(s);
			converter >> std::hex >> hexValue;
			rhs = GetColor(hexValue);

			return true;
		}
	};
}


inline YAML::Emitter& operator<<(YAML::Emitter& out, const Vector2& v)
{
	out << YAML::Flow << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}
inline YAML::Emitter& operator<<(YAML::Emitter& out, const Vector3& v)
{
	out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const Vector4& v)
{
	out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}
inline YAML::Emitter& operator<<(YAML::Emitter& out, const Rectangle& v)
{
	out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.width << v.height << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const Color& v)
{
	std::stringstream hexValue;
	hexValue.imbue(std::locale("C"));
	hexValue << std::uppercase << std::hex << ColorToInt(v);
	out << hexValue.str();
	return out;
}
