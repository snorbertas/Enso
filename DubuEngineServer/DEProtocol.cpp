#include "DEProtocol.hpp"
#include <string.h>
#include <sstream>

DEPacket MakePacket(int id, const char* info1, const char* info2) {
	DEPacket dep;
	dep.packet_type = id;
	strcpy(dep.packet_info_1, info1);
	strcpy(dep.packet_info_2, info2);
	return dep;
}

DEPacket DEPacket::add1(int data) {
	std::string new_data = packet_info_1 + (std::string)" " + std::to_string(data);
	strcpy(this->packet_info_1, new_data.c_str());
	return *this;
}

DEPacket DEPacket::add1(float data) {
	std::string new_data = packet_info_1 + (std::string)" " + std::to_string(data);
	strcpy(this->packet_info_1, new_data.c_str());
	return *this;
}

DEPacket DEPacket::add1(std::string data) {
	std::string new_data = packet_info_1 + (std::string)" " + data;
	strcpy(this->packet_info_1, new_data.c_str());
	return *this;
}

DEPacket DEPacket::add2(int data) {
	std::string new_data = packet_info_2 + (std::string)" " + std::to_string(data);
	strcpy(this->packet_info_2, new_data.c_str());
	return *this;
}

DEPacket DEPacket::add2(float data) {
	std::string new_data = packet_info_2 + (std::string)" " + std::to_string(data);
	strcpy(this->packet_info_2, new_data.c_str());
	return *this;
}

DEPacket DEPacket::add2(std::string data) {
	std::string new_data = packet_info_2 + (std::string)" " + data;
	strcpy(this->packet_info_2, new_data.c_str());
	return *this;
}