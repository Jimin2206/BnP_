//#pragma once
//
//void BnP();

//#pragma once
//#include "BnPNode.h"
//#include <string>
//#include <iomanip>
//
//std::string VarBoundsKey(const BnPNode& node) {
//	std::vector<std::string> parts;
//	for (const auto& vb : node.var_bounds) {
//		std::ostringstream oss;
//		oss << vb.first << ":"
//			<< std::fixed << std::setprecision(5)
//			<< vb.second.first << ","
//			<< vb.second.second;
//		parts.push_back(oss.str());
//	}
//	std::sort(parts.begin(), parts.end());
//	std::ostringstream result;
//	for (const auto& part : parts) {
//		result << part << "|";
//	}
//	return result.str();
//}
